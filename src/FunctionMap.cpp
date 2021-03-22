#include "FunctionMap.h"
#include <string>
#include <sstream>

extern void println(std::string);

FunctionMap::FunctionMap()
    : _functionMap{}
{
}

FunctionCall FunctionMap::parseFunction(std::string funcStr)
{
    FunctionCall call;
    size_t openParen = funcStr.find('(');
    if (openParen == std::string::npos)
    {
        throw "No open parenthesis";
    }
    else if (openParen == 0)
    {
        throw "No function name found";
    }
    call.name = funcStr.substr(0, openParen);
    if (funcStr[funcStr.length() - 1] != ')')
    {
        throw "Missing closing parenthesis";
    }
    if (openParen + 1 < funcStr.length())
    {
        std::string argStr = funcStr.substr(openParen + 1, funcStr.length() - openParen - 2);
        size_t pos = 0;
        while ((pos = argStr.find(',')) != std::string::npos)
        {
            call.args.push_back(argStr.substr(0, pos));
            argStr.erase(0, pos + 1);
        }
        if (argStr.length() > 0)
        {
            call.args.push_back(argStr);
        }
    }
    return call;
}

CRGB FunctionMap::call(
    std::string funcStr,
    uint8_t x, uint8_t i, bool &reset)
{
    auto call = parseFunction(funcStr);
    if (_functionMap.count(call.name) == 0)
    {
        throw "No function named '" + call.name + "'";
    }
    auto entry = _functionMap[call.name];
    if (entry.expectedArgs != call.args.size())
    {
        std::stringstream ss;
        ss << "Function '" << call.name << "' expected " << (int)entry.expectedArgs << " string args but got " << call.args.size();
        throw ss.str();
    }
    return entry.func(x, i, reset, call.args);
}

void FunctionMap::registerFunc(FunctionMapEntry entry)
{
    _functionMap[entry.name] = entry;
}

std::string FunctionMap::getDocstring()
{
    std::stringstream ss;
    for (const auto &f : _functionMap)
    {
        ss << f.second.docstring << ";";
    }
    return ss.str();
}