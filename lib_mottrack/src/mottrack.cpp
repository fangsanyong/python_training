#include "mottrack.h"

MotTrack::MotTrack()
{    
    track_len = 200;
    detect_interval = 30;
    frame_idx = 0;
    //kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    lk_params = cv::TermCriteria(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10, 0.03);
    
    model = cv::createBackgroundSubtractorMOG2();
}

MotTrack::~MotTrack()
{    
    
}

void MotTrack::run(cv::Mat& frame,std::vector<cv::Rect>& remained) {
    //cv::TermCriteria termcrit(cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10, 0.03);
    //cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);

    // 背景建模（MOG2）
    model->apply(frame, frame_gray);
    //cv::imshow("1", frame_gray);
    // 腐蚀和膨胀，去噪点
cv::threshold(frame_gray, frame_gray, 100, 255, cv::THRESH_BINARY);
    cv::dilate(frame_gray, frame_gray, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)), cv::Point(-1, -1), 1);
        cv::erode(frame_gray, frame_gray, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)), cv::Point(-1, -1), 1);
    //cv::imshow("2", gmask);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(frame_gray, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 绘制轮廓
    cv::drawContours(frame_gray, contours, -1, cv::Scalar(0, 0, 255), 1);

    cv::findContours(frame_gray, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    // 处理轮廓获取外接矩形
    std::vector<cv::Rect> rect;
    for (auto& c : contours) {
        cv::Rect r = cv::boundingRect(c);
        rect.push_back(r);
    }

    remained.clear();
    

    // 计算光流
    if (!tracks.empty()) {

        //img1 = frame_gray;
        std::vector<cv::Point2f> p0, p1, p0r;
        for (auto& tr : tracks) {
            p0.push_back(tr.back());
        }
        std::vector<uchar> status;
        std::vector<float> err;
        cv::calcOpticalFlowPyrLK(prev_gray, frame_gray, p0, p1, status, err,cv::Size(15,15), 2, lk_params);
        cv::calcOpticalFlowPyrLK(frame_gray, prev_gray, p1, p0r, status, err,cv::Size(15,15), 2, lk_params);

        for (int i = 0; i < p0.size(); i++) {
            double dx = std::abs(p0[i].x - p0r[i].x);
            double dy = std::abs(p0[i].y - p0r[i].y);
            double d = std::max(dx, dy);
            if (d < 1.0)
                status[i] = 1;
            else
                status[i] = 0;
        }

        std::vector<std::vector<cv::Point2f>> new_tracks;
        for (int i = 0; i < tracks.size(); i++) {
            if (!status[i])
                continue;
            std::vector<cv::Point2f> tr = tracks[i];
            cv::Point2f p = p1[i];
            // cout<<p<<endl;
            tr.push_back(p);
            
            for (int j = 0; j < rect.size(); j++) {
                cv::Rect r = rect[j];
                if (p.x >= r.x && p.x <= (r.x + r.width) && p.y >= r.y && p.y <= (r.y + r.height)) {
                    if (std::find(remained.begin(), remained.end(), r) == remained.end()) {
                        remained.push_back(r);
                    }
                    
                    break;
                }
            }
            if (tr.size() > track_len)
                tr.erase(tr.begin());

            new_tracks.push_back(tr);
        }
        tracks = new_tracks;      
    }

    if (frame_idx % detect_interval == 0) {
        cv::Mat mask = cv::Mat::zeros(frame_gray.size(), CV_8U);
        mask.setTo(255);
        for (auto& p : tracks) {
            cv::Point2f point = p.back();
            cv::circle(mask, point, 5, 0, -1);
        }
    
        std::vector<cv::Point2f> p;
        cv::goodFeaturesToTrack(frame_gray, p, 500, 0.3, 7, mask, 7);
        if (!p.empty()) {
            for (auto& point : p) {
                tracks.push_back(std::vector<cv::Point2f>(1, point));
            }
        }
    }

    prev_gray = frame_gray.clone();
    frame_idx++;   
}
