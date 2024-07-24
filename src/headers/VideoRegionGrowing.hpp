#include <opencv2/opencv.hpp> 
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>

class VideoRegionGrowing{
    private:
        cv::VideoCapture video;
        int rows, cols;

        std::queue<std::future<cv::Mat>> workers;
        std::condition_variable_any createCV;
        std::condition_variable_any joinCV;
        int activeWorker;
        std::mutex mutex;
        bool processing;
    public:
        VideoRegionGrowing(cv::VideoCapture video);
        ~VideoRegionGrowing();
        void execute();
        void deployWorkers();
        void recoverWorkers();
        void showVideo(cv::VideoCapture videoToPrint);
        cv::Mat computeFrame(cv::Mat image);
        void displayProgress(int processedFrame);
};