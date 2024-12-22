#pragma once

#include<string>
#include<vector>

#include "TemperatureData.h"
#include "Candlestick.h"
#include "FilteredData.h"

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
        static const int maxMenuOptions = 8; // Constant for maximum menu options
        int getUserOption();
        void processUserOption(const int& userOption);
        void averageTemperatureForEachCountry();
        std::map<std::string, std::map<int, Candlestick>> candlesticks; // Declare candlesticks as a member variable

        void computeCandlesticks(); // Function to calculate candlesticks
        void printCandleStickData(const std::map<std::string, std::map<int, Candlestick>>& candlesticks);
        void printCandlestickChart(const std::map<std::string, std::map<int, Candlestick>>& candlesticks); 
        
        // Filtered Data
        std::vector<FilteredData> filteredData; // To store filtered data
        void filterByDateRange(); // Function to filter data by date range
        void printFilteredData(const std::map<std::string, std::map<int, std::vector<double>>>& countryYearTemps,
                            const std::map<int, double>& yearlyAvgTemps);
        void filterByCountry(); // Function to filter data by country


        // ANSI color codes (you can add more as needed)
        const std::string colorRed = "\033[31m";
        const std::string colorGreen = "\033[32m";
        const std::string colorBlue = "\033[34m";
        const std::string colorReset = "\033[0m"; 

};