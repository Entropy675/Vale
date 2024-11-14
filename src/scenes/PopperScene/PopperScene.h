#ifndef POPPERSCENE_H__
#define POPPERSCENE_H__

#include "Entity.h"
#include "PhysicsEntity.h"
#include "Scene.h"
#include <vector>


// ----------  Scene objects  ----------
class ContainerObject : public Entity
{
private:
    float radius;
    int resolution;

public:
    ContainerObject(glm::vec3 pos = glm::vec3(0, 0, 0), float radius = 1600, int resolution = 1064);
    ~ContainerObject();
    
    void _setup() override;
    void _update() override; 
    void _draw() override;
};

class BallObject : public PhysicsEntity
{
private:
    float radius;
    int resolution;

    float lastActivationTime = 0.0f; // Keeps track of the last time the popper was activated
    float interval = 1.5f;
    
public:
    BallObject(glm::vec3 pos = glm::vec3(0, 0, 0), float radius = 200, int resolution = 64);
    ~BallObject();
    
    PhysicsEntity* clone() const override; 
    void _collision(const PhysicsEntity& target) override;
    
    void _input() override; 
    
    void _setup() override;
    void _update() override; 
    void _draw() override;
};

glm::vec3 generateRandomVector(float totalDistance);

// ---------- Scene definition ----------
class PopperScene : public Scene
{
public:
    // cleanup is handled in parent 
    PopperScene() 
    {
        sceneObjects.push_back(new ContainerObject());
        for (int i = 0; i < 10; i++)
        {
            BallObject* ball = new BallObject(generateRandomVector(700));
            scenePhysicsObjects.push_back(ball);
        }
    };
};


#endif
