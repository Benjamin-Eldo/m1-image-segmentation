#include "../Settings.hpp"
#include "../headers/ImageRegionGrowing.hpp"
#include "../headers/RegionFactory.hpp"
#include "../headers/ImageUtil.hpp"
#include "../headers/ImageProcessing/GrowthChamber.hpp"
#include "../headers/ImageProcessing/RegionMerger.hpp"
#include <thread>
#include <mutex>
#include <unordered_set>
#include <opencv2/opencv.hpp> 


ImageRegionGrowing::ImageRegionGrowing(const cv::Mat& image){
    this->image = image;
}

ImageRegionGrowing::ImageRegionGrowing(const ImageRegionGrowing& irg){
    this->image = irg.image;
    this->listRegion = irg.listRegion;
}

ImageRegionGrowing::~ImageRegionGrowing(){
    if (!listRegion->empty()){
        delete listRegion->at(0)->getPixelRegion();
    }
    for (int i = 0; i < listRegion->size();i++)
    {
        delete listRegion->at(i);
    }
    delete listRegion;
}

cv::Mat ImageRegionGrowing::execute(){
    RegionFactory factory = RegionFactory();
    this->listRegion = new std::vector<Region*> (factory.buildRegions(image,NUMBER_OF_REGIONS));

    GrowthChamber gc = GrowthChamber(listRegion,image);

    gc.execute();

    for (Region* region : *listRegion) {
        region->updateBorder();
    }

    RegionMerger regionMerger = RegionMerger(listRegion);
    regionMerger.execute();

    return drawRegions();
}

void ImageRegionGrowing::getColorPalette(){
    palette[-1] = randomColor();
    for(Region* region : *listRegion){
        palette[region->getID()] = randomColor();
    }
}

cv::Mat ImageRegionGrowing::drawRegions(){
    getColorPalette();

    cv::Mat imageColored = cv::Mat(image.rows, image.cols, CV_8UC3, cv::Scalar(0, 0, 0));

    std::vector<std::vector<short>>* pixelRegion = listRegion->at(0)->getPixelRegion();

    if(!DRAW_ONLY_BORDERS) {
        for(int i=0;i<pixelRegion->size();i++) {
            for(int j=0;j<pixelRegion[0][0].size();j++) {
                imageColored.at<cv::Vec3b>(i, j) = palette[(*pixelRegion)[i][j]];
            }
        }
    }

    //parti colorie border
    for (Region* region : *listRegion) {
        cv::Vec3b couleur;
        if (DRAW_ONLY_BORDERS) {
            couleur = palette[region->getID()];
        } else {
            couleur = cv::Vec3b(0,0,0);
        }
        setPixelsOnImage(imageColored,region->getBorder(), couleur);
    }
    return imageColored;
}
