#ifndef PHYSICSCONTROLLER__H_
#define PHYSICSCONTROLLER__H_

#include "PhysicsEntity.h"

class EnvironmentObject : public PhysicsEntity
{
private:
    ofMesh& env;
    
public:
    EnvironmentObject(ofMesh& e) : PhysicsEntity(glm::vec3(0, 0, 0)), env(e) {};
    ~EnvironmentObject() {};
    
    PhysicsEntity* clone() const override { return new EnvironmentObject(env); }; 
    void _collision(const PhysicsEntity& target) override
    {
        // global env collision logic, runs against every entity in the scene
    };
    
    const ofMesh& getMesh() const override { return env; };
    
    // not used
    void _setup() override {};
    void _update() override {}; 
    void _draw() override {};
};

class PhysicsController : public Entity
{
    private:
    std::vector<PhysicsEntity*> physicsObjects; // mem managed by this class
    EnvironmentObject env;
    
    public:
    PhysicsController(ofMesh& environment);
    ~PhysicsController();

    void clear();
    void loadScene(std::vector<PhysicsEntity*>& physicsObjects);
    void collisionCheck();

    void _setup() override;
    void _update() override; 
    void _draw() override;
};

#endif

