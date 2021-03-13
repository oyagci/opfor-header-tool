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

enum TokenType
{
    NONE = 0,

    SEMICOLON,
    NEWLINE,

    LPAREN,
    RPAREN,

    NUMBER,
    FLOAT_NUMBER,

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

struct Token
{
    TokenType m_type;
    std::string m_lexeme;
    std::optional<std::string> m_literal;
    int m_line;
    int m_col;

    Token(TokenType p_type, std::string p_lexeme, std::optional<std::string> p_literal, int p_line, int p_col)
        : m_type(p_type), m_lexeme(p_lexeme), m_literal(p_literal), m_line(p_line), m_col(p_col)
    {
    }

    [[nodiscard]] std::string TokenTypeToString() const
    {
        switch (m_type)
        {
            CASE_TOKEN(SEMICOLON)
            CASE_TOKEN(NEWLINE)
            CASE_TOKEN(LPAREN)
            CASE_TOKEN(RPAREN)
            CASE_TOKEN(NUMBER)
            CASE_TOKEN(FLOAT_NUMBER)
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
        return fmt::format("{{ m_type: '{}', m_lexeme: '{}', m_literal: '{}', m_line: {}, m_column: {} }}",
                           TokenTypeToString(), (m_lexeme == "\n" ? "\\n" : m_lexeme), m_literal.value_or("(null)"),
                           std::to_string(m_line), std::to_string(m_col));
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
    void AddToken(TokenType);
    void AddToken(TokenType, std::optional<std::string>);
    bool Match(char p_expected);
    [[nodiscard]] char Peek();
    [[nodiscard]] char PeekNext(std::string_view::size_type n);
    [[nodiscard]] static bool IsAlpha(char);
    [[nodiscard]] static bool IsDigit(char);
    [[nodiscard]] static bool IsAlphaNumeric(char);
    void Identifier();
    void Number();
    void NewLine();

  private:
    std::string m_source;
    std::list<Token> m_tokens;

    std::string_view::size_type m_start;
    std::string_view::size_type m_current;
    std::string_view::size_type m_line;
    std::string_view::size_type m_column;

    inline static const std::unordered_map<std::string, TokenType> s_keywords = {{"int8", INT8},
                                                                                 {"int16", INT16},
                                                                                 {"int32", INT32},
                                                                                 {"float", FLOAT},
                                                                                 {"double", DOUBLE},
                                                                                 {"OP4_CLASS", OP4_CLASS},
                                                                                 {"OP4_COMPONENT", OP4_COMPONENT},
                                                                                 {"OP4_PROPERTY", OP4_PROPERTY},
                                                                                 {"OP4_FUNCTION", OP4_FUNCTION}};
};

} // namespace op4ht
