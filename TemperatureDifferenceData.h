// TemperatureDifferenceData.h
#pragma once
#include <string>

class TemperatureDifferenceData {
  public:
    TemperatureDifferenceData(int _year, 
                              std::string _country, 
                              double _temperature, 
                              int _startYear, 
                              int _endYear, 
                              double _nextTemperature,
                              double _backwardDifference, 
                              double _euDifference, 
                              double _taylorPrediction,
                              double _averageALL = 0.0
                              )
        : year(_year), 
        country(_country), 
        temperature(_temperature), 
        startYear(_startYear), 
        endYear(_endYear), 
        nextTemperature(_nextTemperature),
        backwardDifference(_backwardDifference), 
        euDifference(_euDifference), 
        taylorPrediction(_taylorPrediction),
        averageALL(_averageALL)
    {}

    int year;
    std::string country;
    double temperature;
    int startYear;
    int endYear;
    double backwardDifference; 
    double euDifference; 
    double nextTemperature;
    double taylorPrediction;
    double averageALL;
};