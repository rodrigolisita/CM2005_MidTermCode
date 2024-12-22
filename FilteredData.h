// FilteredData.h
#pragma once

#include <string>

class FilteredData
{
    public:
        FilteredData(int _year,
        std::string _country,
        double _temperature
        );
    int year;
    std::string country;
    double temperature;
    
};