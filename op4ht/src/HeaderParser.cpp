#include "HeaderParser.hpp"
#include <fmt/format.h>
#include <sstream>
#include <unordered_map>

namespace op4ht
{

HeaderParser::HeaderParser(std::string const &src, const std::list<Token> &tokens) : m_tokens(tokens), m_source(src)
{
}

void HeaderParser::ParseHeaderFile()
{
    while (!IsAtEnd())
    {
        auto &t = Peek();

        switch (t.m_type)
        {
        case Token::Type::OP4_COMPONENT:
            ParseComponent();
            break;
        default:
            Advance();
            break;
        }
    }
}

void HeaderParser::ParseComponent()
{
    Token token = Peek();

    if (token.m_type != Token::Type::OP4_COMPONENT)
    {
        throw std::runtime_error("Token.m_type != OP4_COMPONENT");
    }

    Advance();
    token = Peek();
    if (token.m_type != Token::Type::LPAREN)
    {
        throw std::runtime_error("Expected '(' after component identifier");
    }

    Token lparen = token;

    while (!IsAtEnd() && token.m_type != Token::Type::RPAREN)
    {
        Advance();
        token = Peek();
    }
    if (IsAtEnd())
    {
        throw std::runtime_error("Expected ')' after '('");
    }

    Token rparen = token;

    while (!IsAtEnd() && token.m_type != Token::Type::LCURL)
    {
        Advance();
        token = Peek();
    }

    if (IsAtEnd())
    {
        throw std::runtime_error("Expected class head after component designator");
    }

    Token delimiter = token;

    std::string const classHead = m_source.substr(rparen.m_end, delimiter.m_start - rparen.m_end);
}

const Token &HeaderParser::Peek() const
{
    return m_tokens.front();
}

const Token &HeaderParser::PeekNext() const
{
    auto begin = m_tokens.begin();

    std::advance(begin, 1);

    return *begin;
}

void HeaderParser::Advance()
{
    m_tokens.pop_front();
}

bool HeaderParser::IsAtEnd() const
{
    return m_tokens.empty();
}

} // namespace op4ht
