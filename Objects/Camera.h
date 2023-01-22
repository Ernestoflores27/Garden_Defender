#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class Camera
{
public:
    VideoCapture getVideoCapture()
    {
        VideoCapture real_time(0);
        real_time.set(CAP_PROP_FRAME_WIDTH, 640);
        real_time.set(CAP_PROP_FRAME_HEIGHT, 480);

        return real_time;
    }
};