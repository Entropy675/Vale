#include "Player.h"
void Player::_update() {
    float currentFrameTime = ofGetElapsedTimef();
    deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    deltaTime = std::min(deltaTime, 0.05f);

    glm::vec3 newPosition = getPosition();

    if (inputManager->getHeld('w')) {
        newPosition += glm::vec3(0, 0, -moveSpeed) * deltaTime;
    }
    if (inputManager->getHeld('s')) {
        newPosition += glm::vec3(0, 0, moveSpeed) * deltaTime;
    }
    if (inputManager->getHeld('a')) {
        newPosition += glm::vec3(-moveSpeed, 0, 0) * deltaTime;
    }
    if (inputManager->getHeld('d')) {
        newPosition += glm::vec3(moveSpeed, 0, 0) * deltaTime;
    }
    if (isOnGround) {
        if (inputManager->getPressedOnce(' ')) {
            // simulate jump
            setVelocity(getVelocity() + glm::vec3(0, moveSpeed * 10, 0));
            isOnGround = false;
        }
    }
    setAcceleration(getAcceleration() + gravity);
    setVelocity(getVelocity() + (getAcceleration() * deltaTime));

    newPosition += getVelocity() * deltaTime;
    float distanceToIsland = glm::length(newPosition);

    if (distanceToIsland < 1000) {
        glm::vec3 normal = glm::normalize(newPosition);
        newPosition = normal * 1000;
        setVelocity(glm::vec3(0, 0, 0));
        setAcceleration(glm::vec3(0, 0, 0));
        isOnGround = true; 
    }

    moveTo(newPosition);
}
