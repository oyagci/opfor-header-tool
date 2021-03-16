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

    AddToken(TokenType::INPUT_STOP);

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
		AddToken(Match(':') ? DOUBLE_COLON : COLON);
		break;
	case '(':
        AddToken(LPAREN);
        break;
    case ')':
        AddToken(RPAREN);
        break;
	case '{':
		AddToken(LCURL);
		break;
	case '}':
		AddToken(RCURL);
		break;
    case ';':
		AddToken(SEMICOLON);
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
            Keyword();
        }
        else if (IsDigit(l_ch) || l_ch == '-')
        {
            Number();
        }
        else
        {
            // ignore input

            // Lexer::Error(m_line, fmt::format("Unexpected Character: '{}'", l_ch));
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

void Scanner::AddToken(TokenType p_tokenType)
{
    AddToken(p_tokenType, std::nullopt);
}

void Scanner::AddToken(TokenType p_tokenType, std::optional<std::string> p_literal)
{
    std::string l_text = m_source.substr(m_start, m_current - m_start);
    m_tokens.emplace_back(Token(p_tokenType, l_text, p_literal, m_line, m_column, m_start, m_current));
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

void Scanner::Keyword()
{
    while (IsAlphaNumeric(Peek()))
        Advance();

    std::string l_text = m_source.substr(m_start, m_current - m_start);
    const auto l_tokenType = s_keywords.find(l_text);
    if (l_tokenType != s_keywords.end())
    {
        AddToken(l_tokenType->second);
    }
    else
    {
		AddToken(IDENTIFIER);
        // Lexer::Error(m_line, fmt::format("Unexpected Keyword: '{}'", l_text));
    }
}

void Scanner::Identifier()
{
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

    AddToken(NUMBER, m_source.substr(m_start, m_current - m_start));
}

void Scanner::NewLine()
{
    AddToken(NEWLINE);
    //while (!IsAtEnd() && Peek() == '\n')
    //    Advance();
}
} // namespace op4ht