#include "Hero.h"
#include "Camera.h" 

Entity* Hero::clone() const
{
    return new Hero(*this);
}

void Hero::_collision(PhysicsEntity& target)
{
}

void Hero::_input()
{
    // Input logic - handled by player 
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

    glm::vec3 forward = glm::normalize(playerOrientation); // forward vector (camera facing direction)

    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
    glm::vec3 up(0, 1, 0);

    glm::vec3 tempVelocityBuffer = glm::vec3(0, 0, 0);

    if (cameraAssigned) {
        // Movement input
        if (inputManager->getHeld('w')) {
            newPosition += glm::vec3(forward.x, 0, forward.z) * moveSpeed * deltaTime; // Move forward on the XZ plane
        }
        if (inputManager->getHeld('s')) {
            newPosition -= glm::vec3(forward.x, 0, forward.z) * moveSpeed * deltaTime; // Move backward on the XZ plane
        }
        if (inputManager->getHeld('a')) {
            newPosition -= glm::vec3(right.x, 0, right.z) * moveSpeed * deltaTime; // Strafe left on the XZ plane
        }
        if (inputManager->getHeld('d')) {
            newPosition += glm::vec3(right.x, 0, right.z) * moveSpeed * deltaTime; // Strafe right on the XZ plane
        }

        if (isOnGround) {
            if (inputManager->getPressedOnce(' ')) {
                // simulate jump
                tempVelocityBuffer = (getVelocity() + glm::vec3(0, moveSpeed, 0));
                isOnGround = false;
                // if player is jumping, they could be colliding with other entities but not the island or water, but for now... clearing every jump, could replace this to only clear if its water or island
            }
        }
    }

    setAcceleration(getAcceleration() + gravity);
    // simulate jump with velocity buffer)
    setVelocity((getVelocity() + tempVelocityBuffer) + (getAcceleration() * deltaTime));

    newPosition += getVelocity() * deltaTime;
    
    std::cout << name << "moving too" << newPosition << std::endl;
    moveTo(newPosition);

    if (!collisionTag.empty()) {
        // collision occured, check what the entity was
        if (collisionTag == "island") {
            std::cout << name << "collided with island" << std::endl;
            isOnGround = true;
        }
    }
}


void Hero::_draw() {
    ofSetColor(0, 0, 255);

    glm::vec3 playerPos = getPosition();
    float rotationAngle = atan2(playerOrientation.x, playerOrientation.z) * RAD_TO_DEG;

    static glm::vec3 previousOrientation = playerOrientation;

    if (playerOrientation != previousOrientation) {
        ofPushMatrix();
        ofRotateDeg(rotationAngle, 0, 1, 0);
        mesh.draw();
        ofPopMatrix();

        previousOrientation = playerOrientation;
    }
    else {
        ofPushMatrix();
        mesh.draw();
        ofPopMatrix();
    }
}



