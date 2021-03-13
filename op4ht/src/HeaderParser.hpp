#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <exception>

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
    struct ComponentOption
    {
    };

public:
    HeaderParser();

private:
    void ParseHeaderFile(std::string const &path);
    std::vector<ComponentOption> GetComponentOptions(std::string_view start);
    std::string GetComponentName(std::string_view start);
    std::string GenerateComponentBody(std::string componentName, std::vector<ComponentOption> componentOptions);
    void AddComponent(std::string const &path, std::string body);

    static std::ifstream OpenFile(std::string const &path);
    static std::string ReadFile(std::string const &path);
};

}
