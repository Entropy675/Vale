#include "SceneManager.h"

SceneManager::SceneManager() : Entity(glm::vec3(0, 0, 0)), phys(aggregateMesh)
{

}

SceneManager::~SceneManager()
{
    for (Scene* sc : scenes) delete sc;
}

void SceneManager::updateEnvironmentMesh()
{
    aggregateMesh.clear();
    for (Entity* entity : entities) aggregateMesh.append(entity->getMesh());
}

void SceneManager::loadScene(int index)
{
    if (index < 0 && index >= scenes.size()) 
    {
	    std::cerr << "Error: Scene index " << index << " is out of bounds." << std::endl;
    	return;
	}
	scenes[index]->loadScene(phys, &entities);
}

void SceneManager::addScene(Scene* scene)
{
	scenes.push_back(scene);
	if (scenes.size() == 1) loadScene(0);
}

void SceneManager::_setup()
{
    phys.setup();
    for (Entity* ptr : entities) ptr->setup();
	updateEnvironmentMesh();
}

void SceneManager::_update()
{
    phys.update();
    for (Entity* ptr : entities) ptr->update();
}

void SceneManager::_draw()
{
    phys.draw();
    for (Entity* ptr : entities) ptr->draw();
}
