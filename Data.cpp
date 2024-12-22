#include <iostream>
#include <fstream>
#include <sstream> // For std::stringstream
#include <algorithm>
#include <numeric> // For std::accumulate 
#include <iomanip> // For std::setw and std::setfill

#include "Data.h"
#include "tokenize.h"

#include <map>      // Include the map header
#include <functional>

void Data::init()
{
    std::cout << "Hello from Weather data EU 1980-2019" << std::endl;
    loadData();
    computeCandlesticks();
    //printMenu();
    
    int input;
    while(true)
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    }
    
   
}

void Data::printMenu() {
    std::cout << "\nWeather data MENU. Choose option" << std::endl;
    std::cout << "1: Print Candlestick data" << std::endl;
    std::cout << "2: Print Candlestick Chart" << std::endl;
    std::cout << "3: Compute data statistics" << std::endl;
    std::cout << "4: Print Available Countries" << std::endl;
    std::cout << "5: Exit" << std::endl;
}


int Data::getUserOption() {
    int userOption;

    std::cout << "Enter your choice: ";
    std::cin >> userOption;

    // Input validation 
    while (std::cin.fail() || userOption < 1 || userOption > maxMenuOptions) { 
        std::cout << "Invalid input. Please enter a number between 1 and 5: ";
        std::cin.clear();  // Clear error flags
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
        std::cin >> userOption;
    }

    return userOption;
}

/** Process user option */ 
void Data::processUserOption(const int& userOption) {
    switch (userOption) {
        case 1:
            printCandleStickData(candlesticks);
            break;
        case 2:
            printCandlestickChart(candlesticks); // Assuming candlesticks is a member variable
            break;
        case 3:
            computeStats();
            break;
        case 4:
            printData();
            break;
        case 5:
            std::cout << "Exiting program." << std::endl;
            std::exit(0); // Terminate the program
            break;
        default:
            std::cout << "Invalid option selected." << std::endl;
    }
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
        std::cout << "Opening file ... " << std::endl;
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

    std::cout << "\nAvailable countries:" << std::endl;

                // Get the list of countries from the first data point
                const std::map<std::string, double>& countries = data[0].countryTemperatures;

                for (const auto& countryPair : countries) {
                    std::cout << "- " << countryPair.first << std::endl;
                }

//    for (const auto& countryPair : data[0].countryTemperatures) 
//    {
//        for (const auto& line : data) 
//        {
//            std::cout << "Year: " << line.year << ". Country: "<< countryPair.first << std::endl;
//        }
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
    
    //std::map<std::string, std::map<int, Candlestick>> candlesticks; // Country -> Year -> Candlestick
    
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

   //printCandlestickChart(candlesticks);

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

std::string getCountry(const std::map<std::string, std::map<int, Candlestick>>& candlesticks){
     // Print the list of available countries
    std::cout << "\nAvailable countries:" << std::endl;
    int countryIndex = 1; // Start indexing from 1 for user-friendliness
    std::map<int, std::string> countryMap; // To map user input to country codes
    for (const auto& countryPair : candlesticks) {
        std::cout << countryIndex << ": " << countryPair.first << std::endl;
        countryMap[countryIndex] = countryPair.first;
        countryIndex++;
    }

    // Get country selection from user
    int selectedIndex;
    std::cout << "Enter the number for your selected country: ";
    std::cin >> selectedIndex;

    // Input validation (check if the input is a number and within the valid range)
    while (std::cin.fail() || selectedIndex < 1 || selectedIndex >= countryIndex) {
        std::cout << "Invalid input. Please enter a number from the list: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> selectedIndex;
    }
    std::cout << "Selected country: " << countryMap.at(selectedIndex) << std::endl;
    return countryMap.at(selectedIndex);

}

void Data::printCandleStickData(const std::map<std::string, std::map<int, Candlestick>>& candlesticks){
    if (candlesticks.empty() || candlesticks.begin()->second.empty()) {
        std::cout << "No candlestick data to display." << std::endl;
        return;
    }

    // Get the selectedCountry variable
    std::string selectedCountry = getCountry(candlesticks);
    const auto& countryData = candlesticks.at(selectedCountry);

    std::cout << "***************************** " << std::endl; 
    std::cout << "Candlestick data for Country: " << selectedCountry << std::endl; 
    std::cout << "***************************** " << std::endl; 

    int maxColumnWidth = 8;

    // year in rows
    //std::cout << std::setw(maxColumnWidth) << std::left << "Year" << " Open " << "Close " << "High " << "Low" << std::endl; 
    std::cout << std::setw(maxColumnWidth) << std::left << "Year" << 
        std::setw(maxColumnWidth) << std::left << "Open  " <<
        std::setw(maxColumnWidth) << std::left << "Close  " <<
        std::setw(maxColumnWidth) << std::left << "High  " <<
        std::setw(maxColumnWidth) << std::left << "Low  " <<
        std::endl;
    for(const auto& yearPair : countryData){
        int year = yearPair.first;
        const Candlestick& candle = yearPair.second;
        std::cout << std::setw(maxColumnWidth) << std::left << std::fixed << std::setprecision(3)<< year << " ";
        std::cout << std::setw(maxColumnWidth) << std::left << std::fixed << std::setprecision(3)<< candle.open << " ";
        std::cout << std::setw(maxColumnWidth) << std::left << std::fixed << std::setprecision(3)<< candle.close << " ";
        std::cout << std::setw(maxColumnWidth) << std::left << std::fixed << std::setprecision(3)<< candle.high << " ";
        std::cout << std::setw(maxColumnWidth) << std::left << std::fixed << std::setprecision(3)<< candle.low << " ";
        std::cout << std::endl;
    }
    std::cout << std::endl;    

    // year in columns
    std::cout << std::setw(maxColumnWidth) << std::left << "Year"; 
    for(const auto& yearPair : countryData){
        int year = yearPair.first;
        const Candlestick& candle = yearPair.second;
        std::cout << std::setw(maxColumnWidth) << std::left << std::fixed << std::setprecision(3)<< year << " ";
    }
    std::cout << std::endl;

     // Print the open values with adjusted width */
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


}

void Data::printCandlestickChart(const std::map<std::string, std::map<int, Candlestick>>& candlesticks) {

    if (candlesticks.empty() || candlesticks.begin()->second.empty()) {
        std::cout << "No candlestick data to display." << std::endl;
        return;
    }

    int chartHeight = 30;
    int barWidth = 6;
    std::string colorOption;

    // Get the selectedCountry variable
    std::string selectedCountry = getCountry(candlesticks);
    
    std::cout << "***************************** " << std::endl; 
    std::cout << "Candlestick chart for Country: " << selectedCountry << std::endl; 
    std::cout << "***************************** " << std::endl; 

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
        std::cout << std::setw(maxColumnWidth) << std::left << std::fixed << std::setprecision(3)<< year << " ";
    }
    std::cout << std::endl;
    
    
/*     // Print the open values with adjusted width */
/*     std::cout << std::setw(maxColumnWidth) <<std::left << "Open ";  */
/*     for(const auto& yearPair : countryData){ */
/*         const Candlestick& candle = yearPair.second; */
/*         std::cout << std::setw(maxColumnWidth) << candle.open << " "; */
/*     } */
/*     std::cout << std::endl; */
/*     // Print the close values with adjusted width */
/*     std::cout << std::setw(maxColumnWidth) <<std::left << "Close ";  */
/*     for(const auto& yearPair : countryData){ */
/*         const Candlestick& candle = yearPair.second; */
/*         std::cout << std::setw(maxColumnWidth) << candle.close << " "; */
/*     } */
/*     std::cout << std::endl; */
/*     // Print the High values with adjusted width */
/*     std::cout << std::setw(maxColumnWidth) <<std::left << "High ";  */
/*     for(const auto& yearPair : countryData){ */
/*         const Candlestick& candle = yearPair.second; */
/*         std::cout << std::setw(maxColumnWidth) << candle.high << " "; */
/*     } */
/*     std::cout << std::endl; */
/*     // Print the Low values with adjusted width */
/*     std::cout << std::setw(maxColumnWidth) <<std::left << "Low ";  */
/*     for(const auto& yearPair : countryData){ */
/*         const Candlestick& candle = yearPair.second; */
/*         std::cout << std::setw(maxColumnWidth) << candle.low << " "; */
/*     } */
/*     std::cout << std::endl; */
    //std::cout << "----------------------------------" << std::endl;
    // Print "Year" below the y-axis values
    std::cout << std::setw(maxColumnWidth) << std::left << "--------" << " |"; 
    for(const auto& yearPair : countryData){
        std::cout << std::setw(maxColumnWidth) << std::left << "---------";
    }
    std::cout << std::endl;

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

            //colorOption = colorRed; // Choose the color you want to use
            // Set colorOption based on open/close comparison
            colorOption = (candle.close > candle.open) ? colorGreen : colorRed;



            if (open <= tempValue && open > tempValueL) {
            // Print "U" or "D" if open value falls within the current row's range
                std::cout << colorOption << std::setw(maxColumnWidth) << std::left << (open > close ? "O" : "O") << " " << colorReset; 
            } else if (close >= tempValueL && close < tempValue) {
            // Print "U" or "D" if close value falls within the current row's range
                std::cout << colorOption << std::setw(maxColumnWidth) << std::left << (open > close ? "C" : "C") << " " << colorReset; 
            //}
            //if(realBodyHigh <=tempValue && realBodyHigh>tempValueL){
            //    std::cout << std::setw(maxColumnWidth) << std::left << realBodyHigh << " ";
            //}else if (realBodyLow >=tempValueL && realBodyLow<tempValue)
            //{
            //    std::cout << std::setw(maxColumnWidth) << std::left << realBodyLow << " ";
            }else if (high >= tempValue && realBodyHigh < tempValue) { 
                // Print "|" if high value falls within the current row's range
                std::cout << colorOption << std::setw(maxColumnWidth) << std::left << "|" << " " << colorReset; 
            }else if (low <= tempValue && realBodyLow > tempValueL) { 
                // Print "|" if low value falls within the current row's range
               std::cout << colorOption << std::setw(maxColumnWidth) << std::left << "|" << " " << colorReset; 
            }else if (tempValue >= std::min(open, close) && tempValueL <= std::max(open, close)) {
            // Print "*" if the current row is between the open and close values
            std::cout << colorOption << std::setw(maxColumnWidth) << std::left << "*" << " " << colorReset; 
            }else {
                //Print blank space to maintain alignment
                std::cout << colorOption << std::setw(maxColumnWidth) << std::left << "" << " " << colorReset; 
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