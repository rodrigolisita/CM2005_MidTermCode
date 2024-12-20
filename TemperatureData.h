// TemperatureData.h
#pragma once

#include <string>
#include <vector>
#include <map>

class TemperatureData
{
    public:
        enum class Country { AT, BE, BG, CH };
        TemperatureData(std::string _timestamp,
        int _year,
        std::map<std::string, double> _countryTemperatures
        //double _AT,
        //double _BE,
        //double _BG,
        //double _CH
        );
    std::string timestamp;
    int year;
    std::map<std::string, double> countryTemperatures;
    //double AT;
    //double BE;
    //double BG;
    //double CH;
};