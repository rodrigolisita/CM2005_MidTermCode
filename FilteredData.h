// FilteredData.h
#pragma once

#include <string>

class FilteredData
{
    public:
        FilteredData(
                    int _year,
                    std::string _country,
                    double _temperature,
                    int _startYear,
                    int _endYear,
                    double _euAvTemp = 0.0
                    );
    int year;
    std::string country;
    double temperature;
    int startYear; 
    int endYear;
    double euAvTemp;
};