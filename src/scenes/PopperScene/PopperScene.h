#ifndef POPPERSCENE_H__
#define POPPERSCENE_H__

#include "Entity.h"
#include "Scene.h"
#include <vector>


// ----------  Scene objects  ----------
class ContainerObject : public Entity
{
private:
    float radius;
    int resolution;

public:
    ContainerObject(glm::vec3 pos = glm::vec3(0, 0, 0), float radius = 2000, int resolution = 1064);
    ~ContainerObject();
    
    void _setup() override;
    void _update() override; 
    void _draw() override;
};


// ---------- Scene definition ----------
class PopperScene : public Scene
{
public:
    // cleanup is handled in parent 
    PopperScene() 
    {
        sceneObjects.push_back(new ContainerObject());
    };
};


#endif
