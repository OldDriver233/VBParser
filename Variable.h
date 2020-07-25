#include"Token.h"
#include<string>
#include<vector>

class Variable
{
public:
	std::vector<Token> var;
	bool single = false;
	int endIndex;
	Variable();
	Variable(int);
	Variable(Variable&);
	~Variable();

	Variable operator=(Variable& var);

	Token getToken();
	Token getToken(int);
	void setToken(Token&);
	void setToken(Token&, int);
};

inline Variable::Variable()
{
	var.reserve(3);
	endIndex = 0;
	single = true;
}

inline Variable::Variable(int size)
{
	var.reserve(size+2);
	if (size == 1)single = true;
	endIndex = size - 1;
}

inline Variable::Variable(Variable& var)
{
	*this = var;
}

Variable::~Variable()
{
}

inline Variable Variable::operator=(Variable& var)
{
	this->endIndex = var.endIndex;
	this->single = var.single;
	this->var = var.var;
	return *this;
}

inline Token Variable::getToken()
{
	return var[0];
}

inline Token Variable::getToken(int index)
{
	if(single || index > endIndex) throw std::runtime_error("无法获取元素");
	else return var[index];
}

inline void Variable::setToken(Token& t)
{
	var[0] = t;
}

inline void Variable::setToken(Token& t, int index)
{
	if (single || index > endIndex) throw std::runtime_error("无法设置元素");
	else var[index] = t;
}
