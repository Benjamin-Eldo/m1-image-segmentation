#include "../Settings.hpp"
#include "../headers/ImageProcessing/GrowthChamber.hpp"
#include <vector>
#include <iostream>
#include <future>

GrowthChamber::GrowthChamber(std::vector<Region*>* listRegionToGrow, cv::Mat image) : ImageProcess(listRegionToGrow){
    listGrowingRegion = std::vector<Region*>(*listRegionToGrow);
    this->image = image;
}

void GrowthChamber::execute(){
    std::vector<std::future<int>> listFuture;
    std::vector<Region*> regionComplete;
    while(listGrowingRegion.size()!=0){
        for (Region* region : listGrowingRegion){
            listFuture.push_back(std::async([region, this]() {
                return region->grow(std::ref(image), PIXEL_COLOR_SIMILARITY_THRESHOLD);
            }));
        }
        for (int i = 0 ; i < listFuture.size();i++){
            if (listFuture[i].get()==0){
                regionComplete.push_back(listGrowingRegion[i]);
            }
        }
        for (Region* region : listGrowingRegion){
            region->colorPixels();
        }
        for (Region* region : regionComplete){
            listGrowingRegion.erase(std::remove(listGrowingRegion.begin(), 
                listGrowingRegion.end(), region), listGrowingRegion.end());
        }
        listFuture.clear();
    }
}