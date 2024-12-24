#pragma once

#include<string>
#include<vector>
#include <functional>

#include "TemperatureData.h"
#include "Candlestick.h"
#include "FilteredData.h"
#include "PredictTemperatureData.h"

class Data
{
    public:
        Data() = default;
        void init();
        void computeStats();
    private:
        void loadData();
        std::vector<TemperatureData> data;
        static const int maxMenuOptions = 8; // Constant for maximum menu options
        void printMenu();
        void printAvailableCountries();
        std::string getCountry(const std::map<std::string, std::map<int, Candlestick>>& candlesticks);
        int getUserOption();
        void processUserOption(const int& userOption);
        void averageTemperatureForEachCountry();
        std::map<std::string, std::map<int, Candlestick>> candlesticks; // Declare candlesticks as a member variable

        void computeCandlesticks(); // Function to calculate candlesticks
        void printCandleStickData(const std::map<std::string, std::map<int, Candlestick>>& candlesticks);
        void printCandlestickChart(const std::map<std::string, std::map<int, Candlestick>>& candlesticks); 
        
        // Filtered Data
        std::vector<FilteredData> filteredData; // To store filtered data
        std::pair<int, int> getDateRangeFromUser(); // Store start and end year range for filtered data
        void filterByDateRange(const std::map<std::string, std::map<int, Candlestick>>& candlesticks); // Function to filter data by date range
        void filterData(int startYear, int endYear, const std::map<std::string, std::map<int, Candlestick>>& candlesticks);
        void printFilteredAverageTemperatureData(const std::map<std::string, std::map<int, Candlestick>>& candlesticks);
        void filterByCountry(const std::map<std::string, std::map<int, Candlestick>>& candlesticks); // Function to filter data by country
        void predictData(const std::map<std::string, std::map<int, Candlestick>>& candlesticks); // Function to predict temperatures
        void printPrediction(const std::vector<PredictTemperatureData>& tempPrediction, int columnWidth); // Print prediction values
        void printPredictionBarChart(const std::vector<PredictTemperatureData>& tempPrediction); // Print prediction in Bar chart
        void printBarChart(const std::vector<PredictTemperatureData>& data, //Helper function
                    const std::string& label, 
                    double yAxisHigh, double yAxisLow, double yAxisStep, 
                    int chartHeight, int maxYearWidth,
                    std::function<double(const PredictTemperatureData&)> valueGetter);

        std::map<int, double> yearlyAverageTemperatures;
        void computeYearlyAverageTemperatures();
        
        static double error (double x1, double x2);

        bool wantToContinue(std::string message);

};