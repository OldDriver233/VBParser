// VBParser.cpp: 定义应用程序的入口点。
//

#include "VBParser.h"
#include "Token.h"

using namespace std;

int main()
{
	Token t = Token();
	t = 4.5;
	t = t.typeCast(t, INT);
	cout << t << endl;
	return 0;
}
