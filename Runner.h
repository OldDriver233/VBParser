#ifndef _Runner
#define _Runner
#include "Token.h"
#include<vector>
#include<cmath>
#include<map>

struct calcUnit
{
	Token cont;
	int nextVal = 1;
	char operate = '\0';
};

std::string prior[] = {"^","*/","+-"};

Token singleCalc(Token& t1, Token& t2, char& operate)
{
	Token result;
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
	default:
		break;
	}
	return result;
}

Token calculate(std::vector<Token>& input, int begin, int end)
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
							tmp.cont = calculate(input, index+1, j-1);
							break;
						}
					}
				}
				else
				{
					throw std::runtime_error("未完工");
				}
			}
		}
		else
		{
			tmp.cont = input[i];
			withVal = true;
		}
	}
	if (withVal)
	{
		tmp.nextVal = -1;
		vecUnit.push_back(tmp);
	}
	for (auto i = 0; i < 3; i++)
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

Token runner()
{
	Token returnValue;
	return returnValue;
}
#endif