#include "Scene.h"


Scene::Scene() {}

Scene::~Scene()
{
	for (std::vector<Entity*>* list : allReferences)
	{
		// Remove all instances of sceneObjects from the current list
		list->erase(std::remove_if(list->begin(), list->end(),
		    [this](Entity* entity) 
		    {	// Returns true if the entity is part of sceneObjects
		        return std::find(sceneObjects.begin(), sceneObjects.end(), entity) != sceneObjects.end();
		    }), 
	    	list->end());
	}
}

void Scene::loadScene(std::vector<Entity*>* list)
{
	buildScene(list);
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
	// save current scene to a file
	return false;
}