#ifndef SCENE_H__
#define SCENE_H__

#include <vector>
#include "Entity.h"

class Scene
{
protected:
	std::vector<Entity*> sceneObjects;
	std::vector<std::vector<Entity*>*> allReferences;
	
public:
	Scene(std::vector<Entity*>* list = nullptr);
	virtual ~Scene();

	virtual void loadScene(std::vector<Entity*>* list);
	bool loadSceneFromFile(std::string& path);
};



#endif