#include <vector>
#include <thread>

#include "../headers/VideoRegionGrowing.hpp"
#include "../headers/ImageRegionGrowing.hpp"
#include "../Settings.hpp"

VideoRegionGrowing::VideoRegionGrowing(cv::VideoCapture video){
    this->video = video;
    cv::Mat frame;
    video >> frame;
    this->cols = frame.cols;
    this->rows = frame.rows;
    this->processing = false;
    this->activeWorker = 0;
}

VideoRegionGrowing::~VideoRegionGrowing(){
}

void VideoRegionGrowing::showVideo(cv::VideoCapture videoToPrint) {
    cv::Mat frame;
    while (true) {
        videoToPrint >> frame;

        if (frame.empty())
            break;

        cv::imshow("Vidéo", frame);
        char c = (char)cv::waitKey(25);

        if (c == 27)
            break;
    }

    video.release();
}

void VideoRegionGrowing::execute(){
    std::thread workerDeployer = std::thread(&VideoRegionGrowing::deployWorkers, this);
    workerDeployer.detach();
    recoverWorkers();
}


void VideoRegionGrowing::deployWorkers(){
    processing = true;
    while (true) {
        cv::Mat frame;
        video >> frame;

        if (frame.empty()){
            processing = false;
            break;
        }
    

        mutex.lock();
        while (activeWorker >= MAX_FRAME_COMPUTED_AT_ONCE){
            joinCV.wait(mutex);
        }
        if (activeWorker < MAX_FRAME_COMPUTED_AT_ONCE){
            workers.push(std::async(std::launch::async, &VideoRegionGrowing::computeFrame, this, frame));
            activeWorker++;
            mutex.unlock();
            createCV.notify_one();
        }

    }
}

void VideoRegionGrowing::recoverWorkers(){
    int frameProcessed = 0;
    cv::VideoWriter newVideo(OUTPUT_VIDEO, cv::VideoWriter::fourcc('M','J','P','G'), 30, cv::Size(cols, rows));
    int frameCount = video.get(cv::CAP_PROP_FRAME_COUNT);
    if (frameCount == 0)
        std::cout<< "The codex of the video is not supported, we can't show you the progression of the video"<<std::endl;
    while(processing || !workers.empty()){
        mutex.lock();
        while (workers.empty()){
            createCV.wait(mutex);
        }

        if (!workers.empty()){
            std::future<cv::Mat> retrievedWorker = std::move(workers.front());
            workers.pop();
            activeWorker--;
            mutex.unlock();
            joinCV.notify_one();
            newVideo.write(retrievedWorker.get());
            frameProcessed++;
            if (frameCount != 0)
                displayProgress(frameProcessed);
        }
    }
    newVideo.release();
}

cv::Mat VideoRegionGrowing::computeFrame(cv::Mat image){
    ImageRegionGrowing irg(image);
    return irg.execute();
}

void VideoRegionGrowing::displayProgress(int processedFrame){
    int percentage = (processedFrame / video.get(cv::CAP_PROP_FRAME_COUNT)) *100;
    std::cout << "[ "<< percentage << "%]"<<std::endl;
}