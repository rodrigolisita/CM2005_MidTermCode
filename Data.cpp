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

#include "ANSI_CODES.h" //For the color codes

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
    std::cout << "5: Filter data by year range" << std::endl;
    std::cout << "6: Filter data by Country" << std::endl;
    std::cout << "7: Predict data by Country" << std::endl;
    
    std::cout << "8: Exit" << std::endl;
}


int Data::getUserOption() {
    int userOption;

    std::cout << "Enter your choice: ";
    std::cin >> userOption;

    // Input validation 
    while (std::cin.fail() || userOption < 1 || userOption > maxMenuOptions) { 
        std::cout << "Invalid input. Please enter a number between 1 and " << maxMenuOptions << ":";
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
            //filterByDateRange();
            printFilteredAverageTemperatureData(candlesticks);
            break;
        case 6: 
            filterByCountry(candlesticks);
            break;            
        case 7:
            predictData(candlesticks);
            break;
        case 8:
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

    /* // Loop through countries */
    /* for (const auto& countryPair : countries) { */
    /*     std::string country = countryPair.first; */

    /*     int currentYear = -1; // Initialize with an invalid year */

    /*     // Loop through years (from the data) */
    /*     for (const auto& line : data) { */
    /*         int year = line.year; */
    /*         double temperature = line.countryTemperatures.at(country); */


    /*         if(year != currentYear){ */
    /*             //New year detected */
    /*             currentYear = year; */

    /*             //Initialize a new candlestick for this year */
    /*             candlesticks[country][year] = Candlestick(country, temperature, temperature, temperature, temperature); */
    /*         } else { */
    /*             // same year, update the close value */
    /*             candlesticks[country][year].close = temperature; */

    /*             // Update high and low */
    /*             candlesticks[country][year].high = std::max(candlesticks[country][year].high, temperature); */
    /*             candlesticks[country][year].low  = std::min(candlesticks[country][year].low,  temperature);  */

    /*         } */
    /*     } */
    /* } */

    for (const auto& countryPair : countries) {
        std::string country = countryPair.first;

        std::map<int, std::vector<double>> yearlyTemperatures; 

        // Collect temperatures for each year
        for (const auto& line : data) {
            int year = line.year;
            double temperature = line.countryTemperatures.at(country);
            yearlyTemperatures[year].push_back(temperature);
        }

        // Calculate candlesticks 
        for (auto it = yearlyTemperatures.begin(); it != yearlyTemperatures.end(); ++it) {
            int year = it->first;
            const std::vector<double>& temps = it->second;

            // Calculate average open (use the previous year's temperatures if available)
            double open = 0.0;
            if (it == yearlyTemperatures.begin()) {
                const std::vector<double>& temps = it->second;
                if (!temps.empty()) {
                    open = temps[0]; // Get the first temperature of the year
                }
            } else {
                auto prevIt = std::prev(it);
                const std::vector<double>& prevTemps = prevIt->second;
                open = std::accumulate(prevTemps.begin(), prevTemps.end(), 0.0) / prevTemps.size();
            }


            // Calculate average close (average of temperatures for the current year)
            double close = std::accumulate(temps.begin(), temps.end(), 0.0) / temps.size(); 

            // Find high and low
            double high = *std::max_element(temps.begin(), temps.end());
            double low = *std::min_element(temps.begin(), temps.end());

            candlesticks[country][year] = Candlestick(country, open, close, high, low);
        }
    }
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
    std::cout << std::setw(maxColumnWidth) << std::left << "Year" << " " 
              << std::setw(maxColumnWidth) << std::left << "Open" << " "
              << std::setw(maxColumnWidth) << std::left << "Close" << " "
              << std::setw(maxColumnWidth) << std::left << "High" << " "
              << std::setw(maxColumnWidth) << std::left << "Low" << " "
              << std::endl;
        
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

    int chartHeight = 50;
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
    
    // Print "Year" below the y-axis values
    std::cout << std::setw(maxColumnWidth) << std::left << "--------" << " |"; 
    for(const auto& yearPair : countryData){
        std::cout << std::setw(maxColumnWidth) << std::left << "---------";
    }
    std::cout << std::endl;

    // Add a margin to the high and low values for the y-axis range
    double margin = 0.01 * countryRange; // 1% margin
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
                //std::cout << colorOption << std::setw(maxColumnWidth) << std::left << (open > close ? "O" : "O") << " " << colorReset; 
                std::cout << colorOption << std::setw(maxColumnWidth) << std::left << "O" << " " << colorReset; 
            } else if (close >= tempValueL && close < tempValue) {
            // Print "U" or "D" if close value falls within the current row's range
                //std::cout << colorOption << std::setw(maxColumnWidth) << std::left << (open > close ? "C" : "C") << " " << colorReset; 
                std::cout << colorOption << std::setw(maxColumnWidth) << std::left << "C" << " " << colorReset; 
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

std::pair<int, int> Data::getDateRangeFromUser() {
    int startYear, endYear;

    // Find the minimum and maximum years in the data
    int minYear = data.front().year;
    int maxYear = data.back().year;

    // Get starting year input with validation
    do {
        std::cout << "Enter the starting year (minimum: " << minYear << "): ";
        std::cin >> startYear;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (startYear < minYear || startYear > maxYear) {
            std::cout << "Invalid year. Please enter a year between " << minYear << " and " << maxYear << "." << std::endl;
        }
    } while (std::cin.fail() || startYear < minYear || startYear > maxYear);

    // Get ending year input with validation
    do {
        std::cout << "Enter the ending year (maximum: " << maxYear << "): ";
        std::cin >> endYear;

        if (std::cin.fail()) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (endYear < minYear || endYear > maxYear) {
            std::cout << "Invalid year. Please enter a year between " << minYear << " and " << maxYear << "." << std::endl;
        } else if (endYear < startYear) {
            std::cout << "Invalid range: Ending year cannot be less than starting year." << std::endl;
        }
    } while (std::cin.fail() || endYear < minYear || endYear > maxYear || endYear < startYear);

    return std::make_pair(startYear, endYear); // Return the start and end years as a pair
}

void Data::filterData(int startYear, int endYear, const std::map<std::string, std::map<int, Candlestick>>& candlesticks) {
 
    filteredData.clear(); // Clear any previous filtered data

//    for (const auto& line : data) {
//        if (line.year >= startYear && line.year <= endYear) {
//            for (const auto& countryPair : line.countryTemperatures) {
//                filteredData.push_back(FilteredData{line.year, countryPair.first, countryPair.second, startYear, endYear}); 
//            }
//        }
//    }
    
    // Filter and store the candlestick data in filteredData
    for (const auto& countryPair : candlesticks) {
        const std::string& country = countryPair.first;
        for (const auto& yearPair : countryPair.second) {
            int year = yearPair.first;
            if (year >= startYear && year <= endYear) {
                const Candlestick& candle = yearPair.second;

                // Store the average temperature (open or close - they are the same)
                filteredData.push_back(FilteredData{year, country, candle.close, startYear, endYear}); 
            }
        }
    }
}

void Data::filterByDateRange(const std::map<std::string, std::map<int, Candlestick>>& candlesticks) {

    std::pair<int, int> dateRange = Data::getDateRangeFromUser();
    int startYear = dateRange.first;
    int endYear = dateRange.second;
 
    filterData(startYear, endYear, candlesticks); // Call filterData to populate filteredData


}

void Data::printFilteredAverageTemperatureData(const std::map<std::string, std::map<int, Candlestick>>& candlesticks) {                                    

    Data::filterByDateRange(candlesticks);

    // Group data by country
    std::map<std::string, std::vector<FilteredData>> countryData;
    for (const auto& item : filteredData) {
        countryData[item.country].push_back(item);
    }

    // Calculate yearly averages across all countries
    std::map<int, double> yearlyAvgTemps;
    std::map<int, int> yearlyCount; 
    for (const auto& item : filteredData) {
        yearlyAvgTemps[item.year] += item.temperature;
        yearlyCount[item.year]++;
    }
    for (auto& pair : yearlyAvgTemps) {
        pair.second /= yearlyCount[pair.first];
    }

    // Print
    for (const auto& countryPair : countryData) {
        std::cout << "\nCountry: " << countryPair.first << std::endl;
        std::cout << std::setw(4) << std::left << "Year" << ": " 
        << std::left << std::setw(11) << "Temperature" << " " 
        << std::left << std::setw(11) << "/ EU Average" << std::endl; 
        
        for (const auto& item : countryPair.second) {
        
        // Set the color based on the comparison with EU average
        std::string tempColor = (item.temperature >  yearlyAvgTemps.at(item.year)) ? colorRed : colorBlue;
        std::cout << std::setw(4) << std::left << item.year << ": " 
                  << std::right << tempColor << std::setw(11) << std::fixed << std::setprecision(5) << item.temperature << colorReset << " / " 
                  << std::left << std::fixed << std::setw(11) << std::setprecision(5) <<  yearlyAvgTemps.at(item.year) << std::endl;
        }
    }


}

void Data::filterByCountry(const std::map<std::string, std::map<int, Candlestick>>& candlesticks){

    // Find the minimum and maximum years in the data
    int startYear = data.front().year;
    int endYear = data.back().year;
    

    // Get the selectedCountry variable
    std::string selectedCountry = getCountry(candlesticks);

    filteredData.clear();

    // Filter and store the candlestick data in filteredData for the selected country
    const auto& countryData = candlesticks.at(selectedCountry);
    for (const auto& yearPair : countryData) {
        int year = yearPair.first;
        if (year >= startYear && year <= endYear) {
            const Candlestick& candle = yearPair.second;
            filteredData.push_back(FilteredData{year, selectedCountry, candle.close, startYear, endYear});
        }
    }

    // Find the minimum and maximum temperatures in the filtered data
    double minTemp = std::numeric_limits<double>::max();
    double maxTemp = std::numeric_limits<double>::min();
    for (const auto& item : filteredData) {
        minTemp = std::min(minTemp, item.temperature);
        maxTemp = std::max(maxTemp, item.temperature);
    }

    // Add a margin to the high and low values for the y-axis range
    double margin = 0.1 * (maxTemp - minTemp); // 10% margin
    double yAxisHigh = maxTemp + margin;
    double yAxisLow = minTemp - margin;

    // Calculate the step size for the y-axis values
    int chartHeight = 20; 
    double yAxisStep = (yAxisHigh - yAxisLow) / chartHeight;

    // Determine the maximum width for the year labels
    int maxYearWidth = 0;
    for (const auto& item : filteredData) {
        std::stringstream ss;
        ss << item.year;
        maxYearWidth = std::max(maxYearWidth, static_cast<int>(ss.str().length()));
    }

    // Print the y-axis values
    for (int i = chartHeight; i >= 0; --i) {
        double tempValue = yAxisHigh - i * yAxisStep;
        std::cout << std::setw(7) << std::setprecision(5) << tempValue << " | ";

        for (const auto& item : filteredData) {
            //if (item.temperature >= tempValue - yAxisStep && item.temperature <= tempValue) {
            if (item.temperature <= tempValue) {
                std::cout << std::setw(maxYearWidth) << "#" << " "; // Use maxYearWidth for alignment
            } else {
                std::cout << std::setw(maxYearWidth) << "" << " "; // Use maxYearWidth for alignment
            }
        }
        std::cout << std::setw(7) << std::setprecision(5) << tempValue << " | ";
        std::cout << std::endl;
    }

    // Print the x-axis (years)
    //std::cout << std::setw(7) << std::setprecision(5) << "Year" << " | ";
    std::cout << std::setw(7) << "Year" << " | ";
    for (const auto& item : filteredData) {
        std::cout << std::setw(maxYearWidth) << item.year << " "; // Use maxYearWidth for alignment
    }
    std::cout << std::endl;

}

void Data::predictData(const std::map<std::string, std::map<int, Candlestick>>& candlesticks)
{
    // Get the date range from the user
    //std::pair<int, int> dateRange = getDateRangeFromUser();
    //int startYear = dateRange.first;
    //int endYear = dateRange.second;
    int columnWidth=12;

    int firstYear = data.front().year; // Get the first year in the entire dataset
    int lastYear = data.back().year; // Get the first year in the entire dataset

    int startYear = firstYear;
    int endYear = lastYear;

    computeYearlyAverageTemperatures(); // Average temperatures for all countries

    // Get the selected country from the user
    std::string selectedCountry = getCountry(candlesticks);


    std::vector<TemperatureDifferenceData> tempDiffData; // Use the new class

    filteredData.clear();

    // Filter and store the candlestick data in filteredData for the selected country
    const auto& countryData = candlesticks.at(selectedCountry);
    for (const auto& yearPair : countryData) {
        int year = yearPair.first;
        if (year >= startYear && year <= endYear) {
            const Candlestick& candle = yearPair.second;

            // Access the yearly Average Temperature
            double euAvgTemp = yearlyAverageTemperatures.at(year);
            filteredData.push_back(FilteredData{year, selectedCountry, candle.close, startYear, endYear, euAvgTemp});
        }
    }

    // Calculate temperature differences between consecutive years
    //std::vector<double> backWardDifferences;
    //std::vector<double> second;
    //std::vector<double> difToC;
    //std::vector<double> forwardDifferences;
    //std::vector<double> centralDifferences;
  
    double temperatureDifference = 0.0;
    double previousTemperature = 0.0;
    bool isFirstYear = true; // Flag to track the first year
    bool isSecondYear = true; // Flag to track the second year

    for (auto& item : filteredData) {
        if (item.year == firstYear && isFirstYear) {  //First year
            temperatureDifference = item.temperature;
            isFirstYear = false; // Reset the flag after processing the first year
        } else if (isSecondYear) { // Check for the second year
            temperatureDifference = item.temperature - filteredData.front().temperature;
            isSecondYear = false; // Reset the flag after processing the second year
        } else {
            temperatureDifference = item.temperature - previousTemperature;
        }
        tempDiffData.push_back(TemperatureDifferenceData{item.year, item.country, item.temperature, 
                                                         item.startYear, item.endYear, temperatureDifference, item.euAvTemp, item.temperature});
        //backWardDifferences.push_back(temperatureDifference);
        //item.backWardDifference = temperatureDifference;
        previousTemperature = item.temperature;
    }

    // Correct the EU difference
    for (auto& item : tempDiffData) {
        item.euDifference = item.temperature - item.euDifference;
    }

    // Correct next temperature value
    for (auto it = tempDiffData.begin(); it != tempDiffData.end(); ++it) {
        auto& item = *it; // Get the current item
        double nextTemp = item.temperature;
        if (std::next(it) != tempDiffData.end()) { // Check if there is a next item
            nextTemp = std::next(it)->temperature;
        }
        item.nextTemperature = nextTemp;
     }
   
    // Print the temperatures and differences
    std::cout << std::left << std::setw(5) << "Year" << "|";
    std::cout << std::left << std::setw(columnWidth) << "Temperature" << "|";
    std::cout << std::left << std::setw(columnWidth) << "backwardDif" << " ";
    std::cout << std::left << std::setw(columnWidth) << "ERR %" << "|";
//    std::cout << std::left << std::setw(columnWidth) << "Taylor" << " ";
//    std::cout << std::left << std::setw(columnWidth) << "ERR %" << "|";
    std::cout << std::left << std::setw(columnWidth) << "EU" << " ";
    std::cout << std::left << std::setw(columnWidth) << "ERR %" << "|";
//    //std::cout << std::left << std::setw(headerWidths[3]) << "centralDif" << " ";
    std::cout << std::left << std::setw(columnWidth) << "Actual" << std::endl;
    for (const auto& item : tempDiffData) {
    //for (auto it = tempDiffData.begin(); it != tempDiffData.end(); ++it) {
      //  const auto& item = *it; // Get the current item
        double nextTemp = item.nextTemperature;
      //   if (std::next(it) != tempDiffData.end()) { // Check if there is a next item
      //      nextTemp = std::next(it)->temperature;
      //   }
    //for (size_t i = 1; i < filteredData.size()-1; ++i) { // Loopover the second year to the second to last
        std::cout << std::left << std::setw(5) << item.year << "|";
        double t = item.temperature;
        double bd = item.temperature + item.backwardDifference;
        double erD = item.temperature + item.euDifference;
//        double d2 = filteredData[i].temperature + tempDifferences[i] + 0.5*second[i];
//        double d3 = filteredData[i].temperature + difToC[i];
        double err1 = Data::error(nextTemp,bd);
//        double err2 = Data::error(t,d2);
        double err3 = Data::error(nextTemp,erD);
        std::cout << std::left << std::setw(columnWidth) << t << "|";
        std::cout << std::left << std::setw(columnWidth) <<  bd << " ";
        std::cout << std::left << std::setw(columnWidth) <<  err1 << "|";
        std::cout << std::left << std::setw(columnWidth) << erD << " ";
        std::cout << std::left << std::setw(columnWidth) <<  err3 << "|";
//        std::cout << std::left << std::setw(columnWidth) << d3 << " ";
//        std::cout << std::left << std::setw(columnWidth) <<  err3 << "|";
//        //std::cout << std::left << std::setw(headerWidths[4]) << filteredData[i].temperature + centralDifferences[i] << " ";
        //if (std::next(it) != tempDiffData.end()) { // Check if there is a next item
            //std::cout << std::next(it)->temperature; // Print the next item's temperature
            //std::cout << std::left << std::setw(columnWidth) << std::next(it)->temperature << "|";
            std::cout << std::left << std::setw(columnWidth) << nextTemp << "|";
            
        //}

        
        std::cout << std::endl;
    }




}

double Data::error (double x1, double x2){
    double err = 100*(x1-x2)/x1;
    return err;
}

void Data::computeYearlyAverageTemperatures() { 
    std::map<int, std::vector<double>> yearlyTemperatures;

    // Collect temperatures for each year
    for (const auto& line : data) {
        int year = line.year;
        for (const auto& countryPair : line.countryTemperatures) {
            yearlyTemperatures[year].push_back(countryPair.second);
        }
    }

    // Calculate average temperature for each year
    for (const auto& yearPair : yearlyTemperatures) {
        int year = yearPair.first;
        const std::vector<double>& temps = yearPair.second;
        double avgTemp = std::accumulate(temps.begin(), temps.end(), 0.0) / temps.size();
        yearlyAverageTemperatures[year] = avgTemp;
    }
}