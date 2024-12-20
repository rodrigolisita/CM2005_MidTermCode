// TemperatureData.cpp
#include "TemperatureData.h"

TemperatureData::TemperatureData(std::string _timestamp,
                                int _year,
                                std::map<std::string, double> _countryTemperatures
                                //double _AT,
                                //double _BE,
                                //double _BG,
                                //double _CH
)
: timestamp(_timestamp),
  year(_year),
  countryTemperatures(_countryTemperatures)
  //AT(_AT),
  //BE(_BE),
  //BG(_BG),
  //CH(_CH)
  {

  };