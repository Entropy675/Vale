#include "IslandScene.h"

IslandScene::IslandScene() {}


void IslandScene::loadScene(std::vector<Entity*>* list) 
{
	if (ocean == nullptr) 
	{
		ocean = new OceanObject(glm::vec3(500, 500, 10));
		ocean->setFacingRotation(glm::vec4(1, 0, 0, 90));
    }
    
    if(island == nullptr)
    {
    	island = new IslandObject();
    }
    
    list->push_back(ocean);
    list->push_back(island);
    
    allReferences.push_back(list);
}
