#pragma once

#include <string>

class TemperatureData
{
    public:
        TemperatureData(std::string _timestamp,
        double _AT,
        double _BE,
        double _BG,
        double _CH
        );
    std::string timestamp;
    double AT;
    double BE;
    double BG;
    double CH;
};