#include <vector>
#include "../Region.hpp"
#include "ImageProcess.hpp"
#include <opencv2/opencv.hpp> 


class GrowthChamber : public ImageProcess{
    private :
        std::vector<Region*> listGrowingRegion;
        cv::Mat image;
    public:
        GrowthChamber(std::vector<Region*>* listRegionToGrow, cv::Mat image);
        void execute();
};