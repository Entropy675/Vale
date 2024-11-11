#ifndef ENTITYMANAGER__H_
#define ENTITYMANAGER__H_

#include "Entity.h"
#include "Scene.h"
#include "PhysicsController.h"

// entity that holds active entities, and scenes that you can switch to
class SceneManager : public Entity
{
    private:
    PhysicsController phys;
    std::vector<Entity*> entities; // mem managed by scenes
    std::vector<Scene*> scenes; // mem managed by this class

    public:
    SceneManager();
    ~SceneManager();

    void loadScene(int index);
    void addScene(Scene* scene);

    void _setup() override;
    void _update() override; 
    void _draw() override;
};

#endif

