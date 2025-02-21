#ifndef SCENEMANAGER__H_
#define SCENEMANAGER__H_

#include "Entity.h"
#include "PhysicsController.h"
#include "EnvironmentObject.h"
#include "Camera.h"
#include "Scene.h"

// entity that holds active entities, and scenes that you can switch to
class SceneManager : public Entity
{
    private:
    int currentSceneIndex = 0;
    Scene* currentScene;
    std::vector<Scene*> scenes; // mem managed by this class
    
    PhysicsController phys;
    EnvironmentObject aggregateMesh; // Unified mesh for the entire scene
    Camera* cam;

    bool drawStaticMesh = false;

    public:
    SceneManager();
    ~SceneManager();

    void toggleStaticMesh();
    void updateEnvironmentMesh();
    void loadScene(size_t index);
    void addScene(Scene* scene);

    // use this to change scenes in order (if you don't want to specify the # with loadScene)
    void next();
    void prev();

    Entity* clone() const override { return nullptr; };
    void _setup() override;
    void _update() override;
    void _draw() override;
    void registerInputManager(InputManager* input) override;


    void mouseMoved(ofMouseEventArgs& mouse);

};

#endif

