#include "PhysicsController.h"

// the biggest difference between movement here and movement in actual object is that this does modify collision mesh
// maintains a small list of physics objects, cleared when the scene is changed
// scene can load entities into either SceneManager or PhysicsController
// only PhysicsEntities will work PhysicsController
PhysicsController::PhysicsController(ofMesh& environment)
    : Entity(glm::vec3(0, 0, 0)), env(environment) {}

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
    for (PhysicsEntity* obj : preservedPhysicsObjects) 
        physicsObjects.push_back(obj->clone());
}

void PhysicsController::_setup()
{
    for (PhysicsEntity* ptr : physicsObjects) ptr->setup();
}

void PhysicsController::_update()
{
    for (PhysicsEntity* ptr : physicsObjects) ptr->update();
}

void PhysicsController::_draw()
{
    for (PhysicsEntity* ptr : physicsObjects) ptr->draw();
}
