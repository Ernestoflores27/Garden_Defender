#include "Camera.h"
/**
@brief Constructs a Camera object with the specified width and height.
@param width_ The width of the video stream.
@param height_ The height of the video stream.
*/
Camera::Camera(int width_, int height_)
{
    width = width_;
    height = height_;
}
/**

@brief Gets a video capture object with the specified width, height, and setup.
@return A cv::VideoCapture object with the specified width and height.
*/
cv::VideoCapture Camera::getVideoCapture()
{
    // Initialise camera settings
    cv::VideoCapture real_time(0, cv::CAP_V4L2);
    real_time.set(cv::CAP_PROP_FRAME_WIDTH, width);
    real_time.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    real_time.set(cv::CAP_PROP_BUFFERSIZE, 1);
    real_time.set(cv::CAP_PROP_AUTO_EXPOSURE, 3);
    return real_time;
}