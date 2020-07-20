#ifndef _Runner
#define _Runner
#include "Token.h"
#include<vector>
#include<cmath>
struct calcUnit
{
	Token cont;
	int nextVal = 1;
	char operate = '\0';
};

Token calculate(const std::vector<Token>& input, int begin, int end)
{
	Token result;
	std::vector<calcUnit> vecUnit;
	calcUnit tmp;
	int counter = 0;
	bool withVal = false;
	for (auto i : input)
	{
		if (i.CharGet() != '\0' && i.keyword)
		{
			if (withVal)
			{
				withVal = false;
				tmp.operate = i.CharGet();
				vecUnit.push_back(tmp);
				tmp.operate = '\0';
				tmp.nextVal = ++counter;
			}
			else
			{
				throw std::runtime_error("");
			}
		}
		else
		{
			tmp.cont = i;
			withVal = true;
		}
	}
	if (withVal)
	{
		tmp.nextVal = -1;
		vecUnit.push_back(tmp);
	}
	return result;
}

Token runner()
{
	Token returnValue;
	return returnValue;
}
#endif