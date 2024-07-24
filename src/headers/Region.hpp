#pragma once
#include <opencv2/opencv.hpp>
#include "Pixel.hpp"
#include <mutex>

class Region {
private:
    std::vector<std::vector<short>>* pixelRegion;
    cv::Vec3b color;
    std::vector<Pixel> border;
    std::vector<Pixel> lastColored;
    std::set<short> neighborsId;
    std::vector<std::vector<bool>> studiedPixels;
    short idRegion;
    std::mutex neighborMutex;
public:
    Region(cv::Mat& image, std::vector<std::vector<short>>* region, Pixel graine, short id);
    ~Region();
    int grow(cv::Mat & image, int range);
    void updateBorder();
    void colorPixels();
    void colorBorder(std::vector<Pixel> pixelToColor);
    bool isInRegion(int x, int y);
    bool isInRegion(Pixel pixel);
    bool pixelsInRegion(std::vector<Pixel> listPixel);
    bool verifyColor(cv::Vec3b colorToVerify, int range);
    std::vector<Pixel> getBorder();
    std::vector<Pixel> getLastColored();
    std::vector<Pixel> get8Neighbors(Pixel pixel);
    std::vector<Pixel> get4Neighbors(Pixel pixel);
    std::vector<std::vector<short>>* getPixelRegion();
    short getID();
    cv::Vec3b getColor();
    void setStudiedPixels(int row, int col, bool value);


    void findNeighbors();
    std::set<short> getNeighbors();
    void fuseRegion(Region* regionToFuse);
};