#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class Camera
{
public:
    int width, height;

    Camera(int width_, int height_)
    {
        width = width_;
        height = height_;
    }

    VideoCapture getVideoCapture()
    {
        VideoCapture real_time(0);
        real_time.set(CAP_PROP_FRAME_WIDTH, width);
        real_time.set(CAP_PROP_FRAME_HEIGHT, height);

        return real_time;
    }
};