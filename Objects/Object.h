/**
 * @brief A Class to perform the algorithm that arranges the detected objects by distance.
 * 
 */
class Object 
{
public:
    int64_t x, y, h, w, center_x, center_y, offset_x, offset_y;
    int64_t distance;
    Object(cv::Rect object, int camera_width, int camera_height);
};