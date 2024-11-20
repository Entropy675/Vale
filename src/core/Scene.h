#ifndef SCENE_H__
#define SCENE_H__

#include <vector>
#include "Entity.h"
#include "PhysicsController.h"
#include "Player.h"

class Scene // responsible for the mem of its own entities
{
protected:
    std::vector<Entity*> sceneObjects;
    std::vector<PhysicsEntity*> scenePhysicsObjects; // unaltered copy
    std::vector<std::vector<Entity*>*> allReferences;
    // should eventually handle seperately as well as part of sceneObjects
    // std::vector<Player*> playersInScene;

public:
    Scene();
    virtual ~Scene();

    void setup();
    void loadScene(PhysicsController& phys, std::vector<Entity*>* list);
    bool loadSceneFromFile(std::string& path); // TODO not done yet...
    bool saveSceneToFile(std::string& path);
};



#endif