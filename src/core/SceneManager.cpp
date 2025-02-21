#include "SceneManager.h"

SceneManager::SceneManager() : Entity(glm::vec3(0, 0, 0)), phys(aggregateMesh)
{
    addTag("scene_manager");
}
SceneManager::~SceneManager()
{
    if (currentScene) delete currentScene;
    for (Scene* sc : scenes) delete sc;
}

void SceneManager::toggleStaticMesh()
{
    drawStaticMesh = !drawStaticMesh;
}

void SceneManager::updateEnvironmentMesh()
{
    aggregateMesh.clear();
    for (Entity* entity : entities)
    {
        ofMesh entityMesh = entity->getMesh();

        // Get individual transformations
        glm::vec3 scale = entity->getScale();
        ofQuaternion rotation = entity->getRotation();
        glm::vec3 translation = entity->getTranslation();

        if (entity->getMesh().getNumVertices() == 0) continue;
        // Transform each vertex in the entity's mesh
        for (size_t i = 0; i < entityMesh.getNumVertices(); i++)
        {
            ofVec3f vertex = entityMesh.getVertex(i);

            // Scale
            vertex *= scale;
            // Rotate
            ofVec3f transformedVertex = rotation * vertex;
            // Translate
            transformedVertex += translation;

            entityMesh.setVertex(i, ofVec3f(transformedVertex.x, transformedVertex.y, transformedVertex.z)); // Update the vertex
            aggregateMesh.addVertex(entityMesh.getVertex(i), entity->getId());
        }

        // Append the transformed mesh to the aggregate mesh
        // appended each vertex individually above
       // aggregateMesh.addVertex(entityMesh);
        std::cout << "Appended " << entityMesh.getNumVertices() << " vertices from entity." << std::endl;

    }
    aggregateMesh.setupEnvironment();
    std::cout << "updateEnvironmentMesh success" << std::endl;
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

    phys.clear();

    //cam = new Camera(scenes[currentSceneIndex]->getDefaultCameraOffset());
    //cam->registerInputManager(inputManager);
    //cam->setup();

    scenes[currentSceneIndex]->loadScene(phys, &entities);
    currentSceneIndex = index;
    
    std::cout << "adding phys cam... " << std::endl;
    phys.addCam(cam);
    _setup();
}

void SceneManager::addScene(Scene* scene)
{
    scenes.push_back(scene);
    if (scenes.size() == 1) loadScene(0);
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
    phys.setup();
    for (Entity* ptr : entities)
    {
        ptr->registerInputManager(inputManager);
        ptr->setup();
    }
    updateEnvironmentMesh();
}

void SceneManager::_update()
{
    phys.update();
    for (Entity* ptr : entities) ptr->update();
}

void SceneManager::_draw()
{
    cam->camBegin();
    phys.draw();
    for (Entity* ptr : entities) ptr->draw();
    if (drawStaticMesh) aggregateMesh.getMesh().drawWireframe();
    cam->camEnd();
}

void SceneManager::registerInputManager(InputManager* input)
{
    inputManager = input;
    phys.registerInputManager(input);
    input->setActiveEntities(&entities);
}
