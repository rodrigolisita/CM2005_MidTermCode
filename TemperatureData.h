// TemperatureData.h
#pragma once

#include <string>
#include <vector>
#include <map>

class TemperatureData
{
    public:
        TemperatureData(std::string _timestamp,
        int _year,
        std::map<std::string, double> _countryTemperatures
        );
    std::string timestamp;
    int year;
    std::map<std::string, double> countryTemperatures;
};