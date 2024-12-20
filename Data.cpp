#include <iostream>
#include <fstream>
#include <sstream> // For std::stringstream
#include <algorithm>
#include <unordered_map> 


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
            if(tokens.size() != 29 ) // bad line
            {
                //std::cout << "Bad line " << std::endl;
                continue;
            }
            
            try{
                AT = std::stod(tokens[1]);
                BE = std::stod(tokens[2]);
                BG = std::stod(tokens[3]);
                CH = std::stod(tokens[4]);

               
                data.push_back(TemperatureData{
                                                tokens[0],
                                                AT,
                                                BE,
                                                BG,
                                                CH
                                              }
                );

            }catch(std::exception& e){
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
}

void Data::printData()
{
    for (const auto& line : data){
        std::cout << line.timestamp << "AT: " << line.AT << std::endl;
    }
}
    


