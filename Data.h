#pragma once

#include<string>
#include<vector>

#include "TemperatureData.h"
#include "Candlestick.h"

class Data
{
    public:
        
        Data() = default;
        void init();
         void computeStats();
    private:
        void loadData();
        std::vector<TemperatureData> data;
        void printData();
        void averageTemperatureForEachCountry();
        std::map<int, std::vector<Candlestick>> yearlyCandlesticks; // Store candlesticks for each year
        void computeCandlesticks(); // Function to calculate candlesticks

};