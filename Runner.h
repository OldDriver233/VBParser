#ifndef _Runner
#define _Runner
#include "Token.h"
#include"Variable.h"
#include"execStat.h"
#include<vector>
#include<cmath>
#include<map>
#include<tuple>
#include<stack>

struct calcUnit
{
	Token cont;
	int nextVal = 1;
	char operate = '\0';
};

std::string prior[] = {"^","*/","+-","<>={}`","!","&","|"};

Token singleCalc(Token& t1, Token& t2, char& operate)
{
	Token result,tOne,tZero;
	tOne = 1;
	tZero = 0;
	switch (operate)
	{
	case '+':
		result = t1 + t2; 
		break;
	case '-':
		result = t1 - t2; 
		break;
	case '*':
		result = t1 * t2; 
		break;
	case '/':
		result = t1 / t2; 
		break;
	case '^':
		break;
	case '<':
		result = t1 < t2;
		break;
	case '>':
		result = t1 > t2;
		break;
	case '{':
		result = t1 <= t2;
		break;
	case '}':
	case '!':
		result = t1 >= t2;
		break;
	case '=':
		result = t1 == t2;
		break;
	case '`':
		result = t1 != t2;
		break;
	case '|':
		if (t1.BoolGet() || t2.BoolGet())result = tOne;
		else result = tZero;
		break;
	case '&':
		if (t1.BoolGet() && t2.BoolGet())result = tOne;
		else result = tZero;
		break;
	default:
		break;
	}
	return result;
}

Token calculate(std::vector<Token>& input, int begin, int end, std::map<std::string,int>& varIndex, std::vector<Variable>& vars)
{
	Token result;
	std::vector<calcUnit> vecUnit;
	calcUnit tmp;
	int counter = 1;
	bool withVal = false;
	for (auto i = begin; i<=end; i++)
	{
		if (input[i].CharGet() != '\0' && input[i].keyword)
		{
			if (withVal)
			{
				withVal = false;
				tmp.operate = input[i].CharGet();
				vecUnit.push_back(tmp);
				tmp.operate = '\0';
				tmp.nextVal = ++counter;
			}
			else
			{
				if (input[i].CharGet() == '(')
				{
					auto index = i, bracket = 1;
					for (auto j = index + 1; j <= end; j++)
					{
						if (input[j].CharGet() == ')')bracket--;
						else if (input[j].CharGet() == '(')bracket++;
						if (bracket == 0)
						{
							i = j;
							withVal = true;
							tmp.cont = calculate(input, index+1, j-1, varIndex, vars);
							break;
						}
					}
				}
				if (input[i].CharGet() == '=')
				{
					if (input[i - 1].CharGet() == '<')
					{
						vecUnit[counter - 2].operate = '{';
					}
					if (input[i - 1].CharGet() == '>')
					{
						vecUnit[counter - 2].operate = '}';
					}
				}
				if (input[i].CharGet() == '>' && input[i - 1].CharGet() == '<')
				{
					vecUnit[counter - 2].operate = '`';
				}
				else
				{
					throw std::runtime_error("未完工");
				}
			}
		}
		else if(!input[i].keyword)
		{
			tmp.cont = input[i];
			withVal = true;
		}
		else
		{
			if (input[i].StringGet() == "And")
			{
				if (withVal)
				{
					withVal = false;
					tmp.operate = '&';
					vecUnit.push_back(tmp);
					tmp.operate = '\0';
					tmp.nextVal = ++counter;
				}
				else throw std::runtime_error("错误的And");
			}
			else if (input[i].StringGet() == "Or")
			{
				if (withVal)
				{
					withVal = false;
					tmp.operate = '|';
					vecUnit.push_back(tmp);
					tmp.operate = '\0';
					tmp.nextVal = ++counter;
				}
				else throw std::runtime_error("错误的Or");
			}
			else if (input[i].StringGet() == "Not")
			{
				if (!withVal)
				{
					tmp.cont = 0;
					tmp.operate = '0';
					vecUnit.push_back(tmp);
					tmp.nextVal = ++counter;
					tmp.operate = '!';
					vecUnit.push_back(tmp);
					tmp.operate = '\0';
					tmp.nextVal = ++counter;
				}
				else throw std::runtime_error("错误的Not");
			}
			else
			{
				tmp.cont = vars[varIndex[input[i].StringGet()]].var[0];
				withVal = true;
			}
		}
	}
	if (withVal)
	{
		tmp.nextVal = -1;
		vecUnit.push_back(tmp);
	}
	for (auto i = 0; i < 7; i++)
	{
		for (auto j = 0; j!=-1 && vecUnit[j].nextVal != -1; j = vecUnit[j].nextVal)
		{
			if (prior[i].find(vecUnit[j].operate) != prior[i].npos)
			{
				vecUnit[j].cont = singleCalc(vecUnit[j].cont, vecUnit[vecUnit[j].nextVal].cont, vecUnit[j].operate);
				vecUnit[j].operate = vecUnit[vecUnit[j].nextVal].operate;
				vecUnit[j].nextVal = vecUnit[vecUnit[j].nextVal].nextVal;
			}
		}
	}
	result = vecUnit[0].cont;
	return result;
}
//TODO:修复栈
/*
通过读取栈顶元素决定运行状态。
对于返回值先不作变动。
 */
std::tuple<Token,execStat> runner(std::vector<Token>& input, std::map<std::string,int>& varIndex, std::vector<Variable>& vars, std::stack<std::tuple<int,std::string,execStat> >& sta)
{
	Token returnValue;
	if (sta.empty())return std::make_tuple<Token, execStat>(std::move(returnValue), normal);
	execStat exs = std::get<2>(sta.top());
	std::string str = std::get<1>(sta.top());
	int index = std::get<0>(sta.top());
	returnValue = 0;
	sta.pop();
	if (input[0].StringGet() == "If")
	{
		if (exs != normal && exs != ifExec)
		{
			return std::make_tuple<Token, execStat>(std::move(returnValue), std::move(exs));
		}
		else
		{
			int minus = input[input.size() - 1].StringGet() == "Then"? 2: 1;
			if (calculate(input, 1, input.size() - minus, varIndex, vars).BoolGet())
			{
				sta.push(std::make_tuple<int, std::string, execStat>(std::move(index), std::move(str), ifExec));
				return std::make_tuple<Token, execStat>(std::move(returnValue), ifExec);
			}
			else
			{
				sta.push(std::make_tuple<int, std::string, execStat>(std::move(index), std::move(str), condFalse));
				return std::make_tuple<Token, execStat>(std::move(returnValue), condFalse);
			}
		}
	}
	else if (input[0].StringGet() == "ElseIf")
	{
		if (exs == ifExec || exs == ifEnd)
		{
			sta.pop();
			sta.push(std::make_tuple<int, std::string, execStat>(std::move(index), std::move(str), ifEnd));
			return std::make_tuple<Token, execStat>(std::move(returnValue), ifEnd);
		}
		else
		{
			int minus = input[input.size() - 1].StringGet() == "Then" ? 2 : 1;
			if (calculate(input, 1, input.size() - minus, varIndex, vars).BoolGet())
			{
				sta.pop();
				sta.push(std::make_tuple<int, std::string, execStat>(std::move(index), std::move(str), ifExec));
				return std::make_tuple<Token, execStat>(std::move(returnValue), ifExec);
			}
			else
			{
				sta.pop();
				sta.push(std::make_tuple<int, std::string, execStat>(std::move(index), std::move(str), condFalse));
				return std::make_tuple<Token, execStat>(std::move(returnValue), condFalse);
			}
		}
	}
	else if (input[0].StringGet() == "Else")
	{
		if (exs == condFalse)
		{
			sta.pop();
			sta.push(std::make_tuple<int, std::string, execStat>(std::move(index), std::move(str), ifExec));
			return std::make_tuple<Token, execStat>(std::move(returnValue), ifExec);
		}
		else
		{
			sta.pop();
			sta.push(std::make_tuple<int, std::string, execStat>(std::move(index), std::move(str), ifEnd));
			return std::make_tuple<Token, execStat>(std::move(returnValue), ifEnd);
		}
	}
	else if(input[0].StringGet() == "End" && input[1].StringGet() == "If")
	{
		sta.pop();
		if (sta.empty())exs = normal;
		else exs = std::get<2>(sta.top());
		return std::make_tuple<Token, execStat>(std::move(returnValue), std::move(exs));
	}

	if (input[0].StringGet() == "For")
	{
		int toPos = -1, stepPos = -1;
		Token one,zero,step,ext;
		one = 1;
		zero = 0;
		for (int i = 2; i < input.size(); i++)
		{
			if (input[i].StringGet() == "To")toPos = i;
			if (input[i].StringGet() == "Step")stepPos = i;
		}
		ext = calculate(input, toPos + 1,stepPos == -1 ? input.size()-1: stepPos - 1, varIndex, vars);
		if (exs != forNext)
		{
			varIndex[input[1].StringGet()] = vars.size();
			vars.push_back(Variable());
			vars[vars.size() - 1].var.push_back(Token());
			vars[vars.size() - 1].var[0] = calculate(input, 3, toPos - 1, varIndex, vars);
		}
		else
		{
			if (stepPos == -1)step = 1;
			else step = calculate(input, stepPos + 1, input.size() - 1, varIndex, vars);
			vars[varIndex[input[1].StringGet()]].var[0] = vars[varIndex[input[1].StringGet()]].var[0] + step;
		}
		if (((step > zero).BoolGet() && (vars[varIndex[input[1].StringGet()]].var[0] > ext).BoolGet()) || ((step < zero).BoolGet() && (vars[varIndex[input[1].StringGet()]].var[0] < ext).BoolGet()))
		{
			sta.push(std::make_tuple<int, std::string, execStat>(std::move(index), std::move(str), forCondFalse));
			return std::make_tuple<Token, execStat>(std::move(returnValue), forCondFalse);
		}
		else
		{
			sta.push(std::make_tuple<int, std::string, execStat>(std::move(index), std::move(str), forExec));
			return std::make_tuple<Token, execStat>(std::move(returnValue), forExec);
		}
	}
	else if (input[0].StringGet() == "Next")
	{
		return std::make_tuple<Token, execStat>(std::move(returnValue), forNext);
	}

	if(exs != ifExec && exs != normal && exs != forExec)return std::make_tuple<Token, execStat>(std::move(returnValue), std::move(exs));

	if (input[0].StringGet() == "Dim")
	{
		varIndex[input[1].StringGet()] = vars.size();
		vars.push_back(Variable());
		vars[vars.size() - 1].var.push_back(Token());
	}
	else if(input[1].CharGet() == '=')
	{
		Token result = calculate(input, 2, input.size() - 1, varIndex, vars);
		vars[varIndex[input[0].StringGet()]].var[0] = result;
		returnValue = result;
	}
	else if(input[0].StringGet() == "Print")
	{
		std::cout << calculate(input, 1, input.size() - 1, varIndex, vars) << std::endl;
	}
	else
	{
		throw std::runtime_error("未完成");
	}
	return std::make_tuple<Token, execStat>(std::move(returnValue), std::move(exs));
}
#endif