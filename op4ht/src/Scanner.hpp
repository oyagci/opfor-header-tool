#pragma once

#include <fmt/format.h>
#include <optional>
#include <string>
#include <unordered_map>

namespace op4ht
{

#define Q(x) #x
#define QUOTE(x) Q(x)
#define CASE_TOKEN(x)                                                                                                  \
    case x:                                                                                                            \
        return QUOTE(x);                                                                                               \
        break;

struct Token
{
    enum Type
    {
        NONE = 0,

        COLON,
        COLONCOLON,
        SEMICOLON,
        NEWLINE,

        LPAREN,
        RPAREN,

        LCURL,
        RCURL,

        LBRACE,
        RBRACE,
        LBRACELBRACE,
        RBRACERBRACE,

        CLASS,
        STRUCT,
        ENUM,

        USING,

        NUMBER,
        FLOAT_NUMBER,

        IDENTIFIER,

        EQUAL,
        EQUALEQUAL,

        INT,
        INT8,
        INT16,
        INT32,
        FLOAT,
        DOUBLE,

        OP4_CLASS,
        OP4_COMPONENT,
        OP4_PROPERTY,
        OP4_FUNCTION,

        INPUT_STOP,
    };

    Type m_type;
    std::string m_lexeme;
    std::optional<std::string> m_literal;
    int m_line;
    int m_col;
    std::string::size_type m_start;
    std::string::size_type m_end;

    Token(Type p_type, std::string p_lexeme, std::optional<std::string> p_literal, int p_line, int p_col, int p_start,
          int p_end)
        : m_type(p_type), m_lexeme(p_lexeme), m_literal(p_literal), m_line(p_line), m_col(p_col), m_start(p_start),
          m_end(p_end)
    {
    }

    [[nodiscard]] std::string TypeToString() const
    {
        switch (m_type)
        {
            CASE_TOKEN(COLON)
            CASE_TOKEN(COLONCOLON)
            CASE_TOKEN(SEMICOLON)
            CASE_TOKEN(NEWLINE)
            CASE_TOKEN(LPAREN)
            CASE_TOKEN(RPAREN)
            CASE_TOKEN(LCURL)
            CASE_TOKEN(RCURL)
            CASE_TOKEN(LBRACE)
            CASE_TOKEN(RBRACE)
            CASE_TOKEN(LBRACELBRACE)
            CASE_TOKEN(RBRACERBRACE)
            CASE_TOKEN(CLASS)
            CASE_TOKEN(STRUCT)
            CASE_TOKEN(ENUM)
            CASE_TOKEN(USING)
            CASE_TOKEN(NUMBER)
            CASE_TOKEN(FLOAT_NUMBER)
            CASE_TOKEN(IDENTIFIER)
            CASE_TOKEN(EQUAL)
            CASE_TOKEN(EQUALEQUAL)
            CASE_TOKEN(INT)
            CASE_TOKEN(INT8)
            CASE_TOKEN(INT16)
            CASE_TOKEN(INT32)
            CASE_TOKEN(FLOAT)
            CASE_TOKEN(DOUBLE)
            CASE_TOKEN(OP4_PROPERTY)
            CASE_TOKEN(OP4_CLASS)
            CASE_TOKEN(OP4_FUNCTION)
            CASE_TOKEN(OP4_COMPONENT)
            CASE_TOKEN(INPUT_STOP)
        default:
            return "(missing)";
        }
    }

    [[nodiscard]] std::string ToString() const
    {
        return fmt::format(
            "{{ m_type: {:15}, m_lexeme: '{}', m_literal: '{}', m_line: {}, m_column: {}, m_start: {}, m_end: {} }}",
            "'" + TypeToString() + "'", (m_lexeme == "\n" ? "\\n" : m_lexeme), m_literal.value_or("(null)"),
            std::to_string(m_line), std::to_string(m_col), std::to_string(m_start), std::to_string(m_end));
    }
};

class Scanner
{
  public:
    Scanner(std::string_view p_input);
    std::list<Token> ScanTokens();

  private:
    [[nodiscard]] bool IsAtEnd() const;
    void ScanCurrentToken();
    char Advance();
    void AddToken(Token::Type);
    void AddToken(Token::Type, std::optional<std::string>);
    bool Match(char p_expected);
    [[nodiscard]] char Peek();
    [[nodiscard]] char PeekNext(std::string_view::size_type n);
    [[nodiscard]] static bool IsAlpha(char);
    [[nodiscard]] static bool IsDigit(char);
    [[nodiscard]] static bool IsAlphaNumeric(char);
    bool Keyword();
    void Number();
    void NewLine();
    bool Identifier();

    bool IsNonDigit(char);

    void Rollback();

  private:
    std::string m_source;
    std::list<Token> m_tokens;

    std::string_view::size_type m_start;
    std::string_view::size_type m_current;
    std::string_view::size_type m_line;
    std::string_view::size_type m_column;

    inline static const std::unordered_map<std::string, Token::Type> s_keywords = {
        {"int", Token::Type::INT},
        {"int8", Token::Type::INT8},
        {"int16", Token::Type::INT16},
        {"int32", Token::Type::INT32},
        {"float", Token::Type::FLOAT},
        {"double", Token::Type::DOUBLE},
        {"class", Token::Type::CLASS},
        {"struct", Token::Type::STRUCT},
        {"enum", Token::Type::ENUM},
        {"using", Token::Type::USING},
        {"OP4_CLASS", Token::Type::OP4_CLASS},
        {"OP4_COMPONENT", Token::Type::OP4_COMPONENT},
        {"OP4_PROPERTY", Token::Type::OP4_PROPERTY},
        {"OP4_FUNCTION", Token::Type::OP4_FUNCTION},
    };
};

} // namespace op4ht
