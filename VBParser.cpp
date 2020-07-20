// VBParser.cpp: 定义应用程序的入口点。
//
#include "VBParser.h"
#include "Token.h"
#include "Parse.h"
#include "Runner.h"
#include<iostream>
#include<stack>

using namespace std;

void Check()
{
	Token t = Token();
	Token t1 = Token();
	vector<Token> vec;
	t = 4.5;
	t1 = 5;
	t = t * t1;
	cout << t << endl;
	Parse(vec, "rr怒怒怒 + 1");
	calculate(vec, 0, 2);
}

int main()
{
	vector<vector<Token> >parseResult;
	string inputStr;
	int index = 0;
	Check();
	while (getline(cin,inputStr))
	{
		vector<Token> vec;
		Parse(vec, inputStr);
		parseResult.push_back(vec);
	}
	return 0;
}
