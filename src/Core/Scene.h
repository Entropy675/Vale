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
	Scene();
	virtual ~Scene();

	virtual void loadScene(std::vector<Entity*>* list) = 0;
	bool loadSceneFromFile(std::string& path); // TODO not done yet...
	bool saveSceneToFile(std::string& path);
};



#endif