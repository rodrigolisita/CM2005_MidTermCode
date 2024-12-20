// Candlestick.h
#pragma once

#include <string>
#include <map>

class Candlestick 
{
public:
    Candlestick(std::string _country, double _open, double _close, double _high, double _low);
    Candlestick() : country(""), open(0.0), close(0.0), high(0.0), low(0.0) {} // Default constructor

    std::string country;
    double open;
    double close;
    double high;
    double low;
};