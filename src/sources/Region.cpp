#include "../headers/Region.hpp"
#include "../headers/ImageUtil.hpp"
#include "../headers/ImageRegionGrowing.hpp"
#include <random>

Region::Region(cv::Mat& image, std::vector<std::vector<short>>* region, Pixel graine, short id) {
    pixelRegion = region;
    studiedPixels = std::vector<std::vector<bool>>(pixelRegion->size(), std::vector<bool>(pixelRegion[0][0].size(), false));

    studiedPixels[graine.getY()][graine.getX()] = true;
    (*pixelRegion)[graine.getY()][graine.getX()] = id;
    lastColored = std::vector<Pixel>();
    border = std::vector<Pixel>();
    neighborsId = std::set<short>();
    lastColored.push_back(graine);
    border.push_back(graine);
    color = getPixelOnImage(image, graine);

    idRegion = id;
}

Region::~Region() {}

int Region::grow(cv::Mat & image, int range) {
    std::vector<Pixel> pixelToColor = std::vector<Pixel>();
    for(unsigned int i = 0; i < lastColored.size(); i++) {
        std::vector<Pixel> neighbor = get8Neighbors(lastColored[i]);
        for(unsigned int j = 0; j < neighbor.size(); j++) {
            if (!studiedPixels[neighbor[j].getY()][neighbor[j].getX()] 
            && (*pixelRegion)[neighbor[j].getY()][neighbor[j].getX()] < 0){
                studiedPixels[neighbor[j].getY()][neighbor[j].getX()] = true;
                if(verifyColor(getPixelOnImage(image, neighbor[j]), range)) {
                    if(!isInRegion(neighbor[j])) {
                        pixelToColor.push_back(neighbor[j]);
                    }
                }
            }
        }
    }
    lastColored = pixelToColor;
    return pixelToColor.size();
}

void Region::updateBorder() {
    border.clear();
    for(int i=0;i<pixelRegion->size();i++) {
        for(int j=0;j<pixelRegion[0][0].size();j++) {
            if(studiedPixels[i][j]) {
                if(isInRegion(Pixel(j, i))) {
                    std::vector<Pixel> neighbors = get4Neighbors(Pixel(j, i));
                    if(!pixelsInRegion(neighbors)) {
                        border.push_back(Pixel(j, i));
                    }
                }
            }
        }
    }
}

std::vector<Pixel> Region::get4Neighbors(Pixel pixel){
    std::vector<Pixel> neighbors  = std::vector<Pixel>();
    int x = pixel.getX();
    int y = pixel.getY(); 
    if (pixel.getX()+1 < pixelRegion[0][0].size()){
        neighbors.push_back(Pixel(x+1, y));
    }
    if (pixel.getX()-1 > 0){
        neighbors.push_back(Pixel(x-1, y));    
    }
    if (pixel.getY()-1 > 0){
        neighbors.push_back(Pixel(x, y-1));
    }
    if (pixel.getY() +1 < pixelRegion->size()){
        neighbors.push_back(Pixel(x, y+1));
    }
    return neighbors;
}

bool Region::pixelsInRegion(std::vector<Pixel> listPixel){
    for (Pixel pixel : listPixel){
        if (!isInRegion(pixel)){
            return false;
        }
    }
    return true;
}

void Region::colorPixels() {
    for (Pixel pixel : lastColored){
        (*pixelRegion)[pixel.getY()][pixel.getX()] = idRegion;
    }

}

std::vector<Pixel> Region::get8Neighbors(Pixel pixel) {
    std::vector<Pixel> neighbors = get4Neighbors(pixel);
    int x = pixel.getX();
    int y = pixel.getY(); 
    // diagonales
    if (pixel.getX()+1 < pixelRegion[0][0].size() && pixel.getY()-1 > 0){
        neighbors.push_back(Pixel(x+1, y-1));
    }
    if (pixel.getX()+1 < pixelRegion[0][0].size() && pixel.getY()+1 < pixelRegion->size()){
        neighbors.push_back(Pixel(x+1, y+1));    
    }
    if (pixel.getX()-1 > 0 && pixel.getY()-1 > 0){
        neighbors.push_back(Pixel(x-1, y-1));
    }
    if (pixel.getX()-1 > 0 && pixel.getY() +1 < pixelRegion->size()){
        neighbors.push_back(Pixel(x-1, y+1));
    }
    return neighbors;
}

bool Region::isInRegion(int x, int y) {
    short idPixel = (*pixelRegion)[y][x];
    return idPixel>=0 && idPixel == this->idRegion;
}

bool Region::isInRegion(Pixel pixel) {
    short idPixel = (*pixelRegion)[pixel.getY()][pixel.getX()];
    return idPixel>=0 && idPixel == this->idRegion;
}

bool Region::verifyColor(cv::Vec3b colorToVerify, int range) {
    if(colorToVerify[0] >= color[0] - range && colorToVerify[0] <= color[0] + range) {
        if(colorToVerify[1] >= color[1] - range && colorToVerify[1] <= color[1] + range) {
            if(colorToVerify[2] >= color[2] - range && colorToVerify[2] <= color[2] + range) {
                return true;
            }
        }
    }
    return false;
}

std::vector<Pixel> Region::getBorder() {
    return border;
}

std::vector<Pixel> Region::getLastColored() {
    return lastColored;
}   

std::vector<std::vector<short>>* Region::getPixelRegion(){
    return pixelRegion;
}

short Region::getID() {
    return idRegion;
}

cv::Vec3b Region::getColor() {
    return color;
}

void Region::setStudiedPixels(int row, int col, bool value) {
    studiedPixels[row][col] = value;
}

void Region::findNeighbors() {
    neighborMutex.lock();
    neighborsId.clear();
    for (Pixel pixelBordure : border) {
        std::vector<Pixel> quatreConnexes = get4Neighbors(pixelBordure);
        for (Pixel pixelVoisin : quatreConnexes) {
            if (!isInRegion(pixelVoisin)) {
                short idPixel = (*pixelRegion)[pixelVoisin.getY()][pixelVoisin.getX()];
                if (idPixel >= 0) {
                    neighborsId.insert(idPixel);
                }
            }
        }
    }
    neighborMutex.unlock();
}

std::set<short> Region::getNeighbors(){
    return neighborsId;
}

void Region::fuseRegion(Region* regionToFuse){
    for(int i=0;i<pixelRegion->size();i++) {
        for(int j=0;j<pixelRegion[0][0].size();j++) {
            if ((*pixelRegion)[i][j] == regionToFuse->getID()) {
                (*pixelRegion)[i][j] = this->getID();
                this->setStudiedPixels(i, j, true);
            }
        }
    }
    cv::Vec3b newColor;
    for (int i = 0; i < 3;i++){
        newColor[i] = (this->color[i] + regionToFuse->color[i])/2;
    }
    this->color = newColor;
}