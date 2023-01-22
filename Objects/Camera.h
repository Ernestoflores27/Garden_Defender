#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class Camera
{
public:
    VideoCapture getVideoCapture()
    {
        VideoCapture real_time(0);                 // capturing video from default webcam//
        real_time.set(CAP_PROP_FRAME_WIDTH, 320);  // Setting the width of the video
        real_time.set(CAP_PROP_FRAME_HEIGHT, 240); // Setting the height of the video//

        return real_time;
    }
};