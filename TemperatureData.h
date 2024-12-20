#pragma once

#include <string>

class TemperatureData
{
    public:
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