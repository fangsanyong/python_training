#include <opencv2/opencv.hpp>
#include <vector>
using namespace std;
class MotTrack {
public:
    MotTrack();
    ~MotTrack();
    void run(cv::Mat& frame,std::vector<cv::Rect>& remained);

public:
    int track_len;
    int detect_interval;
    std::vector<std::vector<cv::Point2f>> tracks;
    int frame_idx;
    cv::Mat prev_gray;
    cv::Mat frame_gray;
    //cv::Mat kernel;
    cv::Ptr<cv::BackgroundSubtractorMOG2> model;
    cv::TermCriteria lk_params;
    

};
