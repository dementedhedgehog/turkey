
#include "model/camera.h"

const float CAMERA_PAN_X_AMOUNT = 10;
const float CAMERA_PAN_Y_AMOUNT = 10;

Camera::Camera() {
    camera_x = 50.0;
    camera_y = 150.0;
}

void Camera::pan_left() {
    camera_x -= CAMERA_PAN_X_AMOUNT;
}

void Camera::pan_right() {
    camera_x += CAMERA_PAN_X_AMOUNT;
}

void Camera::pan_up() {
    camera_y -= CAMERA_PAN_Y_AMOUNT;
}

void Camera::pan_down() {
    camera_y += CAMERA_PAN_Y_AMOUNT;
}

