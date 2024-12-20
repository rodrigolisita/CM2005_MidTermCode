#pragma once

#include <string>
#include <vector>

class TemperatureData
{
    public:
        enum class Country { AT, BE, BG, CH };
        TemperatureData(std::string _timestamp,
        int _year,
        double _AT,
        double _BE,
        double _BG,
        double _CH
        );
    std::string timestamp;
    int year;
    double AT;
    double BE;
    double BG;
    double CH;
};