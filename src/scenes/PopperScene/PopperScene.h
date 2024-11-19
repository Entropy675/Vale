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
    ContainerObject(glm::vec3 pos = glm::vec3(0, 0, 0), float radius = 1800, int resolution = 864);
    ~ContainerObject();

    Entity* clone() const override;
    void _setup() override;
    void _update() override;
    void _draw() override;
};

class BallObject : public PhysicsEntity
{
private:
    float radius;
    int resolution;
    ofMaterial material2;

    float distance = 600;
    float lastActivationTime = 0.0f; // Keeps track of the last time the popper was activated
    float interval = 42.5f;
    float threshold = 7.1f;

public:
    BallObject(glm::vec3 pos = glm::vec3(0), float radius = 200, int resolution = 64);
    ~BallObject();

    Entity* clone() const override;
    void _collision(PhysicsEntity& target) override;

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
        for (int i = 0; i < 12; i++)
        {
            BallObject* ball = new BallObject(generateRandomVector(600));
            scenePhysicsObjects.push_back(ball);
        }
    };
};


#endif
