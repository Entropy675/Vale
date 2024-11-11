#ifndef PHYSICSCONTROLLER__H_
#define PHYSICSCONTROLLER__H_

#include "PhysicsEntity.h"

class PhysicsController : public Entity
{
    private:
    std::vector<PhysicsEntity*> physicsObjects; // mem managed by this class
    ofMesh& env;
    
    public:
    PhysicsController(ofMesh& environment);
    ~PhysicsController();

    void clear();
    void loadScene(std::vector<PhysicsEntity*>& physicsObjects);

    void _setup() override;
    void _update() override; 
    void _draw() override;
};

#endif

