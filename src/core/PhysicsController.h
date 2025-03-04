#ifndef PHYSICSCONTROLLER__H_
#define PHYSICSCONTROLLER__H_

#include "PhysicsEntity.h"
#include "EnvironmentObject.h"
class Camera;
class Player;

// responsible for managing physics entities within a scene
class PhysicsController : public Entity
{
private:
    std::vector<PhysicsEntity*> physicsObjects; // mem managed by this class
    EnvironmentObject& env;

public:
    PhysicsController(EnvironmentObject& environment);
    ~PhysicsController();

    void clear();
    void loadScene(std::vector<PhysicsEntity*>& physicsObjects);
    void collisionCheck();

    Entity* clone() const override { return nullptr; };
    void _setup() override;
    void _update() override;
    void _draw() override;
    void registerInputManager(InputManager* input) override;

    void addCam(Camera* camEntity);
    void addEntity(PhysicsEntity* physEntity) { if (physEntity) physicsObjects.push_back(physEntity); }
    void removeEntity(PhysicsEntity* physEntity);
};

/*
TODO:
I am thinking of creating a sort of temporary swap physics controller that becomes the initial physics controller of a scene you switch to (via passing it in as a reference) this physics controller lays dormant and before a scene transition, entities tagged with "live_switch" have their pointers placed in the control of the new physics controller while being removed from the old one without being deleted. Then the old physics controller and the non live_switch entities get deleted (not the EnvironmentMesh which is a reference in any controller anyways and is actually managed by the SceneManager) along with their respective controller, we seemlessly switch to a new scene that may be adjacent to the previous one. We can even still run update steps for neighbour scenes, but at longer intervals for performance (of course the delta time will be taken into account for everything done in the scene) 
*/

#endif

