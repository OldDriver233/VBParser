// VBParser.cpp: ����Ӧ�ó������ڵ㡣
//

#include "VBParser.h"
#include "Token.h"

using namespace std;

int main()
{
	Token t = Token();
	Token t1 = Token();
	t = 4.5;
	t1 = "5";
	t = t * t1;
	cout << t << endl;
	return 0;
}
