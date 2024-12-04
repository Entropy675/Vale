#ifndef PHYSICSCONTROLLER__H_
#define PHYSICSCONTROLLER__H_

#include "PhysicsEntity.h"
#include "EnvironmentObject.h"

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
};

#endif

