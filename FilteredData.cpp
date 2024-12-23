// FilteredData.cpp
#include "FilteredData.h"
#include <numeric> // For std::accumulate 

FilteredData::FilteredData(int _year, std::string _country, double _temperature, int _startYear, int _endYear, double _euAvTemp) 
    : year(_year), 
    country(_country), 
    temperature(_temperature), 
    startYear(_startYear),
    endYear(_endYear),
    euAvTemp(_euAvTemp)
{
    
}