#ifndef SCENE_H__
#define SCENE_H__

#include <vector>
#include "Entity.h"
#include "Player.h"
#include "PhysicsController.h"

class Scene // responsible for the mem of its own entities
{
private:
    bool sceneLoaded = false;

protected:
    std::vector<Entity*> entities;
    PhysicsController phys; // std::vector<PhysicsEntity*> scenePhysicsObjects; // unaltered copy

public:
    Scene() {};
    virtual ~Scene();
    
    // if 'player' tag present in entity, then add to playersInScene.
    // this will be replaced by a list of type sorted pointers returned by tag manager
    std::vector<Player*>* playersInScene;
    //std::vector<supertype*>* someSuperTypeList
    // -> will be replaced with map: tags -> std:vector<Entity*>
    // far better map... or simple list of tuples (tag, std::vector<Entity*>) also possible
    // std::list<std::tuple<std::string, std::vector<Entity*>>> tagEntityList;
    
    void setup();
    void loadScene();
    bool loadSceneFromFile(const std::string& path); // TODO not done yet...
    bool saveSceneToFile(const std::string& path);

    std::vector<Player*>* getPlayersInScene() { return &playersInScene; };

    void addEntity(PhysicsEntity* physEntity);
    void addEntity(Entity* Entity);
};



#endif
