#ifndef SCENE_H__
#define SCENE_H__

#include <vector>
#include "Entity.h"
#include "Player.h"
#include "Camera.h"
#include "PhysicsController.h"

class Scene : public Entity // responsible for the mem of its own entities, physics entities are managed by its PhysicsController
{
private:
    bool sceneLoaded = false;

protected:
    std::vector<Entity*> entities;
    PhysicsController phys;
    
public:
    Scene(EnvironmentObject& env) : phys(env) {};
    virtual ~Scene();
    
    // if 'player' tag present in entity, then add to playersInScene.
    // this will eventually be replaced by a list of type sorted pointers returned by tag manager
    std::vector<Player*> playersInScene = {};
    std::vector<Camera*> camsInScene = {};
    std::vector<PhysicsEntity*> camTargetsInScene = {};
    Camera* getCurrentCam() { return camsInScene[currentCam]; };
    int currentCam = 0;
    //std::vector<supertype*>* someSuperTypeList
    // -> will be replaced with map: tags -> std:vector<Entity*>
    // far better map... or simple list of tuples (tag, std::vector<Entity*>) also possible for low numbers (performance)
    // std::list<std::tuple<std::string, std::vector<Entity*>>> tagEntityList;
    
    Entity* clone() const override { return nullptr; }; // TODO implement this here and in the PhysicsController
    
    // These functions are for internal use, all registered entities are automatically drawn/updated/setup.
    void setup() override;
    void update() override;
    void draw() override;

    // you can hook on these events optionally by implementing these functions for non entity related behaviour
    void _setup() override {};
    void _update() override {};
    void _draw() override {};

    ofMesh getAggregateMesh();
    
    void loadScene();
    bool loadSceneFromFile(const std::string& path); // TODO not done yet...
    bool saveSceneToFile(const std::string& path);

    const std::vector<Player*>& getPlayersInScene() { return playersInScene; };
    const std::vector<Entity*>& getEntities() { return entities; };

    void addEntity(Entity* Entity);
    void removeEntity(Entity* ent);

    template<typename T>
    std::vector<T*> getEntitiesWithTag(const std::string& tag);
};



#endif
