#include <opencv4/opencv2/opencv.hpp>

class Camera
{
public:
    int width, height;

    Camera(int width_, int height_)
    {
        width = width_;
        height = height_;
    }

    cv::VideoCapture getVideoCapture()
    {
        cv::VideoCapture real_time(0, cv::CAP_V4L2);
        real_time.set(cv::CAP_PROP_FRAME_WIDTH, width);
        real_time.set(cv::CAP_PROP_FRAME_HEIGHT, height);
        real_time.set(cv::CAP_PROP_BUFFERSIZE, 1);
        real_time.set(cv::CAP_PROP_AUTO_EXPOSURE, 3);
        return real_time;
    }
};