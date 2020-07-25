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
	map<string, Variable> m;
	vector<Token> vec;
	t = 4.5;
	t1 = 5;
	t = t * t1;
	cout << t << endl;
	Parse(vec, "Dim a");
	runner(vec,m);
	vec.clear();
	Parse(vec, "a = 1 * 4");
	runner(vec, m);
}

int main(int argc, char** argv)
{
	vector<vector<Token> >parseResult;
	map<string, Variable> varMap;
	string inputStr;
	int index = 0;
	Check();
	cout << "=>";
	while (getline(cin,inputStr))
	{
		vector<Token> vec;
		Parse(vec, inputStr);
		parseResult.push_back(vec);
		cout << calculate(parseResult[index], 0, parseResult[index].size() - 1,varMap) << endl;
		cout << "=>";
		vec.clear();
	}
	return 0;
}
