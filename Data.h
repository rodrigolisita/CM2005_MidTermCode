#pragma once

#include<string>
#include<vector>

#include "TemperatureData.h"

class Data
{
    public:
        Data() = default;
        void init();
         void computeStats();
    private:
        void loadData();
        std::vector<TemperatureData> data;
        void printData();

};