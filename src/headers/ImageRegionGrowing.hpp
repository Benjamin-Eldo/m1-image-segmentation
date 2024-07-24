#include <opencv2/opencv.hpp> 
#include <vector>
#include <mutex>
#include <set>
#include <condition_variable>
#include <unordered_map>
#include "Region.hpp"

class ImageRegionGrowing{
    public:
        ImageRegionGrowing(const cv::Mat& image);
        ImageRegionGrowing(const ImageRegionGrowing& irg);
        ~ImageRegionGrowing();
        cv::Mat execute();
        cv::Mat drawRegions();
        void getColorPalette();
    private:
        std::vector<Region*>* listRegion;
        cv::Mat image;
        std::unordered_map<short, cv::Vec3b> palette;
};