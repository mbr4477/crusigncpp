#ifndef _LOGGER_H_
#define _LOGGER_H_
#include <string>
#include <Arduino.h>

void println(std::string msg)
{
    Serial.println(msg.c_str());
}

#endif