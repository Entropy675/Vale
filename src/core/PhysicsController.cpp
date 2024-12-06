#include "PhysicsController.h"
#include "Camera.h"
#include "Player.h"

// maintains a list of active physics objects, cleared when the scene is changed
PhysicsController::PhysicsController(EnvironmentObject& environment)
    : Entity(glm::vec3(0, 0, 0)), env(environment)
{
    addTag("physics_controller");
    env.addTag("env");
}

PhysicsController::~PhysicsController()
{
    for (PhysicsEntity* obj : physicsObjects) delete obj; // Free memory for each existing object
}

void PhysicsController::clear()
{
    for (PhysicsEntity* obj : physicsObjects) delete obj;
    physicsObjects.clear();
}

void PhysicsController::loadScene(std::vector<PhysicsEntity*>& preservedPhysicsObjects)
{
    for (PhysicsEntity* obj : physicsObjects) delete obj; // Free memory for each existing object
    physicsObjects.clear();

    // copy each preserved object in the scene into active physicsObjects
    std::cout << "Physics loading count: " << preservedPhysicsObjects.size() << std::endl;
    for (PhysicsEntity* obj : preservedPhysicsObjects)
    {
        std::cout << "Physics loading: " << obj->getId() << std::endl;
        PhysicsEntity* clone = static_cast<PhysicsEntity*>(obj->clone());
        clone->registerInputManager(inputManager);
        clone->setup();
        physicsObjects.push_back(clone);
        std::cout << "Physics loaded active clone: " << clone->getId() << std::endl;
    }
}

void PhysicsController::collisionCheck()
{
    for (PhysicsEntity* obj : physicsObjects)
    {
        obj->collision(env);
        for (PhysicsEntity* otherObj : physicsObjects)
        {
            if (obj == otherObj) continue;
            obj->collision(*otherObj);
        }
    }
}

void PhysicsController::_setup()
{
    for (PhysicsEntity* ptr : physicsObjects) ptr->setup();
}

void PhysicsController::_update()
{
    collisionCheck(); // needs to happen before due to collision checking
    for (PhysicsEntity* ptr : physicsObjects) ptr->update();
}

void PhysicsController::_draw()
{
    for (PhysicsEntity* ptr : physicsObjects)
        ptr->draw();
}

bool PhysicsController::addCam(Camera* cam) {
    for (PhysicsEntity* ptr : physicsObjects)
    {
        if (ptr->hasTag("player")) {
            Player* playerPtr = static_cast<Player*>(ptr);
            if (playerPtr) {
                cam->setPlayer(playerPtr);
                addEntity(cam);
                return true;
                std::cerr << "Cam is set to: " << playerPtr->getPlayerName() << std::endl;
            }
        }
        std::cout << "No players found" << std::endl;
    }
    return false;
}
