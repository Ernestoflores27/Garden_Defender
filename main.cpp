#include "Objects/Camera.cpp"
// #include "Objects/Detector.cpp"
#include "Objects/Turret.cpp"
#include <opencv4/opencv2/opencv.hpp>
#include <unistd.h>

int main()
{
        //! Declare variable video stream (type: matrix from OpenCV library)
        cv::Mat video_stream;
        Camera cam1(640, 480);
        cv::VideoCapture real_time = cam1.getVideoCapture();

        Detector detector_model("Garden_Defender/yoloFastestV2.onnx", 0.3, 0.4, 0.4, real_time);
        detector_model.detectT();

        Turret turret(23, 24, 25, &detector_model);
        turret.changePosition(0, 0);
        turret.moveT();

        do
        {
                std::cout << '\n'
                          << "Press a Enter to close...";

        } while (std::cin.get() != '\n');

        real_time.release();
        turret.release();
}
