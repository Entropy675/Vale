#include "Scene.h"


Scene::Scene() {}

Scene::~Scene()
{
    for (std::vector<Entity*>* list : allReferences)
    {
        // Remove all instances of sceneObjects from the current list
        list->erase(std::remove_if(list->begin(), list->end(),
            [this](Entity* entity)
            {    // Returns true if the entity is part of sceneObjects
                return std::find(sceneObjects.begin(), sceneObjects.end(), entity) != sceneObjects.end();
            }),
            list->end());
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
    allReferences.push_back(list); // keep a reference to where referenced
}

void Scene::setup()
{
    for (Entity* ptr : sceneObjects) ptr->setup();
}

// TODO: make a format for loading entities from a file.
bool Scene::loadSceneFromFile(std::string& path)
{
    // nothing for now, any scene can load more entities
    return false;
}

bool Scene::saveSceneToFile(std::string& path)
{
    // save current scene to a files
    return false;
}

void Scene::addEntity(PhysicsEntity* physEntity) 
{
    scenePhysicsObjects.push_back(physEntity);
}

void Scene::addEntity(Entity* entity) 
{
    sceneObjects.push_back(entity);
}