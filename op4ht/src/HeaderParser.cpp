#include "HeaderParser.hpp"
#include <fmt/format.h>
#include <sstream>
#include <unordered_map>

namespace op4ht
{

HeaderParser::HeaderParser() = default;

std::ifstream HeaderParser::OpenFile(std::string const &path)
{
    std::ifstream file(path, std::ios::in);

    if (!file.is_open())
    {
        throw std::runtime_error(fmt::format("Failed to open input file {}", path));
    }

    return file;
}

std::string HeaderParser::ReadFile(std::string const &path)
{
    const auto file = OpenFile(path);
    std::stringstream sstr;

    sstr << file.rdbuf();
    return sstr.str();
}

void HeaderParser::ParseHeaderFile(std::string const &path)
{
    const auto content = ReadFile(path);
    std::string::size_type idx;

    do
    {
        idx = content.find("OP4_COMPONENT");
        if (idx != std::string::npos)
        {
            const auto componentOptions = GetComponentOptions(std::string_view(content).substr(idx));
            const auto componentName = GetComponentName(std::string_view(content).substr(idx));
            const std::string componentBody = GenerateComponentBody(componentName, componentOptions);
            AddComponent(path, componentBody);
        }
    } while (idx != std::string::npos);
}

std::vector<HeaderParser::ComponentOption> HeaderParser::GetComponentOptions(std::string_view start)
{
    auto idx = start.find("OP4_COMPONENT");

    if (idx != 0)
    {
        throw std::runtime_error("Not a valid component keyword");
    }

    while (idx < start.size() && std::isspace(start[idx]))
    {
        idx++;
    }

    if (start[idx] != '(')
    {
        throw SyntaxError("Expected '(' after OP4_COMPONENT");
    }

    idx++;

    if (start[idx] != ')')
    {
        throw SyntaxError("Expected ')' after OP4_COMPONENT");
    }
}

std::string HeaderParser::GetComponentName(std::string_view start)
{
    auto idx = start.find(')');

    while (idx < start.size() && std::isspace(start[idx]))
    {
        idx++;
    }

    return "";
}

std::string HeaderParser::GenerateComponentBody(std::string componentName,
    std::vector<ComponentOption> componentOptions)
{
    return "";
}

void HeaderParser::AddComponent(std::string const &path, std::string body)
{
}

}
