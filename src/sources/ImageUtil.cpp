#include <opencv2/opencv.hpp>
#include <random>

#include "../headers/ImageUtil.hpp"


void setPixelOnImage(cv::Mat image, Pixel pixel, cv::Vec3b color){
    image.at<cv::Vec3b>(pixel.getY(), pixel.getX()) = color;
}
void setPixelOnImage(cv::Mat image, Pixel pixel, int red, int green, int blue){
    uchar cred = (uchar) red;
    uchar cgreen = (uchar) green;
    uchar cblue = (uchar) blue;
    cv::Vec3b color = cv::Vec3b(cblue, cgreen, cred);
    image.at<cv::Vec3b>(pixel.getY(), pixel.getX()) = color;
}

void setPixelsOnImage(cv::Mat image, std::vector<Pixel> pixels, int red, int green, int blue){
    uchar cred = (uchar) red;
    uchar cgreen = (uchar) green;
    uchar cblue = (uchar) blue;
    cv::Vec3b color = cv::Vec3b(cblue, cgreen, cred);
    for (Pixel pixel : pixels){
        setPixelOnImage(image,pixel, color);
    }
}

void setPixelsOnImage(cv::Mat image, std::vector<Pixel> pixels, cv::Vec3b color){
    for (Pixel pixel : pixels){
        setPixelOnImage(image,pixel, color);
    }
}


cv::Vec3b getPixelOnImage(cv::Mat image, Pixel pixel){
    return image.at<cv::Vec3b>(pixel.getY(), pixel.getX());
}

cv::Vec3b randomColor(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    uchar cred = 255 - dis(gen);
    uchar cgreen = 255 - dis(gen);
    uchar cblue = 255 - dis(gen);
    return cv::Vec3b(cblue, cgreen, cred);
}