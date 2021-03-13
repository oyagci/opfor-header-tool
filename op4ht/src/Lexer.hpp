#pragma once
#include "Scanner.hpp"
#include <fstream>
#include <sstream>
#include <fmt/format.h>

namespace op4ht
{

class Lexer
{
  public:
    static void RunFile(std::string_view p_path)
    {
        std::fstream l_file;
        l_file.open(std::string(p_path), std::ios::in);

        std::ostringstream l_stringStream;
        l_stringStream << l_file.rdbuf();

        Run(l_stringStream.str());
    }

    static void Run(std::string_view p_source)
    {
        Scanner l_scanner(p_source);
        std::list<Token> l_tokens = l_scanner.ScanTokens();

        s_tokens = l_tokens;
    }

    static void Error(int p_line, std::string_view p_message)
    {
        Report(p_line, "", p_message);
    }

    static void Error(Token p_token, std::string p_message)
    {
        if (p_token.m_type == TokenType::INPUT_STOP)
        {
            Report(p_token.m_line, "at end", p_message);
        }
        else
        {
            Report(p_token.m_line, " at '" + p_token.m_lexeme + "'", p_message);
        }
    }

    static void Report(int p_line, std::string_view p_where, std::string_view p_message)
    {
        fmt::print("[line {}] Error {}: {}\n", p_line, p_where, p_message);
        s_hadError = true;
    }

    static bool HadError()
    {
        return s_hadError;
    }
    static std::list<Token> const &GetTokens()
    {
        return s_tokens;
    }

  private:
    inline static bool s_hadError = false;
    inline static std::list<Token> s_tokens;
};

} // namespace op4ht
