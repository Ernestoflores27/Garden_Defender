#include "Objects/Camera.h"
#include "Objects/Detector.h"
#include "Objects/Turret.h"
#include <opencv4/opencv2/opencv.hpp>
#include <unistd.h>

using namespace std;

using namespace cv;
using namespace cv::dnn;

// #define MARGIN 40

// int main()
// {
//     Mat video_stream;

//     Camera cam1(320, 240);
//     VideoCapture real_time = cam1.getVideoCapture();

//     Detector detector(real_time);
//     vector<Face> faces;

//     Turret turret(23);
//     turret.changePosition(0, 0);

//     while (true)
//     {

//         if (!detector.faces_vector.empty())
//         {
//             // detector.lineClosest();
//             if (detector.getOffsetX() > MARGIN)
//             {
//                 turret.movePitch(5);
//             }
//             else if (detector.getOffsetX() < -MARGIN)
//             {
//                 turret.movePitch(-5);
//             }

//             if (detector.getOffsetY() > MARGIN)
//             {
//                 turret.moveYaw(-3);
//             }
//             else if (detector.getOffsetY() < -MARGIN)
//             {
//                 turret.moveYaw(3);
//             }

//             if (detector.getOffsetY() < MARGIN and detector.getOffsetY() > -MARGIN and detector.getOffsetX() < MARGIN and detector.getOffsetX() > -MARGIN)
//             {
//                 // turret.shoot();
//                 detector.showShooting();
//             }
//         }

//         faces = detector.Detect();
//         detector.drawBoundaries();

//         // detector.drawCrossair();
//         detector.Show();

//         int k = waitKey(1);
//         if (k == 27)
//         {
//             break;
//         }
//         else if (k == 82)
//         {
//             turret.moveYaw(-5);
//         }
//         else if (k == 84)
//         {
//             turret.moveYaw(5);
//         }
//         else if (k == 81)
//         {
//             turret.movePitch(5);
//         }
//         else if (k == 83)
//         {
//             turret.movePitch(-5);
//         }
//         else
//         {
//         }
//     }
// }

int main()
{
    yolo_fast yolo_model("Garden_Defender/yolov5n.onnx", 0.3, 0.3, 0.4);

    Mat video_stream;
    vector<Face> faces;

    Camera cam1(320, 240);
    VideoCapture real_time = cam1.getVideoCapture();

    while (true)
    {
        real_time.read(video_stream);
        faces = yolo_model.detect(video_stream);

        static const string kWinName = "Deep learning object detection in OpenCV";
        namedWindow(kWinName, WINDOW_NORMAL);
        imshow(kWinName, video_stream);
        waitKey(1);
    }
    destroyAllWindows();
}