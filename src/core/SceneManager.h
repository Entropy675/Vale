#ifndef ENTITYMANAGER__H_
#define ENTITYMANAGER__H_

#include "Entity.h"
#include "Scene.h"
#include "PhysicsController.h"

// entity that holds active entities, and scenes that you can switch to
class SceneManager : public Entity
{
    private:
    std::vector<Scene*> scenes; // mem managed by this class
    std::vector<Entity*> entities; // mem managed by scenes
    ofMesh aggregateMesh; // Unified mesh for the entire scene
    PhysicsController phys;

    public:
    SceneManager();
    ~SceneManager();

    void updateEnvironmentMesh();
    void loadScene(int index);
    void addScene(Scene* scene);

    void _setup() override;
    void _update() override; 
    void _draw() override;
};

#endif

