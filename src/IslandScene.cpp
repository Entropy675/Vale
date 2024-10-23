#include "IslandScene.h"

IslandScene::IslandScene(std::vector<Entity*>* list) : Scene(list) {}


void IslandScene::loadScene(std::vector<Entity*>* list) 
{
	if (ocean == nullptr) 
	{
		ocean = new OceanObject(glm::vec3(400, 400, 10));
		ocean->setFacingRotation(glm::vec4(1, 0, 0, 90));
    }
    list->push_back(ocean);
    
    if(island == nullptr) island = new IslandObject();
    list->push_back(island);
    
    allReferences.push_back(list);
}
