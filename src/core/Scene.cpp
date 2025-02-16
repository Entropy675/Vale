#include "Scene.h"


Scene::~Scene()
{

    if (references)
    {
        // Remove all instances of sceneObjects from the current list
        references->erase(std::remove_if(references->begin(), references->end(),
            [this](Entity* entity) {// Returns true if the entity is part of sceneObjects
                return std::find(sceneObjects.begin(), sceneObjects.end(), entity) != sceneObjects.end();
            }),
            references->end()
        );
    }
    // delete every added entity
    for (Entity* ent : sceneObjects) delete ent;
    for (Entity* ent : scenePhysicsObjects) delete ent;
}

void Scene::loadScene(PhysicsController& phys, std::vector<Entity*>* list)
{
    phys.loadScene(scenePhysicsObjects);
    list->clear();
    list->reserve(sceneObjects.size());
    //std::copy(sceneObjects.begin(), sceneObjects.end(), std::back_inserter(*list));
    std::cout << "Scene loading count: " << sceneObjects.size() << std::endl;
    for (Entity* ptr : sceneObjects)
    {
        Entity* clone = ptr->clone();
        list->push_back(clone);
        std::cout << "Scene loading: " << ptr->getId() << std::endl;
        std::cout << "Scene loaded active clone: " << clone->getId() << std::endl;
    }
    references = list; // keep a reference to where referenced
}

void Scene::setup()
{
    for (Entity* ptr : sceneObjects) ptr->setup();
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

void Scene::addEntity(PhysicsEntity* physEntity) 
{
    // keep track of current players in Scene
    if (physEntity->hasTag("player")) {
        Player* playerPtr = static_cast<Player*>(physEntity);
        playersInScene.push_back(playerPtr);
        std::cout << "Added to players in scene: " << playerPtr->getPlayerName() << std::endl;
    }
    scenePhysicsObjects.push_back(physEntity);
}

void Scene::addEntity(Entity* entity) 
{
    sceneObjects.push_back(entity);
}
