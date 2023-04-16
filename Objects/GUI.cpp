#include "GUI.h"

GUI::GUI(Detector *detector_model_, Turret *turret_)
{
    this->turret = turret_;
    this->detector_model = detector_model_;
}

/**
@brief Method to draw a crosshair in the center of a given frame for tracking.
*/
void GUI::drawCrossair(cv::Mat &frame)
{
    cv::Size s = frame.size();
    cv::line(frame, cv::Point(s.width / 2, 0), cv::Point(s.width / 2, s.height), cv::Scalar(0, 255, 0), 1);
    cv::line(frame, cv::Point(0, s.height / 2), cv::Point(s.width, s.height / 2), cv::Scalar(0, 255, 0), 1);
}

/**
@brief Method draws a green line between the center of the detected object and the center of the video frame for tracking.
*/
void GUI::lineClosest(cv::Mat &frame)
{
    cv::Size s = frame.size();
    if (detector_model->objs_vector.empty())
        return;

    line(frame, cv::Point(s.width / 2, s.height / 2), cv::Point(detector_model->objs_vector[0].center_x, detector_model->objs_vector[0].center_y), cv::Scalar(0, 255, 0), 2);
}

/**
@brief Method for showing the frame with detected objects and the crosshair on the turret.
*/
void GUI::showShooting()
{
    putText(this->frame, "Shooting", cv::Point(detector_model->objs_vector[0].x, detector_model->objs_vector[0].y + 25), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
}

/**
@brief Method for showing the frame with detected objects and the information to command the turret in manual or automatic mode. It also command the turrent when it is in Manual mode.
*/
void GUI::start()
{
    running = true;
    while (running)
    {
        frame = detector_model->frame;
        if (!frame.empty())
        {
            this->drawCrossair(frame);
            this->lineClosest(frame);
            if (!detector_model->objs_vector.empty())
            {
                for (Object object : detector_model->objs_vector)
                {
                    circle(frame, cv::Point(object.center_x, object.center_y), 5, cv::Scalar(255, 0, 0), -1);
                    circle(frame, cv::Point(object.center_x, object.center_y), 30, cv::Scalar(255, 0, 0));
                }
            }

            if (turret->shooting)
            {
                putText(this->frame, "Shooting", cv::Point(detector_model->objs_vector[0].center_x, detector_model->objs_vector[0].center_y - 40), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
            }

            if (!turret->manual)
            {
                putText(this->frame, "Mode: Auto", cv::Point(10, 20), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
                putText(this->frame, "To change modes press Q", cv::Point(10, 40), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
            }
            else
            {
                putText(this->frame, "Mode: Manual", cv::Point(10, 20), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
                putText(this->frame, "To change modes press Q", cv::Point(10, 40), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
                putText(this->frame, "To move use arrow keys", cv::Point(10, 60), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
                putText(this->frame, "To shoot press the space key", cv::Point(10, 80), cv::FONT_HERSHEY_DUPLEX, 0.75, cv::Scalar(0, 0, 255), 1.5);
            }

            static const std::string kWinName = "Garden Defender";
            cv::namedWindow(kWinName);
            cv::imshow(kWinName, frame);
            int k = cv::waitKey(100);

            if (k != 27 && nullptr != this->callback)
            {
                this->callback->callback_func(k);
            }
            else if (k == 27)
            {
                running = false;
            }
        }
    }
}