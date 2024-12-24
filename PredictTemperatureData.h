// PredictTemperatureData.h
#pragma once

#include <string>

class PredictTemperatureData {
public:
    PredictTemperatureData(int _year, 
                            std::string _country, 
                            double _temperature, 
                            int _startYear, 
                            int _endYear, 
                            double _nextTemperature,
                            double _backwardDifference, 
                            double _euDifference, 
                            double _taylorPrediction,
                            double _averageALL = 0.0);

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