#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

class Camera
{
public:
    int run()
    {
        Mat image;
        namedWindow("Display window");
        VideoCapture cap(0);

        if (!cap.isOpened())
        {
            cout << "cannot open camera";
        }

        while (true)
        {
            cap >> image;
            imshow("Display window", image);

            char c = (char)waitKey(25);
            if (c == 27)
            { // If 'Esc' is entered break the loop//
                break;
            }
        }
        return 0;
    }
};