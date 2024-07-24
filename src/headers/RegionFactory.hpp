#include <vector>
#include <opencv2/opencv.hpp> 

#include "Region.hpp"

class RegionFactory{
    public :
        RegionFactory();
        std::vector<Region*> buildRegions(cv::Mat image,int nbRegion);
};