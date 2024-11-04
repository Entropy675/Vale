#ifndef SCENE_H__
#define SCENE_H__

#include <vector>
#include "Entity.h"

class Scene
{
protected:
	std::vector<Entity*> sceneObjects;
	std::vector<std::vector<Entity*>*> allReferences;
	virtual void buildScene(std::vector<Entity*>* list) = 0;
	
public:
	Scene();
	virtual ~Scene();

	void loadScene(std::vector<Entity*>* list);
	void setup();
	bool loadSceneFromFile(std::string& path); // TODO not done yet...
	bool saveSceneToFile(std::string& path);
};



#endif