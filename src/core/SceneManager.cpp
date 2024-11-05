#include "SceneManager.h"

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{
    for (Scene* sc : scenes) delete sc;
}

void SceneManager::loadScene(int index)
{
    if (index < 0 && index >= scenes.size()) 
    {
	    std::cerr << "Error: Scene index " << index << " is out of bounds." << std::endl;
    	return;
	}
	scenes[index]->loadScene(&entities);
}

void SceneManager::addScene(Scene* scene)
{
	scenes.push_back(scene);
	if (scenes.size() == 1) loadScene(0);
}

void SceneManager::_setup()
{

    for (Scene* sc : scenes) sc->setup();
}

void SceneManager::_update()
{
    for (Entity* ptr : entities) ptr->update();
}

void SceneManager::_draw()
{
    for (Entity* ptr : entities) ptr->draw();
}
