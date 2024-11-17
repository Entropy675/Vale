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
    
    bool drawStaticMesh = true;

    public:
    SceneManager();
    ~SceneManager();

    void toggleStaticMesh();
    void updateEnvironmentMesh();
    void loadScene(size_t index);
    void addScene(Scene* scene);
    
    Entity* clone() const override { return nullptr; };
    void _setup() override;
    void _update() override; 
    void _draw() override;
    void registerInputManager(InputManager* input) override;
};

#endif

