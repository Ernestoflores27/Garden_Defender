#include "Objects/Camera.h"
#include "Objects/Detector.h"
#include "Objects/Turret.h"
#include <opencv4/opencv2/opencv.hpp>
#include <unistd.h>

using namespace std;

using namespace cv;
using namespace cv::dnn;

#define MARGIN 15

int main()
{

    Mat video_stream;
    Camera cam1(640, 480);
    VideoCapture real_time = cam1.getVideoCapture();

    yolo_fast yolo_model("Garden_Defender/yolov5n.onnx", 0.3, 0.3, 0.4); //     vector<Face> faces;
    vector<Face> faces;

    Turret turret(23);
    turret.changePosition(0, 0);

    while (true)
    {
        real_time.read(video_stream);
        faces = yolo_model.detect(video_stream);

        if (!yolo_model.faces_vector.empty())
        {
            // detector.lineClosest();
            if (yolo_model.getOffsetX() > MARGIN)
            {
                turret.movePitch((yolo_model.getOffsetX() * 4 / 320) + 1);
            }
            else if (yolo_model.getOffsetX() < -MARGIN)
            {
                turret.movePitch(((yolo_model.getOffsetX() * 4 / 320) - 1));
            }

            if (yolo_model.getOffsetY() > MARGIN)
            {
                turret.moveYaw(-1);
            }
            else if (yolo_model.getOffsetY() < -MARGIN)
            {
                turret.moveYaw(1);
            }

            if (yolo_model.getOffsetY() < MARGIN and yolo_model.getOffsetY() > -MARGIN and yolo_model.getOffsetX() < MARGIN and yolo_model.getOffsetX() > -MARGIN)
            {
                // turret.shoot();
                // yolo_model.showShooting();
            }
        }

        static const string kWinName = "Deep learning object detection in OpenCV";
        namedWindow(kWinName);
        imshow(kWinName, video_stream);
        waitKey(1);
        // yolo_model.drawBoundaries();

        // detector.drawCrossair();
        // yolo_model.Show();

        int k = waitKey(1);
        if (k == 27)
        {
            break;
        }
        else if (k == 82)
        {
            turret.moveYaw(-5);
        }
        else if (k == 84)
        {
            turret.moveYaw(5);
        }
        else if (k == 81)
        {
            turret.movePitch(5);
        }
        else if (k == 83)
        {
            turret.movePitch(-5);
        }
        else
        {
        }
    }
}

// int main()
// {
//     yolo_fast yolo_model("Garden_Defender/yolov5n.onnx", 0.3, 0.3, 0.4);

//     Mat video_stream;
//     vector<Face> faces;

//     Camera cam1(320, 240);
//     VideoCapture real_time = cam1.getVideoCapture();

//     while (true)
//     {
//         real_time.read(video_stream);
//         faces = yolo_model.detect(video_stream);

//         static const string kWinName = "Deep learning object detection in OpenCV";
//         namedWindow(kWinName, WINDOW_NORMAL);
//         imshow(kWinName, video_stream);
//         waitKey(1);
//     }
//     destroyAllWindows();
// }