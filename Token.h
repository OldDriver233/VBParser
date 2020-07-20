#ifndef _Token
#define _Token
#include<iostream>
#include<string>
#include<exception>
enum tokenType
{
	INT,
	CHAR,
	DOUBLE,
	STR
};

class Token
{
public:
	Token();
	Token(const Token&);
	~Token();

	Token& operator=(const Token&);
	Token& operator=(int);
	Token& operator=(double);
	Token& operator=(char);
	Token& operator=(const std::string&);

	friend std::ostream& operator<<(std::ostream&,const Token&);
	Token operator+(Token);
	Token operator-(Token);
	Token operator*(Token);
	Token operator/(Token);

	friend Token typeCast(const Token&, tokenType);
	char CharGet();

	tokenType tType;
	bool changeable;
	bool keyword;
private:
	union
	{
		char cval;
		int ival;
		double dval;
		std::string sval;
	};
	void copyToken(const Token&);
};

inline Token::Token()
{
	tType = INT;
	changeable = true;
	keyword = false;
	ival = 0;
}

inline Token::Token(const Token& t)
{
	copyToken(t);
}

inline Token::~Token()
{
	if (tType == STR)sval.~basic_string();
}

void Token::copyToken(const Token& t)
{
	tType = t.tType;
	//std::cout << tType;
	switch (t.tType)
	{
	case INT: ival = t.ival; break;
	case DOUBLE: dval = t.dval; break;
	case CHAR: cval = t.cval; break;
	case STR: new(&sval) std::string(t.sval); break;
	default:break;
	}
	changeable = t.changeable;
	keyword = t.keyword;
}

Token& Token::operator=(const Token& t)
{
	if (tType == STR && t.tType != STR)sval.~basic_string();
	if (tType == STR && t.tType == STR)sval = t.sval;
	else copyToken(t);
	tType = t.tType;
	return *this;
}

Token& Token::operator=(int i)
{
	if (tType == STR)sval.~basic_string();
	ival = i;
	tType = INT;
	return *this;
}

Token& Token::operator=(double d)
{
	if (tType == STR)sval.~basic_string();
	dval = d;
	tType = DOUBLE;
	return *this;
}

Token& Token::operator=(char c)
{
	if (tType == STR)sval.~basic_string();
	cval = c;
	tType = CHAR;
	return *this;
}

Token& Token::operator=(const std::string& s)
{
	if (tType == STR)sval = s;
	else new(&sval) std::string(s);
	tType = STR;
	return *this;
}

/*TODO: 🈚️
 */

Token Token::operator+(Token t)
{
	if (this->tType == STR || t.tType == STR) 
	{
		if (this->tType == STR && t.tType == STR)
		{
			this->sval += t.sval;
		}
	}
	else if (this->tType == DOUBLE || t.tType == DOUBLE)
	{
		*this = typeCast(*this, DOUBLE);
		t = typeCast(t, DOUBLE);
		this->dval += t.dval;
	}
	else if (this->tType == INT || t.tType == INT)
	{
		*this = typeCast(*this, INT);
		t = typeCast(t, INT);
		this->ival += t.ival;
	}
	else
	{
		this->cval += t.cval;
	}
	return *this;
}

Token Token::operator-(Token t)
{
	if (this->tType == STR || t.tType == STR)
	{
		throw (std::runtime_error("你为什么对string做减法？"));
	}
	else if (this->tType == DOUBLE || t.tType == DOUBLE)
	{
		*this = typeCast(*this, DOUBLE);
		t = typeCast(t, DOUBLE);
		this->dval -= t.dval;
	}
	else if (this->tType == INT || t.tType == INT)
	{
		*this = typeCast(*this, INT);
		t = typeCast(t, INT);
		this->ival -= t.ival;
	}
	else
	{
		this->cval -= t.cval;
	}
	return *this;
}

Token Token::operator*(Token t)
{
	if (this->tType == STR || t.tType == STR)
	{
		throw std::runtime_error("你为什么对string做乘法？");
	}
	else if (this->tType == DOUBLE || t.tType == DOUBLE)
	{
		*this = typeCast(*this, DOUBLE);
		t = typeCast(t, DOUBLE);
		this->dval *= t.dval;
	}
	else if (this->tType == INT || t.tType == INT)
	{
		*this = typeCast(*this, INT);
		t = typeCast(t, INT);
		this->ival *= t.ival;
	}
	else
	{
		this->cval *= t.cval;
	}
	return *this;
}

Token Token::operator/(Token t)
{
	if (this->tType == STR || t.tType == STR)
	{
		throw std::runtime_error("你为什么对string做除法？");
	}
	else if (this->tType == DOUBLE || t.tType == DOUBLE)
	{
		*this = typeCast(*this, DOUBLE);
		t = typeCast(t, DOUBLE);
		this->dval /= t.dval;
	}
	else if (this->tType == INT || t.tType == INT)
	{
		*this = typeCast(*this, INT);
		t = typeCast(t, INT);
		this->ival /= t.ival;
	}
	else
	{
		this->cval /= t.cval;
	}
	return *this;
}

inline char Token::CharGet()
{
	if (tType != CHAR)return '\0';
	else return cval;
}

inline Token typeCast(const Token& t, tokenType target)
{
	Token output;
	output.tType = target;
	switch (target)
	{
	case INT:
		if (t.tType == INT)output.ival = t.ival;
		else if (t.tType == DOUBLE) output.ival = t.dval;
		else if (t.tType == CHAR) output.ival = t.cval;
		else throw std::runtime_error("string无法转为int");
		break;
	case CHAR:
		if (t.tType == INT)output.cval = t.ival;
		else if (t.tType == DOUBLE) output.cval = t.dval;
		else if (t.tType == CHAR) output.cval = t.cval;
		else throw std::runtime_error("string无法转为char");
		break;
	case DOUBLE:
		if (t.tType == INT)output.dval = t.ival;
		else if (t.tType == DOUBLE) output.dval = t.dval;
		else if (t.tType == CHAR) output.dval = t.cval;
		else throw std::runtime_error("string无法转为double");
		break;
	case STR:
		output = "";
		if (t.tType == INT)output.sval = t.ival;
		else if (t.tType == DOUBLE) output.sval = t.dval;
		else if (t.tType == CHAR) output.sval = t.cval;
		else output.sval = t.ival;
		break;
	default:
		break;
	}
	return output;
}

std::ostream& operator<<(std::ostream& os,const Token& t)
{
	//os << t.tType << std::endl;
	switch (t.tType)
	{
	case INT:os << t.ival; break;
	case DOUBLE:os << t.dval; break;
	case CHAR:os << t.cval; break;
	case STR:os << t.sval; break;
	default:
		break;
	}
	return os;
}
#endif