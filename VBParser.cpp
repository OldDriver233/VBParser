// VBParser.cpp: 定义应用程序的入口点。
//
#include "VBParser.h"
#include "Token.h"
#include "Parse.h"
#include "Runner.h"
#include<iostream>
#include<stack>
#include<map>

using namespace std;

void Check()
{
	Token t = Token();
	Token t1 = Token();
	map<string, int> mIndex;
	vector<Token> vec;
	vector<Variable> mVar;
	t = 4.5;
	t1 = 5;
	t = t * t1;
	cout << t << endl;
	vec.push_back(Token());
	vec.clear();
	Parse(vec, "Dim a");
	runner(vec, mIndex, mVar);
	vec.clear();
	Parse(vec, "a = 1 * 4");
	runner(vec, mIndex, mVar);
}

int main(int argc, char** argv)
{
	vector<vector<Token> >parseResult;
	map<string, int> varIndex;
	vector<Variable> varVec;
	string inputStr;
	int index = 0;
	//Check();
	cout << "=>";
	while (getline(cin,inputStr))
	{
		vector<Token> vec;
		try
		{
			Parse(vec, inputStr);
			parseResult.push_back(vec);
			runner(vec,varIndex,varVec);
		}
		catch (const std::exception& e)
		{
			cout << e.what() << endl;
		}
		cout << "=>";
		vec.clear();
	}
	return 0;
}
