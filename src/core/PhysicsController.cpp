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
    for (PhysicsEntity* obj : physicsObjects) delete obj;
}

void PhysicsController::clear()
{
    for (PhysicsEntity* obj : physicsObjects) delete obj;
    physicsObjects.clear();
}

void PhysicsController::loadScene(std::vector<PhysicsEntity*>& preservedPhysicsObjects)
{
    for (PhysicsEntity* obj : physicsObjects) delete obj;
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
        env.collision(*obj);
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
    collisionCheck();
    for (PhysicsEntity* ptr : physicsObjects) ptr->update();
}

void PhysicsController::_draw()
{
    for (PhysicsEntity* ptr : physicsObjects)
        ptr->draw();
}

void PhysicsController::addCam(Camera* cam)
{
    if (!cam) std::cout << "!! attempted to add null cam to physics" << std::endl;
    addEntity(cam);
    cam->setPlayer(&physicsObjects);
}

void PhysicsController::registerInputManager(InputManager* input)
{
    inputManager = input;
    input->setActivePhysicsEntities(&physicsObjects);
}
