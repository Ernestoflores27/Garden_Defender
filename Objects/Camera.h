/**

@file
@brief This file defines a Camera class that allows setting up a camera and obtaining a video stream.
*/
#include <opencv4/opencv2/opencv.hpp>
/**

@brief A Camera class for obtaining video streams.
*/
class Camera
{
public:
    int width, height;
    /**
@brief Constructs a Camera object with the specified width and height.
@param width_ The width of the video stream.
@param height_ The height of the video stream.
    */
    Camera(int width_, int height_)
    {
        width = width_;
        height = height_;
    }

/**

@brief Gets a video capture object with the specified width and height.
@return A cv::VideoCapture object with the specified width and height.
*/
    cv::VideoCapture getVideoCapture()
    {
        //Initialise camera settings
        cv::VideoCapture real_time(0, cv::CAP_V4L2);
        real_time.set(cv::CAP_PROP_FRAME_WIDTH, width);
        real_time.set(cv::CAP_PROP_FRAME_HEIGHT, height);
        real_time.set(cv::CAP_PROP_BUFFERSIZE, 1);
        // real_time.set(cv::CAP_PROP_AUTO_EXPOSURE, 3);
        return real_time;
    }
};