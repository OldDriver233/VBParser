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
	vector<Token> vec;
	t = 4.5;
	t1 = 5;
	t = t * t1;
	cout << t << endl;
	Parse(vec, "3 + 1 * 4");
	cout << calculate(vec, 0, vec.size()-1);
}

int main(int argc, char** argv)
{
	vector<vector<Token> >parseResult;
	string inputStr;
	int index = 0;
	//Check();
	cout << "=>";
	while (getline(cin,inputStr))
	{
		vector<Token> vec;
		Parse(vec, inputStr);
		parseResult.push_back(vec);
		cout << calculate(parseResult[index], 0, parseResult[index].size() - 1) << endl;
		cout << "=>";
	}
	return 0;
}
