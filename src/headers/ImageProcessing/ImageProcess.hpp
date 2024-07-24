#pragma once
#include <vector>
#include "../Region.hpp"

class ImageProcess{
    protected :
        std::vector<Region*>* listRegion;
    public :
        ImageProcess(std::vector<Region*>* _listRegion) : listRegion(_listRegion){}
        virtual void execute() = 0;
};