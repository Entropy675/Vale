#include "Camera.h"


void Camera::_setup() 
{
    camera.setPosition(startingPos);
    camera.setFov(60);
    camera.setNearClip(1.0f); 
    camera.setFarClip(31000.0f);
}

void Camera::_update() 
{
   
}

void Camera::_input() 
{
    if (!currPlayer) return;

    std::cout << "BEFORE Player: " << currPlayer->getPlayerName() << " | New Position: ("
        << currPlayer->getPosition().x << ", " << currPlayer->getPosition().y << ", " << currPlayer->getPosition().z << ")" << "At this speed:" << currPlayer->getPlayerSpeed() << std::endl;
    currPlayer->moveTo(camera.getPosition());
    std::cout << "AFTER Player: " << currPlayer->getPlayerName() << " | New Position: ("
        << camera.getPosition().x << ", " << camera.getPosition().y << ", " << camera.getPosition().z << ")" << "At this speed:" << currPlayer->getPlayerSpeed() << std::endl;

    float moveSpeed = currPlayer->getPlayerSpeed();
    if (inputManager->getHeld('w')) camera.dolly(-moveSpeed); // Move forward
    if (inputManager->getHeld('s')) camera.dolly(moveSpeed); // Move backward
    if (inputManager->getHeld('a')) camera.truck(-moveSpeed); // Move left
    if (inputManager->getHeld('d')) camera.truck(moveSpeed); // Move right'

    if (inputManager->getHeld(' ')) camera.move(0, moveSpeed, 0); // Move up (Space key)
    if (inputManager->getHeld(OF_KEY_SHIFT)) camera.boom(-moveSpeed); // Move down (Shift key)
    
    //switch Players
    if (inputManager->getPressedOnce('p')) {
        for (Player* player : *playersInScene) {
            if (player->getPlayerName() != currPlayer->getPlayerName()) {
                setPlayer(player);
                camera.setPosition(player->getPosition()); 
                std::cout << "Switching to Player: " << player->getPlayerName() << std::endl;
                break; 
            }
        }
    }

    
}

void Camera::mouseMoved(ofMouseEventArgs& mouseMovement)  {
    // Reset to a default forward-facing orientation
    camera.setGlobalOrientation(glm::quat());

    float deltaX = mouseMovement.x - ofGetWidth() / 2;
    float deltaY = mouseMovement.y - ofGetHeight() / 2;

    // Rotate the camera based on mouse movement
    float sensitivity = .35f;
    camera.panDeg(-sensitivity * 1.4 * deltaX);   // Pan (left-right rotation)
    camera.tiltDeg(-sensitivity * deltaY);   // Tilt (up-down rotation)
}

void Camera::camBegin() {
    camera.begin();
}

void Camera::camEnd() {
    camera.end();
}

bool Camera::setPlayer(Player* player ) 
{
    if (player == nullptr) return false;

    currPlayer = player;
    return true; 
}
