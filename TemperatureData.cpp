// TemperatureData.cpp
#include "TemperatureData.h"

TemperatureData::TemperatureData(std::string _timestamp,
                                int _year,
                                std::map<std::string, double> _countryTemperatures
)
: timestamp(_timestamp),
  year(_year),
  countryTemperatures(_countryTemperatures)
  {

  };