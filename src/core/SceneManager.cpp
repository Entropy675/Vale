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

void SceneManager::updateEnvironmentMesh()
{
    aggregateMesh.clear();
    for (const Entity* ent : scenes[currentSceneIndex]->getEntities())
    {
        if (!ent->hasTag("physics")) continue;
        PhysicsEntity* target = static_cast<PhysicsEntity*>(ent);
        ofMesh transformedMesh = target->getMesh();
        if (transformedMesh.getNumVertices() == 0) continue;

        ofMatrix4x4 transformationMatrix = target->getTransformationMatrix(); 
        transformedMesh.transform(transformationMatrix); // transform the copied mesh

        aggregateMesh.addMesh(transformedMesh); // Add the transformed mesh
        std::cout << "Appended " << transformedMesh.getNumVertices() << " vertices from entity." << std::endl;
    }

    aggregateMesh.setupEnvironment();
    std::cout << "updateEnvironmentMesh success" << std::endl;
    
    /*
    for (Entity* ent : entities)
    {
        if(!ent->hasTag("physics")) continue;
        PhysicsEntity* target = static_cast<PhysicsEntity*>(ent);  
        if (target->getMesh().getNumVertices() == 0) continue;
        ofMesh entityMesh = target->getMesh();

        // Transform each vertex in the entity's mesh
        for (size_t i = 0; i < entityMesh.getNumVertices(); i++)
        {
            ofVec3f vertex = entityMesh.getVertex(i);
            vertex *= target->getScale();
            ofVec3f transformedVertex = target->getRotation() * vertex;
            transformedVertex += target->getTranslation();
            //ofMatrix4x4 transformationMatrix = target->getTransformationMatrix();

            entityMesh.setVertex(i, ofVec3f(transformedVertex.x, transformedVertex.y, transformedVertex.z)); // Update the vertex
            aggregateMesh.addVertex(entityMesh.getVertex(i), entity->getId());
        }

        // Append the transformed mesh to the aggregate mesh
        // appended each vertex individually above
        // aggregateMesh.addVertex(entityMesh);
        std::cout << "Appended " << entityMesh.getNumVertices() << " vertices from entity." << std::endl;

    }*/
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

    phys.clear(); // TODO: special tag for keeping physics entities across scene transitions 
    currentSceneIndex = index;
    
    // TODO: change to searching supertype to class list managed by TagManager (not done yet)
    Camera* cam = nullptr;
    Entity* camTarget = nullptr;
    for (const Entity* ent : scenes[currentSceneIndex]->getEntities())
    {
        if(ent->hasTag("camera_target")
            camTarget = ent;
        if(ent->hasTag("camera")
            cam = static_cast<Camera*>(ent);
    }
    if(camTarget) cam = new Camera(camTarget->getPosition());
    else cam = new Camera();
    cam->registerInputManager(inputManager);
    cam->setup();
    std::cout << "adding phys cam... " << std::endl;
    phys.addCam(cam);
    
    _setup();
}

template <typename T>
void SceneManager::addScene()
{
    scenes.push_back(new T(aggregateMesh));
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
