#include <cmath>

using namespace std;

class Face
{
public:
    int x, y, h, w, center_x, center_y, offset_x, offset_y;
    float distance;
    Face(Rect face, int camera_width, int camera_height)
    {
        x = face.x;
        y = face.y;
        h = y + face.height;
        w = x + face.width;

        center_x = (x + w) / 2;
        center_y = (y + h) / 2;

        offset_x = (camera_width / 2) - center_x;
        offset_y = (camera_height / 2) - center_y;

        distance = sqrt(offset_x ^ 2 + offset_y ^ 2);
    }
};

bool operator<(const Face &s1, const Face &s2)
{
    return s1.distance < s2.distance;
}