#include "TemperatureData.h"

TemperatureData::TemperatureData(std::string _timestamp,
                                int _year,
                                double _AT,
                                double _BE,
                                double _BG,
                                double _CH
)
: timestamp(_timestamp),
  year(_year),
  AT(_AT),
  BE(_BE),
  BG(_BG),
  CH(_CH)
  {

  };