#include "Objects/Camera.cpp"
#include "Objects/Turret.cpp"
#include "Objects/GUI.h"
#include <opencv4/opencv2/opencv.hpp>
#include <unistd.h>
#include <iostream>

class Callback : public Detector::Callback
{
public:
        Callback()
        {
        }
        void callback_func(std::vector<Object> &objs_vector)
        {
                std::cout << "callback";
        }
};

int main()
{
        //! Declare variable video stream (type: matrix from OpenCV library)
        cv::Mat video_stream;
        Camera cam1(640, 480);
        cv::VideoCapture real_time = cam1.getVideoCapture();

        Callback ca;
        Detector detector_model("Garden_Defender/yoloFastestV2.onnx", 0.3, 0.4, 0.4, real_time);
        detector_model.registerCallback(&ca);
        detector_model.detectT();

        GUI gui(real_time, &detector_model);
        gui.showT();

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
