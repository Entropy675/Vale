#include "Hero.h"

Entity* Hero::clone() const
{
    return new Hero(*this);
}

void Hero::_collision(PhysicsEntity& target)
{
    if (target.hasTag("island"));
}

void Hero::_input()
{
    // Input logic
}

void Hero::_setup()
{
    float radius = 600;
    int resolution = 24;

    ofSpherePrimitive tempMesh;
    tempMesh.set(radius, resolution);

    mesh.addVertices(tempMesh.getMesh().getVertices());
    mesh.addNormals(tempMesh.getMesh().getNormals());
    mesh.addTexCoords(tempMesh.getMesh().getTexCoords());
    mesh.addIndices(tempMesh.getMesh().getIndices());

}
void Hero::_update() {
    float currentFrameTime = ofGetElapsedTimef();
    deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    glm::vec3 newPosition = getPosition();

    if (cameraAssigned) {
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
    }
    setAcceleration(getAcceleration() + gravity);
    setVelocity(getVelocity() + (getAcceleration() * deltaTime));

    newPosition += getVelocity() * deltaTime;


    // hardcoded implementation for now
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


void Hero::_draw()
{
    ofSetColor(0, 0, 255);
    mesh.draw();
}
