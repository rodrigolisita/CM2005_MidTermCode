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
        void printMenu();
        void averageTemperatureForEachCountry();
//        std::map<int, std::vector<Candlestick>> yearlyCandlesticks; // Store candlesticks for each year
        std::map<std::string, std::map<int, Candlestick>> candlesticks; // Declare candlesticks as a member variable

        void computeCandlesticks(); // Function to calculate candlesticks
        void printCandleStickData(const std::map<std::string, std::map<int, Candlestick>>& candlesticks);
        void printCandlestickChart(const std::map<std::string, std::map<int, Candlestick>>& candlesticks); 

        

};