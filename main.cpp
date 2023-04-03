#include "Objects/Camera.h"
#include "Objects/Detector.h"
#include <opencv4/opencv2/opencv.hpp>
#include <unistd.h>

int main()
{
        //! Declare variable video stream (type: matrix from OpenCV library)
        cv::Mat video_stream;
        Camera cam1(640, 480);
        cv::VideoCapture real_time = cam1.getVideoCapture();

        Turret turret(23, 24, 25);
        turret.changePosition(0, 0);
        turret.moveT();

        Detector detector_model("Garden_Defender/yoloFastestV2.onnx", 0.3, 0.4, 0.4, real_time, &turret);
        detector_model.detectT();

        do
        {
                std::cout << '\n'
                          << "Press a Enter to close...";

        } while (std::cin.get() != '\n');

        real_time.release();
        turret.release();
}
