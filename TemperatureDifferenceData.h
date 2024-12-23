// TemperatureDifferenceData.h
#pragma once
#include <string>

class TemperatureDifferenceData {
  public:
    TemperatureDifferenceData(int _year, std::string _country, double _temperature, int _startYear, int _endYear, double _backwardDifference, double _euDifference, double _nextTemperature)
        : year(_year), country(_country), temperature(_temperature), startYear(_startYear), endYear(_endYear), backwardDifference(_backwardDifference) , euDifference(_euDifference), nextTemperature(_nextTemperature)
    {}

    int year;
    std::string country;
    double temperature;
    int startYear;
    int endYear;
    double backwardDifference; 
    double euDifference; 
    double nextTemperature;
};