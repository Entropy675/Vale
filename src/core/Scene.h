#ifndef SCENE_H__
#define SCENE_H__

#include <vector>
#include "Entity.h"
#include "Player.h"
#include "PhysicsController.h"

class SceneManager;

class Scene // responsible for the mem of its own entities
{
protected:
    std::vector<Entity*> sceneObjects;
    std::vector<PhysicsEntity*> scenePhysicsObjects; // unaltered copy
    std::vector<Entity*>* references;

    // if 'player' tag present in entity, then add to playersInScene.
    std::vector<Player*> playersInScene;
    glm::vec3 defaultCameraOffset = glm::vec3(0.0f, 0.0f, 0.0f);

    friend class SceneManager;

public:
    Scene() {};
    virtual ~Scene();

    void setup();
    void loadScene(PhysicsController& phys, std::vector<Entity*>* list);
    bool loadSceneFromFile(const std::string& path); // TODO not done yet...
    bool saveSceneToFile(const std::string& path);

    std::vector<Player*>* getPlayersInScene() { return &playersInScene; };

    void addEntity(PhysicsEntity* physEntity);
    void addEntity(Entity* Entity);
    const glm::vec3& getDefaultCameraOffset() const { return defaultCameraOffset; };

};



#endif
