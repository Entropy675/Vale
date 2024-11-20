#include "Camera.h"


void Camera::_setup() 
{
    camera.move(0, 400, 0);
    camera.setFov(60);
    camera.setNearClip(1.0f); 
    camera.setFarClip(31000.0f);
}
