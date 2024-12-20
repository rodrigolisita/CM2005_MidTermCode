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

}

void Data::loadData()
{
    std::ifstream csvFile{"weather_data_EU_1980-2019_temp_only.csv"};
    std::string line;
    std::vector<std::string> tokens;
    std::vector<std::string> tokensTime;

    int year;
    //double AT;
    //double BE;
    //double BG;
    //double CH;
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
                //AT = std::stod(tokens[1]);
                //BE = std::stod(tokens[2]);
                //BG = std::stod(tokens[3]);
                //CH = std::stod(tokens[4]);

               
                data.push_back(TemperatureData{
                                                tokens[0],
                                                year,
                                                countryTemperatures 
                                                //AT,
                                                //BE,
                                                //BG,
                                                //CH
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