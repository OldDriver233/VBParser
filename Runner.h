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
						vecUnit[counter - 2].operate == '}';
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

std::tuple<Token,execStat::execStat> runner(std::vector<Token>& input, std::map<std::string,int>& varIndex, std::vector<Variable>& vars, std::stack<std::tuple<int,std::string,execStat::execStat> >& sta)
{
	Token returnValue;
	if (sta.empty())return std::make_tuple<Token, execStat::execStat>(std::move(returnValue), execStat::normal);
	execStat::execStat exs = std::get<2>(sta.top());
	returnValue = 0;
	sta.pop();
	if (input[0].StringGet() == "If")
	{
		if (exs != execStat::normal && exs != execStat::ifExec)
		{
			return std::make_tuple<Token, execStat::execStat>(std::move(returnValue), std::move(exs));
		}
		else
		{
			if (calculate(input, 1, input.size() - 1, varIndex, vars).BoolGet())
			{
				return std::make_tuple<Token, execStat::execStat>(std::move(returnValue), execStat::ifExec);
			}
			else
			{
				return std::make_tuple<Token, execStat::execStat>(std::move(returnValue), execStat::condFalse);
			}
		}
	}
	else if (input[0].StringGet() == "ElseIf")
	{
		if (exs == execStat::ifExec || exs == execStat::ifEnd)return std::make_tuple<Token, execStat::execStat>(std::move(returnValue), execStat::ifEnd);
		else
		{
			if (calculate(input, 1, input.size() - 1, varIndex, vars).BoolGet())
			{
				return std::make_tuple<Token, execStat::execStat>(std::move(returnValue), execStat::ifExec);
			}
			else
			{
				return std::make_tuple<Token, execStat::execStat>(std::move(returnValue), execStat::condFalse);
			}
		}
	}
	else if (input[0].StringGet() == "Else")
	{
		if(exs == execStat::condFalse)return std::make_tuple<Token, execStat::execStat>(std::move(returnValue), execStat::ifExec);
		else return std::make_tuple<Token, execStat::execStat>(std::move(returnValue), execStat::ifEnd);
	}
	else if(input[0].StringGet() == "End")
	{
		return std::make_tuple<Token, execStat::execStat>(std::move(returnValue), execStat::normal);
	}

	if(exs != execStat::ifExec && exs != execStat::normal)return std::make_tuple<Token, execStat::execStat>(std::move(returnValue), std::move(exs));

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
		std::cout << calculate(input,1,input.size()-1,varIndex,vars) << std::endl;
	}
	else
	{
		throw std::runtime_error("未完成");
	}
	return std::make_tuple<Token, execStat::execStat>(std::move(returnValue), std::move(exs));
}
#endif