#include "Camera.h"


void Camera::_setup()
{
    camera.setPosition(position);
    camera.setFov(60);
    camera.setNearClip(1.0f);
    camera.setFarClip(31000.0f);
}

void Camera::_update()
{

}
void Camera::_input()
{
    if (playersInScene.size())
    {
        if (inputManager->getPressedOnce('p'))
        {
            playersInScene[currPlayer]->toggleCameraAssignment();
            currPlayer++;
            currPlayer %= playersInScene.size();
            playersInScene[currPlayer]->toggleCameraAssignment();
        }
        playersInScene[currPlayer]->setPlayerOrientation(camera.getLookAtDir());
        // third person logic
        glm::vec3 playerPos = playersInScene[currPlayer]->getPosition();

        // current player position + y offset, and then move backwards (radius + 200) pixels to get ideal third person perspective
        camera.setPosition((playerPos + getPosition()) - (800 * camera.getLookAtDir()));
    }

    else {
        if (inputManager->getHeld('w')) camera.dolly(-moveSpeed); // Move forward
        if (inputManager->getHeld('s')) camera.dolly(moveSpeed);  // Move backward
        if (inputManager->getHeld('a')) camera.truck(-moveSpeed); // Move left
        if (inputManager->getHeld('d')) camera.truck(moveSpeed);  // Move right
        if (inputManager->getHeld(' ')) camera.move(0, moveSpeed, 0); // Move Up
        if (inputManager->getHeld(OF_KEY_SHIFT)) camera.boom(-moveSpeed); // Move Down
    }
}


void Camera::_mouseMoved(ofMouseEventArgs& mouseMovement)
{
    // Reset to a default forward-facing orientation
    camera.setGlobalOrientation(glm::quat());

    float deltaX = mouseMovement.x - ofGetWidth() / 2;
    float deltaY = mouseMovement.y - ofGetHeight() / 2;

    // Rotate the camera based on mouse movement
    float sensitivity = .35f;
    camera.panDeg(-sensitivity * 1.4 * deltaX);   // Pan (left-right rotation)
    camera.tiltDeg(-sensitivity * deltaY);   // Tilt (up-down rotation)

    std::cout << "Mouse moved in camera: " << deltaX << " " << deltaY << std::endl;
    // update player orientation
    if (playersInScene.size())
        playersInScene[currPlayer]->setPlayerOrientation(camera.getLookAtDir());

}

void Camera::camBegin() {
    camera.begin();
}

void Camera::camEnd() {
    camera.end();
}

bool Camera::setPlayer(std::vector<PhysicsEntity*>* physicsObjects)
{
    bool success = false;
    currPlayer = 0;
    playersInScene.clear();
    for (PhysicsEntity* ptr : *physicsObjects)
    {
        if (!ptr) continue;
        std::cout << "adding phys id: " << ptr->getId() << std::endl;
        if (ptr->hasTag("player"))
        {
            std::cout << "is player id: " << ptr->getId() << std::endl;
            playersInScene.push_back(static_cast<Player*>(ptr)); // assumes player class if player tag
            success = true;
        }
    }
    if (success)
    {
        std::cout << "Cam is set to: " << playersInScene[currPlayer]->getPlayerName() << std::endl;
        playersInScene[currPlayer]->toggleCameraAssignment();
    }
    return success;
}


