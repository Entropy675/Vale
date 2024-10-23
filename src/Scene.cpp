#include "Scene.h"


Scene::Scene(std::vector<Entity*>* list)
{
	if (list != nullptr)
		loadScene(list);
}

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
	 // Does nothing, inherit/overwrite to add your scene objects here
}
