#ifndef _FUNCTIONPARSE_H_
#define _FUNCTIONPARSE_H_
#include <stdint.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <exception>
#include "FastLED.h"

typedef struct function_map_entry
{
    std::string name;
    std::string docstring;
    uint8_t expectedArgs;
    std::function<CRGB(uint8_t, uint8_t, bool &, std::vector<std::string>)> func;
} FunctionMapEntry;

typedef struct function_call
{
    std::string name;
    std::vector<std::string> args;
} FunctionCall;

class FunctionMap
{
public:
    FunctionMap();
    void registerFunc(FunctionMapEntry entry);
    std::string getDocstring();
    CRGB call(
        std::string funcStr,
        uint8_t x,
        uint8_t i,
        bool &resetCounter);

private:
    std::unordered_map<std::string, FunctionMapEntry> _functionMap;
    FunctionCall parseFunction(std::string funcStr);
};

#endif
