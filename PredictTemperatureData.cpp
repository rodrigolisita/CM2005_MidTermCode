// PredictTemperatureData.cpp
#include "PredictTemperatureData.h"

PredictTemperatureData::PredictTemperatureData(int _year,
                                               std::string _country,
                                               double _temperature,
                                               int _startYear,
                                               int _endYear,
                                               double _nextTemperature,
                                               double _backwardDifference,
                                               double _euDifference,
                                               double _taylorPrediction,
                                               double _averageALL)  // No default value here
    : year(_year),
      country(_country),
      temperature(_temperature),
      startYear(_startYear),
      endYear(_endYear),
      nextTemperature(_nextTemperature),
      backwardDifference(_backwardDifference),
      euDifference(_euDifference),
      taylorPrediction(_taylorPrediction),
      averageALL(_averageALL)
{
 
}