#include "Scene.h"


Scene::~Scene()
{   // manages own entities
    for (Entity* ent : entities) delete ent;
}


/*
void Scene::loadScene(std::vector<Entity*>* list)
{
    phys.loadScene(scenePhysicsObjects);
    list->clear();
    list->reserve(entities.size());
    //std::copy(sceneObjects.begin(), sceneObjects.end(), std::back_inserter(*list)); see environment object for transferring entities across scenes, TODO: theres should be a more robust way for dynamic entities, temp env object?
    std::cout << "Scene loading count: " << entities.size() << std::endl;
    for (Entity* ptr : entities)
    {
        Entity* clone = ptr->clone();
        list->push_back(clone);
        std::cout << "Scene loading: " << ptr->getId() << std::endl;
        std::cout << "Scene loaded active clone: " << clone->getId() << std::endl;
    }
    
    sceneLoaded = true;
}
*/

void Scene::setup()
{
    inputManager->setActiveEntities(&entities); // activates its entities input callbacks whenever setup is called
    if (setupDone) return;
    _setup(); // scene sets up its objects here
    for (Entity* ptr : entities)
    {
        ptr->registerInputManager(inputManager);
        ptr->setup();
    }
    phys.setup();
    
    // TODO: change to searching supertype to setting up tag type map managed by TagManager (not done yet)
    Camera* cam = nullptr;
    PhysicsEntity* camTarget = nullptr;
    for (Entity* ptr : entities)
    {
        ptr->registerInputManager(inputManager);
        ptr->setup();
        if(ptr->hasTag("camera_target"))
            camTarget = static_cast<PhysicsEntity*>(ptr);
        if(ptr->hasTag("camera"))
            cam = static_cast<Camera*>(ptr);
    }
    
    if(!cam && camTarget) cam = new Camera(camTarget->getPosition());
    else if(!cam) cam = new Camera(glm::vec3(0));
    cam->registerInputManager(inputManager);
    cam->setup();
    std::cout << "adding phys cam... " << std::endl;
    phys.addCam(cam);
    
    setupDone = true;
}

void Scene::update()
{
    _update(); // every scene update call
    phys.update();
    for (Entity* ptr : entities) ptr->update();
}

void Scene::draw()
{
    if(!camsInScene.size())
    {
        std::cout << "!! Error: No camera found in scene!" << std::endl;
        return; // in case no camera in the scenes camera array (maybe the entity isn't being added to its taglist)??
    }
    camsInScene[currentCam]->camBegin();
    _draw(); // every scene draw call
    phys.draw();
    for (Entity* ptr : entities) ptr->draw();
    camsInScene[currentCam]->camEnd();
}

ofMesh Scene::getAggregateMesh()
{
    ofMesh aggregateMesh;
    for (Entity* ent : entities)
    {
        if (!ent->hasTag("physics")) continue;
        PhysicsEntity* target = static_cast<PhysicsEntity*>(ent);
        ofMesh transformedMesh = target->getMesh();
        if (transformedMesh.getNumVertices() == 0) continue;

        ofMatrix4x4 transformationMatrix = target->getTransformationMatrix();         
        for (size_t i = 0; i < transformedMesh.getNumVertices(); i++)
        {
            ofVec3f transformedVertex = transformationMatrix.preMult((ofVec3f)transformedMesh.getVertex(i));
            transformedMesh.setVertex(i, transformedVertex); // glm::vec3(transformedVertex.x, transformedVertex.y, transformedVertex.z)
        }
        aggregateMesh.append(transformedMesh); // Add the transformed mesh
        std::cout << "Appended " << transformedMesh.getNumVertices() << " vertices from entity." << std::endl;
    }
    return aggregateMesh;
}

    
void Scene::addEntity(Entity* ent)
{
    if (!ent->hasTag("physics")) 
    {
        entities.push_back(ent);
        return;
    }
    
    // add to playersInScene. TODO: replace with adding to the tag type map when that function is done
    // keep track of current players in Scene
    if (ent->hasTag("player")) 
    {
        Player* playerPtr = static_cast<Player*>(ent);
        playersInScene.push_back(playerPtr);
        std::cout << "Added to players in scene: " << playerPtr->getPlayerName() << std::endl;
    }
    
    if(ent->hasTag("camera"))
    {
        camsInScene.push_back(static_cast<Camera*>(ent));
        std::cout << "Added existing cam in scene to camsInScene" << std::endl;
    }
    
    
    phys.addEntity(static_cast<PhysicsEntity*>(ent));
}


void Scene::removeEntity(Entity* ent)
{
    if (!ent->hasTag("physics")) 
    {
        auto it = std::find(entities.begin(), entities.end(), ent);
        if (it != entities.end()) entities.erase(it);
        delete ent;
        return;
    }
    
    //TODO: replace these with a removal from the tag type map when that is done
    if (playersInScene.size()) 
    {
        auto playerIt = std::find(playersInScene.begin(), playersInScene.end(), ent);
        if (playerIt != playersInScene.end())
            playersInScene.erase(playerIt);
    }
    if (camsInScene.size()) 
    {
        auto camIt = std::find(camsInScene.begin(), camsInScene.end(), ent);
        if (camIt != camsInScene.end())
            camsInScene.erase(camIt);
    }
    
    phys.removeEntity(static_cast<PhysicsEntity*>(ent));
    return;
}

// TODO: make a format for loading entities from a file.
bool Scene::loadSceneFromFile(const std::string& path)
{
    // nothing for now, any scene can load more entities
    return false;
}

bool Scene::saveSceneToFile(const std::string& path)
{
    // save current scene to a file
    return false;
}

