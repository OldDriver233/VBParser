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
	//Check();
	cout << "=>";
	tp = make_tuple<Token, execStat>(Token(), normal);
	while (getline(cin,inputStr))
	{
		vector<Token> vec;
		execStat stat;
		if (runStack.empty())stat = normal;
		else stat = get<2>(runStack.top());
		try
		{
			Parse(vec, inputStr);
			parseResult.push_back(vec);
			runStack.push(make_tuple<int,string,execStat>(std::move(index),"<Main>",move(stat)));
			tp = runner(vec,varIndex,varVec,runStack);
		}
		catch (const std::exception& e)
		{
			cout << e.what() << endl;
		}
		cout << "=>";
		vec.clear();
		index++;
	}
	return 0;
}
