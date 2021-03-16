#include "Scanner.hpp"
#include "Lexer.hpp"

namespace op4ht
{

Scanner::Scanner(std::string_view p_input) : m_source(p_input), m_start(0), m_current(0), m_line(1), m_column(0)
{
}

auto Scanner::ScanTokens() -> std::list<Token>
{
    std::list<Token> l_tokens;

    while (!IsAtEnd())
    {
        m_start = m_current;
        ScanCurrentToken();
    }

    AddToken(Token::Type::INPUT_STOP);

    return m_tokens;
}

bool Scanner::IsAtEnd() const
{
    return m_current >= m_source.length();
}

void Scanner::ScanCurrentToken()
{
    char l_ch = Advance();
    switch (l_ch)
    {
    case ':':
        AddToken(Match(':') ? Token::Type::COLONCOLON : Token::Type::COLON);
        break;
    case '(':
        AddToken(Token::Type::LPAREN);
        break;
    case ')':
        AddToken(Token::Type::RPAREN);
        break;
    case '{':
        AddToken(Token::Type::LCURL);
        break;
    case '}':
        AddToken(Token::Type::RCURL);
        break;
    case ';':
        AddToken(Token::Type::SEMICOLON);
        break;
    case '=':
        AddToken(Match('=') ? Token::Type::EQUALEQUAL : Token::Type::EQUAL);
        break;
    case '[':
        AddToken(Match('[') ? Token::Type::LBRACELBRACE : Token::Type::LBRACE);
        break;
    case ']':
        AddToken(Match(']') ? Token::Type::RBRACERBRACE : Token::Type::RBRACE);
        break;
    case ' ':
    case '\t':
    case '\r':
        break;
    case '\n':
        NewLine();
        m_line++;
        m_column = 0;
        break;
    default:
        if (IsAlpha(l_ch))
        {
            if (!Keyword() && !Identifier())
            {
                Lexer::Error(m_line, fmt::format("Unexpected Character: '{}'", l_ch));
            }
        }
        else if (IsDigit(l_ch) || l_ch == '-')
        {
            Number();
        }
        else
        {
            Lexer::Error(m_line, fmt::format("Unexpected Character: '{}'", l_ch));
        }
        break;
    }
}

char Scanner::Advance()
{
    m_current++;
    m_column++;
    return m_source[m_current - 1];
}

void Scanner::AddToken(Token::Type p_token)
{
    AddToken(p_token, std::nullopt);
}

void Scanner::AddToken(Token::Type p_token, std::optional<std::string> p_literal)
{
    std::string l_text = m_source.substr(m_start, m_current - m_start);
    m_tokens.emplace_back(Token(p_token, l_text, p_literal, m_line, m_column, m_start, m_current));
}

bool Scanner::Match(char p_expected)
{
    if (IsAtEnd())
        return false;
    if (m_source.at(m_current) != p_expected)
        return false;

    m_current++;
    return true;
}

char Scanner::Peek()
{
    if (IsAtEnd())
        return '\0';
    return m_source.at(m_current);
}

char Scanner::PeekNext(std::string_view::size_type n = 1)
{
    if (m_current + n >= m_source.length())
        return '\0';
    return m_source.at(m_current + n);
}

bool Scanner::IsAlpha(char p_char)
{
    return (p_char >= 'A' && p_char <= 'Z') || (p_char >= 'a' && p_char <= 'z');
}

bool Scanner::IsDigit(char p_char)
{
    return p_char >= '0' && p_char <= '9';
}

bool Scanner::IsAlphaNumeric(char p_char)
{
    return IsAlpha(p_char) || IsDigit(p_char) || p_char == '_';
}

bool Scanner::Keyword()
{
    while (IsAlphaNumeric(Peek()))
        Advance();

    std::string l_text = m_source.substr(m_start, m_current - m_start);
    const auto l_token = s_keywords.find(l_text);
    if (l_token != s_keywords.end())
    {
        AddToken(l_token->second);
        return true;
    }
    else
    {
        Rollback();
        return false;
    }
}

void Scanner::Rollback()
{
    m_current = m_start;
}

bool Scanner::Identifier()
{
    if (!IsNonDigit(Peek()))
        return false;

    while (IsNonDigit(Peek()) || IsDigit(Peek()))
        Advance();

    AddToken(Token::Type::IDENTIFIER);

    return true;
}

bool Scanner::IsNonDigit(char p_c)
{
    return (p_c >= 'a' && p_c <= 'z') || (p_c >= 'A' && p_c <= 'Z') || p_c == '_';
}

void Scanner::Number()
{
    while (IsDigit(Peek()))
    {
        Advance();
    }

    if (Peek() == '.' && IsDigit(PeekNext()))
    {
        Advance();
        while (IsDigit(Peek()))
        {
            Advance();
        }
    }

    AddToken(Token::Type::NUMBER, m_source.substr(m_start, m_current - m_start));
}

void Scanner::NewLine()
{
    AddToken(Token::Type::NEWLINE);
    // while (!IsAtEnd() && Peek() == '\n')
    //    Advance();
}
} // namespace op4ht