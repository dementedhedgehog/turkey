#ifndef CAMERA_H
#define CAMERA_H

class Camera {
private:
    float camera_x;
    float camera_y;

public:

    Camera();
    void pan_left();
    void pan_right();
    void pan_up();
    void pan_down();


    // return the current camera position
    void get_camera_position(float * camera_x, float * camera_y) const {
        *camera_x = this->camera_x;
        *camera_y = this->camera_y;
    }
};
#endif
