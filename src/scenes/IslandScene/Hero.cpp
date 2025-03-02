#include "Hero.h"
#include "Camera.h"

Entity* Hero::clone() const
{
    return new Hero(*this);
}


// called for every collision with a target, more efficient to use this then putting everything in update
void Hero::_collision(PhysicsEntity& target)
{
    //if(target.hasTag("hero")) for example collision between players
}

void Hero::_input()
{
    // Input logic (same as update just always called before update)
}

void Hero::_setup()
{
    ofSpherePrimitive tempMesh;
    tempMesh.set(radius, resolution);
    mesh = tempMesh.getMesh();
    addTag("onGround");
}

void Hero::_update()
{
    float currentFrameTime = ofGetElapsedTimef();
    deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime; // we will move towards a tick rate based approach for physics sim, with a seperate delta time based thread for draw

    glm::vec3 forward = glm::normalize(playerOrientation); // forward vector (camera facing direction)
    glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
    glm::vec3 tempVelocityBuffer = glm::vec3(0, 0, 0);
    glm::vec3 newPosition = getPosition();

    if (cameraAssigned)
    {
        // Movement input
        if (inputManager->getHeld('w'))
            newPosition += glm::vec3(forward.x, 0, forward.z) * moveSpeed * deltaTime; // Move forward on the XZ plane
        if (inputManager->getHeld('s'))
            newPosition -= glm::vec3(forward.x, 0, forward.z) * moveSpeed * deltaTime; // Move backward on the XZ plane
        if (inputManager->getHeld('a'))
            newPosition -= glm::vec3(right.x, 0, right.z) * moveSpeed * deltaTime; // Strafe left on the XZ plane
        if (inputManager->getHeld('d'))
            newPosition += glm::vec3(right.x, 0, right.z) * moveSpeed * deltaTime; // Strafe right on the XZ plane

        // double jump functionality
        if (inputManager->getPressedOnce(' ') && hasTag("onGround"))
        {
            // simulate jump
            tempVelocityBuffer = (getVelocity() + glm::vec3(0, moveSpeed * 1.0f, 0));
            removeTag("onGround");
            // if player is jumping, they could be colliding with other entities but not the island or water, but for now... clearing every jump, could replace this to only clear if its water or island
        }
    }

    setAcceleration(getAcceleration() + gravity);
    // simulate jump with velocity buffer)
    setVelocity((getVelocity() + tempVelocityBuffer) + (getAcceleration() * deltaTime));

    newPosition += getVelocity() * deltaTime;

    // std::cout << name << "moving to" << newPosition << std::endl;
    moveTo(newPosition);

}


void Hero::_draw()
{
    ofSetColor(0, 0, 255);

    /*
    float rotationAngle = atan2(playerOrientation.x, playerOrientation.z) * RAD_TO_DEG;
    static glm::vec3 previousOrientation = playerOrientation;
    if (playerOrientation != previousOrientation)
    {
        ofPushMatrix();
        ofRotateDeg(rotationAngle, 0, 1, 0);
        ofSetColor(heroColor);
        mesh.draw();
        ofPopMatrix();

        previousOrientation = playerOrientation;
    } // store a local rotation (see rotation variable in the PhysicsEntity) and modify it when needed: by default, the rotation variable always rotates the Hero to that local offset
    */
    ofSetColor(heroColor);
    mesh.draw();
}



