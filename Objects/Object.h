/**
 * @brief A Class about a detection
 * 
 */
class Object 
{
public:
    int64_t x, y, h, w, center_x, center_y, offset_x, offset_y;
    int64_t distance;
    Object(cv::Rect object, int camera_width, int camera_height);
};

/**
 * @brief Construct a new Object:: Object object()
 * 
 * @param object 
 * @param camera_width 
 * @param camera_height 
 */