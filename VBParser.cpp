// VBParser.cpp: 定义应用程序的入口点。
//
#include "VBParser.h"
#include "Token.h"
#include "Parse.h"
#include "Runner.h"
#include "execStat.h"
#include<iostream>
#include<stack>
#include<map>
#include<tuple>
#include<cstdio>

using namespace std;

void Check()
{
	Token t = Token();
	Token t1 = Token();
	map<string, int> mIndex;
	vector<Token> vec;
	vector<Variable> mVar;
	stack<tuple<int, string, execStat>> mStack;
	t = "1";
	t1 = "1";
	cout << (t == t1);
	vec.push_back(Token());
	vec.clear();
	Parse(vec, "Dim a");
	mStack.push(make_tuple<int, string>(std::move(0), "<Main>", normal));
	runner(vec, mIndex, mVar, mStack);
	vec.clear();
	Parse(vec, "a = 1 > 2 And 1 < 2");
	mStack.push(make_tuple<int, string>(std::move(0), "<Main>", normal));
	runner(vec, mIndex, mVar, mStack);
}

int main(int argc, char** argv)
{
	vector<vector<Token> >parseResult;
	map<string, int> varIndex;
	vector<Variable> varVec;
	string inputStr;
	stack<tuple<int,string,execStat>> runStack;
	tuple<Token, execStat> tp;
	int index = 0;
	int forLayer = 0;
	int phaseStart;
	//Check();
	tp = make_tuple<Token, execStat>(Token(), normal);
	if (argc > 1)freopen(argv[1], "r", stdin);
	while (true)
	{
		vector<Token> vec;
		execStat stat;
		int newIndex;
		if (runStack.empty())stat = normal;
		else stat = get<2>(runStack.top());
		try
		{
			if (stat != forExec && stat != forNext)
			{
				if (!getline(cin, inputStr))break;
				Parse(vec, inputStr);
				parseResult.push_back(vec);
				if (vec[0].StringGet() == "For")
				{
					forLayer += 1;
					if (forLayer == 1)phaseStart = index;
				}
				else if (vec[0].StringGet() == "Next")
				{
					forLayer -= 1;
				}
				else if(forLayer == 0)phaseStart = index;
				if (forLayer == 0)
				{
					index = phaseStart;
				}
			}
			if (forLayer == 0)
			{
				runStack.push(make_tuple<int, string, execStat>(std::move(index), "<Main>", move(stat)));
				tp = runner(parseResult[index], varIndex, varVec, runStack);
			}
			if (get<1>(tp) == forNext)
			{
				newIndex = get<0>(runStack.top());
				string func = get<1>(runStack.top());
				runStack.pop();
				runStack.push(make_tuple<int, string, execStat>(std::move(newIndex), "<Main>", forNext));
				tp = runner(parseResult[newIndex], varIndex, varVec, runStack);
				if (get<1>(tp) != forCondFalse)
				{
					index = newIndex;
				}
				else
				{
					runStack.pop();
				}
			}
			vec.clear();
			index++;
		}
		catch (const std::exception& e)
		{
			cout << e.what() << endl;
			vec.clear();
			index++;
		}
	}
	return 0;
}
