#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <exception>
#include <variant>
#include "Lexer.hpp"

namespace op4ht
{

class SyntaxError : public std::exception
{
private:
    std::string _errstr;

  public:
    SyntaxError(std::string errstr) : _errstr(std::move(errstr))
    {
    }

    [[nodiscard]] const char *what() const noexcept override
    {
        return _errstr.c_str();
    }
};

class HeaderParser
{
private:
	struct Property
	{
		enum class Type
		{
			INT,
			FLOAT,
			DOUBLE,
			STRING,
		};

		Type type;
		std::string name;
		void *value;
	};

	struct Method
	{
		enum class RetType
		{
			INT,
			FLOAT,
			DOUBLE,
			STRING,
		};

		RetType returnType;
		std::string name;
	};

    struct ComponentDescriptor
    {
		enum class Type
		{
			CLASS,
			STRUCT,
		};
		std::vector<Property> properties;
		std::vector<Method> methods;
    };

public:
	HeaderParser() = delete;
	HeaderParser(std::string const &src, const std::list<Token> &tokens);

	void ParseHeaderFile();

private:
	void ParseComponent();

	void Advance();
	bool IsAtEnd() const;
	const Token &Peek() const;
	const Token &PeekNext() const;

	std::string m_source;
	std::list<Token> m_tokens;
};

}
