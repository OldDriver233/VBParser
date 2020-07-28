#ifndef _Parse
#define _Parse
#include"Token.h"
#include<vector>
std::string pattern = "=+_*/%><()&|";
//？？？

void Parse(std::vector<Token>& tInput, const std::string& sInput)
{
	tokenType stat;
	bool inStat = false;
	bool kwd = false;
	int tmpInt = 0;
	double tmpDbl = 0.0;
	std::string tmpStr = "";
	double dCount = 0;
	for (long long i = 0; i < sInput.size(); i++)
	{
		if (!inStat)
		{
			if (sInput[i] == ' ')continue;
			else if (sInput[i] >= '0' && sInput[i] <= '9')
			{
				inStat = true;
				tmpInt = sInput[i] - '0';
				stat = INT;
			}
			else if (pattern.find(sInput[i]) != pattern.npos)
			{
				Token object = Token();
				object = sInput[i];
				object.keyword = true;
				object.changeable = false;
				tInput.push_back(object);
			}
			else if (sInput[i] == '\'' || sInput[i] == '\"')
			{
				inStat = true;
				kwd = false;
				stat = STR;
			}
			else
			{
				inStat = true;
				kwd = true;
				stat = STR;
				tmpStr += sInput[i];
			}
		}
		else
		{
			if (sInput[i] == ' ' || pattern.find(sInput[i]) != pattern.npos)
			{
				Token object = Token();
				Token inp;
				inStat = false;
				switch (stat)
				{
				case INT:
					object = tmpInt;
					tmpInt = 0;
					tInput.push_back(object);
					break;
				case CHAR:
					break;
				case DOUBLE:
					object = tmpDbl;
					tmpDbl = 0;
					dCount = 0;
					tInput.push_back(object);
					break;
				case STR:
					object = tmpStr;
					tmpStr = "";
					if (kwd)
					{
						object.changeable = false;
						object.keyword = true;
					}
					inp = Token(object);
					tInput.push_back(inp);
					break;
				default:
					break;
				}
				if (sInput[i] != ' ')
				{
					Token object = Token();
					object = sInput[i];
					object.keyword = true;
					object.changeable = false;
					tInput.push_back(object);
				}
			}
			else if (sInput[i] >= '0' && sInput[i] <= '9')
			{
				switch (stat)
				{
				case INT:
					tmpInt = sInput[i] - '0' + tmpInt * 10;
					break;
				case CHAR:
					break;
				case DOUBLE:
					tmpDbl = tmpDbl + (sInput[i] - '0') * dCount;
					dCount *= 0.1;
					break;
				case STR:
					tmpStr = tmpStr + sInput[i];
					break;
				default:
					break;
				}
			}
			else if (sInput[i] == '\'' || sInput[i] == '\"')
			{
				if (sInput[i - 1] != '\\')
				{
					inStat = false;
					Token object = Token();
					object = tmpStr;
					tmpStr = "";
					tInput.push_back(object);
				}
				else
				{
					tmpStr = tmpStr + sInput[i];
				}
			}
			else if(sInput[i] == '.')
			{
				switch (stat)
				{
				case INT:
				case CHAR:
					stat = DOUBLE;
					tmpDbl = tmpInt;
					dCount = 0.1;
					break;
				case DOUBLE:
				case STR:
					throw std::runtime_error("错误的\'.\'");
					break;
				default:
					break;
				}
			}
			else
			{
				Token object = Token();
				switch (stat)
				{
				case INT:
					object = tmpInt;
					tmpInt = 0;
					tInput.push_back(object);
					inStat = false;
					i -= 1;
					break;
				case CHAR:
					break;
				case DOUBLE:
					object = tmpDbl;
					tmpDbl = 0;
					tInput.push_back(object);
					inStat = false;
					i -= 1;
					break;
				case STR:
					tmpStr = tmpStr + sInput[i];
					break;
				default:
					break;
				}
			}
		}
	}
	if (inStat)
	{
		Token object = Token();
		switch (stat)
		{
		case INT:
			object = tmpInt;
			tmpInt = 0;
			tInput.push_back(object);
			break;
		case CHAR:
			break;
		case DOUBLE:
			object = tmpDbl;
			tmpDbl = 0;
			tInput.push_back(object);
			break;
		case STR:
			object = tmpStr;
			tmpStr = "";
			if (kwd)
			{
				object.changeable = false;
				object.keyword = true;
			}
			tInput.push_back(object);
			break;
		default:
			break;
		}
	}
}
#endif