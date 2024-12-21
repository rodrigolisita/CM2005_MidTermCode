#include <iostream>
#include <fstream>
#include <sstream> // For std::stringstream
#include <algorithm>
#include <numeric> // For std::accumulate 
#include <iomanip> // For std::setw and std::setfill

#include "Data.h"
#include "tokenize.h"

void Data::init()
{
    std::cout << "Hello World from data class" << std::endl;
    loadData();
    computeStats();
    computeCandlesticks();
    

}

void Data::loadData()
{
    std::ifstream csvFile{"weather_data_EU_1980-2019_temp_only.csv"};
    std::string line;
    std::vector<std::string> tokens;
    std::vector<std::string> tokensTime;

    int year;
    std::map<std::string, double> countryTemperatures;

    if(csvFile.is_open())
    {
        std::cout << "File open " << std::endl;
        while(std::getline(csvFile, line))
        {
            //std::cout << "Read line " << line << std::endl;
            tokens = tokenise(line, ',');
            tokensTime = tokenise(tokens[0], '-');
            
            if(tokens.size() != 29 ) // bad line
            {
                //std::cout << "Bad line " << std::endl;
                continue;
            }
            
            try{
                year = std::stoi(tokensTime[0]);
                countryTemperatures["AT"] = std::stod(tokens[1]); 
                countryTemperatures["BE"] = std::stod(tokens[2]); 
                countryTemperatures["BG"] = std::stod(tokens[3]); 
                countryTemperatures["CH"] = std::stod(tokens[4]);
                data.push_back(TemperatureData{
                                                tokens[0],
                                                year,
                                                countryTemperatures
                                              }
                );

            }catch(std::exception& e){
                std::cout << "Bad int! " << tokensTime[0] << std::endl;
                std::cout << "Bad double! " << tokens[1] << std::endl;
                std::cout << "Bad double! " << tokens[2] << std::endl;
                std::cout << "Bad double! " << tokens[3] << std::endl;
                std::cout << "Bad double! " << tokens[4] << std::endl;
                
                continue;
            }
        }
    
        csvFile.close();
    }
    else{
        std::cout << "Could not open file " << std::endl;
    }
     
}

void Data::computeStats()
{
    std::cout << "===========================" << std::endl;
    std::cout << "Statistics from the data." << std::endl;
    std::cout << "===========================" << std::endl;

    //printData();
    averageTemperatureForEachCountry();
}

void Data::printData()
{

    for (const auto& countryPair : data[0].countryTemperatures) 
    {
        for (const auto& line : data) 
        {
            std::cout << "Year: " << line.year << ". Country: "<< countryPair.first << std::endl;
        }
    }

//    for (const auto& line : data){
//        //std::cout << "Year: " << line.year << ". Country AT: " << line.AT << std::endl;
//        std::cout << "Year: " << line.year << ". AT: "<< line.AT << std::endl;
//        // Loop through each country in the enum
        
//    }
}

void Data::averageTemperatureForEachCountry() {
    std::cout << "===========================" << std::endl;
    std::cout << "Average Temperatures for Each Country" << std::endl;
    std::cout << "===========================" << std::endl;

// Calculate average temperature for each country in the map
    for (const auto& countryPair : data[0].countryTemperatures) { // Assuming all data points have the same countries
        double sum = 0;
        for (const auto& line : data) {
            sum += line.countryTemperatures.at(countryPair.first); 
        }

        double average = sum / data.size();
        std::cout << "Average temperature for country " 
                  << countryPair.first  // Print the country code (key in the map)
                  << ": " << average << std::endl;
    }
    
}

void Data::computeCandlesticks() {
    std::map<std::string, std::map<int, Candlestick>> candlesticks; // Country -> Year -> Candlestick
    
    // Get the list of countries from the first data point
    const std::map<std::string, double>& countries = data[0].countryTemperatures; 

    // Loop through countries
    for (const auto& countryPair : countries) {
        std::string country = countryPair.first;
        int currentYear = -1; // Initialize with an invalid year

        // Loop through years (from the data)
        for (const auto& line : data) {
            int year = line.year;
            double temperature = line.countryTemperatures.at(country);

            if(year != currentYear){
                //New year detected
                currentYear = year;

                //Initialize a new candlestick for this year
                candlesticks[country][year] = Candlestick(country, temperature, temperature, temperature, temperature);
            } else {
                // same year, update the close value
                candlesticks[country][year].close = temperature;

                // Update high and low
                candlesticks[country][year].high = std::max(candlesticks[country][year].high, temperature);
                candlesticks[country][year].low  = std::min(candlesticks[country][year].low,  temperature); 

            }
        }
    }

   printCandlestickChart(candlesticks);

    // Print the candlesticks for each country
//        for (const auto& countryPair : candlesticks) {
//            std::cout << "Country: " << countryPair.first << std::endl;
//            for (const auto& yearPair : countryPair.second) {
//                int year = yearPair.first;
//                const Candlestick& candle = yearPair.second;
//                std::cout << "  Year: " << year
//                        << ", Open: " << candle.open
//                        << ", High: " << candle.high
//                        << ", Low: " << candle.low
//                        << ", Close: " << candle.close << std::endl;
//            }
//        }

}

void Data::printCandlestickChart(const std::map<std::string, std::map<int, Candlestick>>& candlesticks) {

    if (candlesticks.empty() || candlesticks.begin()->second.empty()) {
        std::cout << "No candlestick data to display." << std::endl;
        return;
    }

    int chartHeight = 30;
    int barWidth = 5;

    // Select the first country ("AT")
    std::string selectedCountry = "AT"; 

    // Get the candlestick data for the selected country
    const auto& countryData = candlesticks.at(selectedCountry);

    // Calculate range for this country
    double countryMaxHigh = std::numeric_limits<double>::min();
    double countryMinLow = std::numeric_limits<double>::max();
    for (const auto& yearPair : countryData) {
        const Candlestick& candle = yearPair.second;
        countryMaxHigh = std::max(countryMaxHigh, candle.high);
        countryMinLow = std::min(countryMinLow, candle.low);
    }
    double countryRange = countryMaxHigh - countryMinLow;

    // Calculate the maximum width needed for the temperature values AND the year labels
    int maxColumnWidth = 0; 
    for (int i = chartHeight; i >= 0; --i) {
        double tempValue = i * countryRange / chartHeight + countryMinLow;
        std::stringstream ss;
        ss << tempValue;
        maxColumnWidth = std::max(maxColumnWidth, static_cast<int>(ss.str().length()));
    }
    for (const auto& yearPair : countryData) {
        int year = yearPair.first;
        std::stringstream ss;
        ss << year;
        maxColumnWidth = std::max(maxColumnWidth, static_cast<int>(ss.str().length()));
    }
    maxColumnWidth = std::max(maxColumnWidth, 4); // "Open" has 4 characters

    // Print the x-axis (years) with adjusted width and left alignment
    std::cout << std::setw(maxColumnWidth) << std::left << "" << " "; 
    for(const auto& yearPair : countryData){
        int year = yearPair.first;
        std::cout << std::setw(maxColumnWidth) << std::left << year << " ";
    }
    std::cout << std::endl;
    
    
    // Print the open values with adjusted width
    std::cout << std::setw(maxColumnWidth) <<std::left << "Open "; 
    for(const auto& yearPair : countryData){
        const Candlestick& candle = yearPair.second;
        std::cout << std::setw(maxColumnWidth) << candle.open << " ";
    }
    std::cout << std::endl;

    // Print the close values with adjusted width
    std::cout << std::setw(maxColumnWidth) <<std::left << "Close "; 
    for(const auto& yearPair : countryData){
        const Candlestick& candle = yearPair.second;
        std::cout << std::setw(maxColumnWidth) << candle.close << " ";
    }
    std::cout << std::endl;

    // Print the High values with adjusted width
    std::cout << std::setw(maxColumnWidth) <<std::left << "High "; 
    for(const auto& yearPair : countryData){
        const Candlestick& candle = yearPair.second;
        std::cout << std::setw(maxColumnWidth) << candle.high << " ";
    }
    std::cout << std::endl;

    // Print the Low values with adjusted width
    std::cout << std::setw(maxColumnWidth) <<std::left << "Low "; 
    for(const auto& yearPair : countryData){
        const Candlestick& candle = yearPair.second;
        std::cout << std::setw(maxColumnWidth) << candle.low << " ";
    }
    std::cout << std::endl;
    std::cout << "----------------------------------" << std::endl;

    // Add a margin to the high and low values for the y-axis range
    double margin = 0.1 * countryRange; // 10% margin
    double yAxisHigh = countryMaxHigh + margin;
    double yAxisLow = countryMinLow - margin;    
    
    // Calculate the step size for the y-axis values
    double yAxisStep = (yAxisHigh - yAxisLow) / chartHeight;

    // Print the y-axis values
    for (int i = 0; i <= chartHeight; ++i) {  // Iterate from 0 to chartHeight
        double tempValue = yAxisHigh - i * yAxisStep;
        double tempValueL = yAxisHigh - (i+1) * yAxisStep;
        std::cout << std::setw(maxColumnWidth) << tempValue << " |";
        for(const auto& yearPair : countryData){
            const Candlestick& candle = yearPair.second;

            double open = candle.open;  // Get the open value
            double close = candle.close; // Get the close value
            
            double realBodyHigh = std::max(candle.open,candle.close);
            double realBodyLow = std::min(candle.open,candle.close);
            double high = candle.high; // Get the high value
            double low = candle.low; // Get the low value
          
            if (open <= tempValue && open > tempValueL) {
            // Print "U" or "D" if open value falls within the current row's range
                std::cout << std::setw(maxColumnWidth) << std::left << (open > close ? "O" : "O") << " "; 
            } else if (close >= tempValueL && close < tempValue) {
            // Print "U" or "D" if close value falls within the current row's range
                std::cout << std::setw(maxColumnWidth) << std::left << (open > close ? "C" : "C") << " "; 
            //}
            //if(realBodyHigh <=tempValue && realBodyHigh>tempValueL){
            //    std::cout << std::setw(maxColumnWidth) << std::left << realBodyHigh << " ";
            //}else if (realBodyLow >=tempValueL && realBodyLow<tempValue)
            //{
            //    std::cout << std::setw(maxColumnWidth) << std::left << realBodyLow << " ";
            }else if (high >= tempValue && realBodyHigh < tempValue) { 
                // Print "|" if high value falls within the current row's range
                std::cout << std::setw(maxColumnWidth) << std::left << "|" << " "; 
            }else if (low <= tempValue && realBodyLow > tempValueL) { 
                // Print "|" if low value falls within the current row's range
               std::cout << std::setw(maxColumnWidth) << std::left << "|" << " "; 
            }else if (tempValue >= std::min(open, close) && tempValueL <= std::max(open, close)) {
            // Print "*" if the current row is between the open and close values
            std::cout << std::setw(maxColumnWidth) << std::left << "*" << " "; 
            }else {
                //Print blank space to maintain alignment
                std::cout << std::setw(maxColumnWidth) << std::left << "" << " "; 
            }
        }
        std::cout << std::endl;
    }
    // Print "Year" below the y-axis values
    std::cout << std::setw(maxColumnWidth) << std::left << "--------" << " |"; 
    for(const auto& yearPair : countryData){
        std::cout << std::setw(maxColumnWidth) << std::left << "---------";
    }
    std::cout << std::endl;
    
    // Print the x-axis (years) with adjusted width and left alignment
    std::cout << std::setw(maxColumnWidth) << std::left << "Year" << " |"; 
    for(const auto& yearPair : countryData){
        int year = yearPair.first;
        std::cout << std::setw(maxColumnWidth) << std::left << year << " ";
    }
    std::cout << std::endl;

   
}