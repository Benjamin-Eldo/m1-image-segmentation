#include "Pixel.hpp"
#include <opencv2/opencv.hpp> 

void setPixelOnImage(cv::Mat image,Pixel pixel, cv::Vec3b color);
void setPixelOnImage(cv::Mat image,Pixel pixel, int red, int green, int blue);
void setPixelsOnImage(cv::Mat image, std::vector<Pixel> pixels, int red, int green, int blue);
void setPixelsOnImage(cv::Mat image, std::vector<Pixel> pixels, cv::Vec3b color);

cv::Vec3b getPixelOnImage(cv::Mat image,Pixel pixel);
cv::Vec3b randomColor();