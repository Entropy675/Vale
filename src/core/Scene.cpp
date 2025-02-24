#include "Scene.h"


Scene::~Scene()
{   // manages own entities
    for (Entity* ent : entities) delete ent;
}

void Scene::loadScene(std::vector<Entity*>* list)
{
    phys.loadScene(scenePhysicsObjects);
    list->clear();
    list->reserve(entities.size());
    //std::copy(sceneObjects.begin(), sceneObjects.end(), std::back_inserter(*list)); see environment object for transferring entities across scenes, TODO: theres should be a more robust way for dynamic entities
    std::cout << "Scene loading count: " << entities.size() << std::endl;
    for (Entity* ptr : entities)
    {
        Entity* clone = ptr->clone();
        list->push_back(clone);
        std::cout << "Scene loading: " << ptr->getId() << std::endl;
        std::cout << "Scene loaded active clone: " << clone->getId() << std::endl;
    }
    
    sceneLoaded = true;
}

void Scene::setup()
{
    for (Entity* ptr : entities) ptr->setup();
}

void Scene::addEntity(Entity* ent)
{
    if (!ent->hasTag("physics")) 
    {
        entities.push_back(ent);
        return;
    }
    
    // add to playersInScene. TODO: replace with generating the tag type map when that function is done
    // keep track of current players in Scene
    if (ent->hasTag("player")) 
    {
        Player* playerPtr = static_cast<Player*>(ent);
        playersInScene.push_back(playerPtr);
        std::cout << "Added to players in scene: " << playerPtr->getPlayerName() << std::endl;
    }
    
    phys.addEntity(static_cast<PhysicsEntity*>(ent));
    return;
}


void Scene::removeEntity(Entity* ent)
{
    if (!ent->hasTag("physics")) 
    {
        auto it = std::find(entities.begin(), entities.end(), ent);
        if (it != entities.end()) entities.erase(it);
        delete ent;
        return;
    }
    // Remove from playersInScene. TODO: replace with a recalculation of the tag type map when that is done
    if (playersInScene != nullptr) 
    {
        auto playerIt = std::find(playersInScene->begin(), playersInScene->end(), ent);
        if (playerIt != playersInScene->end())
            playersInScene->erase(playerIt);
    }
    phys.removeEntity(static_cast<PhysicsEntity*>(ent));
    return;
}

// TODO: make a format for loading entities from a file.
bool Scene::loadSceneFromFile(const std::string& path)
{
    // nothing for now, any scene can load more entities
    return false;
}

bool Scene::saveSceneToFile(const std::string& path)
{
    // save current scene to a file
    return false;
}

