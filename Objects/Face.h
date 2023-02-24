#include <cmath>

using namespace std;

class Face
{
public:
    int64_t x, y, h, w, center_x, center_y, offset_x, offset_y;
    int64_t distance;
    Face(Rect face, int camera_width, int camera_height)
    {
        x = face.x;
        y = face.y;
        h = y + face.height;
        w = x + face.width;

        center_x = x + (w / 2);
        center_y = y + (h / 2);

        offset_x = (camera_width / 2) - center_x;
        offset_y = (camera_height / 2) - center_y;

        distance = (offset_x*offset_x) + (offset_y *offset_y);
    }
};

bool operator<(const Face &s1, const Face &s2)
{
    return s1.distance < s2.distance;
}