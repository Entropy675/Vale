#include "Scene.h"


Scene::~Scene()
{
    // delete every added entity
    for (Entity* ent : entities) delete ent;
    for (Entity* ent : scenePhysicsObjects) delete ent;
}

void Scene::loadScene(std::vector<Entity*>* list)
{
    phys.loadScene(scenePhysicsObjects);
    list->clear();
    list->reserve(entities.size());
    //std::copy(sceneObjects.begin(), sceneObjects.end(), std::back_inserter(*list));
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

// standard addEntity for scene, TODO: use tag manager to return a type-based set of lists
void Scene::addEntity(PhysicsEntity* physEntity) 
{
    // keep track of current players in Scene
    if (physEntity->hasTag("player")) 
    {
        Player* playerPtr = static_cast<Player*>(physEntity);
        playersInScene.push_back(playerPtr);
        std::cout << "Added to players in scene: " << playerPtr->getPlayerName() << std::endl;
    }
    scenePhysicsObjects.push_back(physEntity);
}

void Scene::addEntity(Entity* entity) 
{
    entities.push_back(entity);
}
