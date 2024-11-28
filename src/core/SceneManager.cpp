#include "SceneManager.h"

SceneManager::SceneManager() : Entity(glm::vec3(0, 0, 0)), phys(aggregateMesh)
{
    addTag("scene_manager");
}
SceneManager::~SceneManager()
{
    for (Scene* sc : scenes) delete sc;
    for (Entity* ptr : entities) delete ptr;
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
        std::cout << "Entity mesh vertex count BEFORE: " << entityMesh.getNumVertices() << std::endl;
        // Transform each vertex in the entity's mesh
        for (size_t i = 0; i < entityMesh.getNumVertices(); i++)
        {

            ofVec3f vertex = entityMesh.getVertex(i);

            // Scale
            vertex.x *= scale.x;
            vertex.y *= scale.y;
            vertex.z *= scale.z;

            // Rotate
            ofVec3f transformedVertex = rotation * vertex;

            // Translate
            transformedVertex.x += translation.x;
            transformedVertex.y += translation.y;
            transformedVertex.z += translation.z;

            entityMesh.setVertex(i, ofVec3f(transformedVertex.x, transformedVertex.y, transformedVertex.z)); // Update the vertex
            aggregateMesh.addVertex(entityMesh.getVertex(i), entity->getId());
        }

        // Append the transformed mesh to the aggregate mesh
        // appended each vertex individually above
       // aggregateMesh.addVertex(entityMesh);
        std::cout << "Appended " << entityMesh.getNumVertices() << " vertices from entity." << std::endl;

    }

    std::cout << "updateEnvironmentMesh success" << std::endl;
}

void SceneManager::loadScene(size_t index)
{
    std::cout << "SCENE: " << currentScene << " -> " << index << std::endl;
    if (index < 0 || index >= scenes.size())
    {
        std::cerr << "Error: Scene index " << index << " is out of bounds." << std::endl;
        return;
    }

    currentScene = index;
    for (Entity* ptr : entities) delete ptr;
    scenes[index]->loadScene(phys, &entities);
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
    loadScene((currentScene + 1) % scenes.size());
}

void SceneManager::prev()
{
    if(scenes.size() == 0)
    {
        std::cout << "Cannot call prev(), there are no scenes!" << std::endl;
        return;
    }
    loadScene((currentScene - 1 + scenes.size()) % scenes.size()); // wrap around making sure its not negative
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
    phys.draw();
    for (Entity* ptr : entities) ptr->draw();
    if (drawStaticMesh) aggregateMesh.getMesh().drawWireframe();
}

void SceneManager::registerInputManager(InputManager* input)
{
    inputManager = input;
    phys.registerInputManager(input);
}
