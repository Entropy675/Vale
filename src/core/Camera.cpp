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
    float moveSpeed = 10; // some default move to header
    if (currPlayer) moveSpeed = currPlayer->getPlayerSpeed();
    if (inputManager->getHeld('w')) camera.dolly(-moveSpeed); // Move forward
    if (inputManager->getHeld('s')) camera.dolly(moveSpeed); // Move backward
    if (inputManager->getHeld('a')) camera.truck(-moveSpeed); // Move left
    if (inputManager->getHeld('d')) camera.truck(moveSpeed); // Move right'

    if (inputManager->getHeld(' ')) camera.move(0, moveSpeed, 0); // Move up (Space key)
    if (inputManager->getHeld(OF_KEY_SHIFT)) camera.boom(-moveSpeed); // Move down (Shift key)

    if (!currPlayer) return; // the rest of the func is if we have a player/players

    currPlayer->moveTo(camera.getPosition());

    //switch Players
    if (inputManager->getPressedOnce('p'))
    {
        for (Player* player : playersInScene)
        {
            if (player->getPlayerName() != currPlayer->getPlayerName())
            {
                currPlayer = player;
                camera.setPosition(player->getPosition());
                std::cout << "Switching to Player: " << player->getPlayerName() << std::endl;
                break;
            }
        }
    }
}

void Camera::mouseMoved(ofMouseEventArgs& mouseMovement)
{
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

bool Camera::setPlayer(std::vector<PhysicsEntity*>* physicsObjects)
{
    bool success = false;
    playersInScene.clear();

    for (PhysicsEntity* ptr : *physicsObjects)
    {
        if (!ptr) continue;
        std::cout << "adding phys id: " << ptr->getId() << std::endl;
        if (ptr->hasTag("player"))
        {
            std::cout << "is player id: " << ptr->getId() << std::endl;
            currPlayer = static_cast<Player*>(ptr); // assumes player class if player tag
            playersInScene.push_back(currPlayer);
            success = true;
        }
    }

    if (success) std::cout << "Cam is set to: " << currPlayer->getPlayerName() << std::endl;
    return success;
}

