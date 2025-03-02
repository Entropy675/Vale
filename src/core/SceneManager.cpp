#include "SceneManager.h"

SceneManager::SceneManager() : Entity(glm::vec3(0, 0, 0)), phys(aggregateMesh)
{
    addTag("scene_manager");
}
SceneManager::~SceneManager()
{
    for (Scene* sc : scenes) delete sc;
}

void SceneManager::toggleStaticMesh()
{
    drawStaticMesh = !drawStaticMesh;
}

void SceneManager::initializeEnvironmentMesh()
{
    aggregateMesh.clear();
    aggregateMesh.addMesh(scenes[currentSceneIndex]->getAggregateMesh());
    aggregateMesh.setupEnvironment();
    std::cout << "initializeEnvironmentMesh success" << std::endl;
}

void SceneManager::loadScene(size_t index)
{
    if (!scenes.size())
    {
        std::cout << "Attempted to load a scene when none exist in SceneManager." << std::endl;
        return;
    }
    if (index < 0 || index >= scenes.size())
    {
        std::cerr << "Error: Scene index " << index << " is out of bounds." << std::endl;
        return;
    }
    
    std::cout << "SCENE: " << currentSceneIndex << " -> " << index << std::endl;

    currentSceneIndex = index;
    scenes[currentSceneIndex]->setup();
    initializeEnvironmentMesh();
}

void SceneManager::next()
{
    if(scenes.size() == 0)
    {
        std::cout << "Cannot call next(), there are no scenes!" << std::endl;
        return;
    }
    loadScene((currentSceneIndex + 1) % scenes.size());
}

void SceneManager::prev()
{
    if(scenes.size() == 0)
    {
        std::cout << "Cannot call prev(), there are no scenes!" << std::endl;
        return;
    }
    loadScene((currentSceneIndex - 1 + scenes.size()) % scenes.size()); // wrap around making sure its not negative
}

void SceneManager::_setup()
{
    if (scenes.size()) loadScene(currentSceneIndex);
    else std::cout << "SceneManager setup called with no scenes added..." << std::endl; 
}

void SceneManager::_update()
{
    scenes[currentSceneIndex]->update();
}

void SceneManager::_draw()
{
    scenes[currentSceneIndex]->draw();
    //if (drawStaticMesh) aggregateMesh.getMesh().drawWireframe(); // maybe wireframe instead so you can see better...
    if (drawStaticMesh) aggregateMesh.draw();
}

void SceneManager::registerInputManager(InputManager* input)
{
    inputManager = input;
    phys.registerInputManager(input);
}
