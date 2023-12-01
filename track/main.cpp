#include "mottrack.h"

int main() {
    MotTrack motTrack;
    std::vector<std::string> dc = {"22", "22-2", "26", "35", "36-2", "49", "57", "68", "68-2"};
    std::string nq = dc[1];
    std::string path = "../../videos/35.mp4";

    cv::VideoCapture cam(path);
 	std::vector<cv::Rect> remained;
    
    while (true) {
        cv::Mat frame;
        bool ret = cam.read(frame);
        if (!ret)
            break;
       
        motTrack.run(frame,remained);
        
        for (int i = 0; i < remained.size(); i++) {
            cv::rectangle(frame, remained[i], cv::Scalar(0, 0, 255), 2);
        }
	
        cv::imshow("lk_track", frame);
        cv::waitKey(1);
    }
    std::cout << "Done" << std::endl;
    cv::destroyAllWindows();
    return 0;
}

