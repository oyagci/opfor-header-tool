#include <fmt/format.h>
#include <cxxopts.hpp>
#include "Lexer.hpp"

int main(int ac, char *av[])
{
    cxxopts::Options options("op4ht", "OPFOR engine header tool");

    options.add_options()
        ("i,input", "Input file", cxxopts::value<std::string>())
        ("o,output", "Output file", cxxopts::value<std::string>());
    const auto result = options.parse(ac, av);
    const auto &inputFile = result["input"].as<std::string>();

    op4ht::Lexer::RunFile(inputFile);

    for (const auto &t: op4ht::Lexer::GetTokens())
    {
    	fmt::print("{}\n", t.ToString());
    }

    return 0;
}
