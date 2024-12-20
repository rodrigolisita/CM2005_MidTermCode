#include <iostream>
#include <fstream>
#include <sstream> // For std::stringstream
#include <algorithm>
#include <unordered_map>
#include <numeric> // For std::accumulate 

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


// Print the candlesticks for each country
    for (const auto& countryPair : candlesticks) {
        std::cout << "Country: " << countryPair.first << std::endl;

        for (const auto& yearPair : countryPair.second) {
            int year = yearPair.first;
            const Candlestick& candle = yearPair.second;

            std::cout << "  Year: " << year
                      << ", Open: " << candle.open
                      << ", High: " << candle.high
                      << ", Low: " << candle.low
                      << ", Close: " << candle.close << std::endl;
        }
    }

}