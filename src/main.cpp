#include <opencv2/opencv.hpp> 
#include <stdio.h>
#include <cstring>

#include "Settings.hpp"
#include "headers/ImageRegionGrowing.hpp"
#include "headers/VideoRegionGrowing.hpp"

using namespace cv; 

int main(int argc, char** argv) {
    if (argc > 1){
        if (std::strcmp(argv[1], "image")==0){
            Mat image; 
            image = imread(INPUT_IMAGE, 1); 
            if (!image.data) { 
                std::cerr << "No image data (may be caused by wrong file placement)" << std::endl;
                return -1; 
            }

            std::cout << "Processing image : " << INPUT_IMAGE << " (defined in Settings.hpp)" << std::endl; 

            ImageRegionGrowing regionGrowing = ImageRegionGrowing(image);

            Mat regions = regionGrowing.execute();
            
            namedWindow("Display Image", WINDOW_AUTOSIZE); 
            imshow("Display Image", regions);
            waitKey(0);
            destroyAllWindows();
            return 0;
        }
        else if (std::strcmp(argv[1], "video")==0){
            VideoCapture video(INPUT_VIDEO);

            if (!video.isOpened()) {
                std::cerr << "No video data (may be caused by wrong file placement or bad installation)" << std::endl;
                std::cerr << "Check Settings.hpp for the video path" << std::endl;
                return -1;
            }

            std::cout << "Processing video : " << INPUT_VIDEO << " (defined in Settings.hpp)" <<  std::endl; 

            VideoRegionGrowing videoRegionGrowing = VideoRegionGrowing(video);

            videoRegionGrowing.execute();

            std::cout << "Finished, find result here : " << OUTPUT_VIDEO << std::endl;
            
            return 0;
        }
    }
    std::cout << "Incorrect usage !" << std::endl;
    std::cout << "Use './main image' to process an image or './main video' to process a video" << std::endl;
    
}