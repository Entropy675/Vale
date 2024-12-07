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
                // if player is jumping, they could be colliding with other entities but not the island or water, but for now... clearing every jump, could replace this to only clear if its water or island
                collisionTag.empty();
            }
        }
    }
    setAcceleration(getAcceleration() + gravity);
    setVelocity(getVelocity() + (getAcceleration() * deltaTime));

    newPosition += getVelocity() * deltaTime;


    if (!collisionTag.empty()) {
        // collision occured, check what the entity was
        if (collisionTag == "island") {
            std::cout << name << "collided with island" << std::endl;
            isOnGround = true;
        }
    }
    // if collision, with island then do this...

    // hardcoded implementation for now
    

    moveTo(newPosition);
}


void Hero::_draw()
{
    ofSetColor(0, 0, 255);
    mesh.draw();
}
