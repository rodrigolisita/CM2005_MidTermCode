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
    double AT;
    double BE;
    double BG;
    double CH;

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
                AT = std::stod(tokens[1]);
                BE = std::stod(tokens[2]);
                BG = std::stod(tokens[3]);
                CH = std::stod(tokens[4]);

               
                data.push_back(TemperatureData{
                                                tokens[0],
                                                year,
                                                AT,
                                                BE,
                                                BG,
                                                CH
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

    printData();
    averageTemperatureForEachCountry();
}

void Data::printData()
{

    int i; 
    for (i = static_cast<int>(TemperatureData::Country::AT); i <=static_cast<int>(TemperatureData::Country::CH); i++){
        std::cout << i << " ";
        switch (static_cast<TemperatureData::Country>(i)) {
            case TemperatureData::Country::AT: std::cout << "AT"; break;
            case TemperatureData::Country::BE: std::cout << "BE"; break;
            case TemperatureData::Country::BG: std::cout << "BG"; break;
            case TemperatureData::Country::CH: std::cout << "CH"; break;
            // Add more cases for other countries as needed
        }

        std::cout << std::endl;
    }

//    for (const auto& line : data){
//        //std::cout << "Year: " << line.year << ". Country AT: " << line.AT << std::endl;
//        std::cout << "Year: " << line.year << ". AT: "<< line.AT << std::endl;
//        // Loop through each country in the enum
        
//    }
}

void Data::averageTemperatureForEachCountry()
{
    std::cout << "===========================" << std::endl;
    std::cout << "Average Temperatures for Each Country" << std::endl;
    std::cout << "===========================" << std::endl;


    
    
    
    
    
    

    
    
 
 
    
    
}    


