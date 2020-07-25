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
	~Variable();

	Variable operator=(const Variable& var);

	Token getToken();
	Token getToken(int);
	void setToken(Token&);
	void setToken(Token&, int);
};

inline Variable::Variable()
{
	endIndex = 0;
	single = true;
}

Variable::~Variable()
{
}

inline Variable Variable::operator=(const Variable& var)
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
