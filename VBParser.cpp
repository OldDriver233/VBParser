// VBParser.cpp: 定义应用程序的入口点。
//

#include "VBParser.h"
#include "Token.h"
#include "Parse.cpp"

using namespace std;

int main()
{
	Token t = Token();
	Token t1 = Token();
	vector<Token> vec;
	t = 4.5;
	t1 = 5;
	t = t * t1;
	cout << t << endl;
	Parse(vec,"rr怒怒怒 + 1");
	return 0;
}
