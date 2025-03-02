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
    std::cout << "Entering scene setup... " << std::endl;
    inputManager->flushActiveEntities();

    if (setupDone)
    {
        inputManager->setActiveEntities(entities); // activates its entities input callbacks whenever setup is called
        return;
    }

    std::cout << "first time setup... " << std::endl;
    for (Entity* ptr : entities)
    {
        ptr->registerInputManager(inputManager);
        ptr->setup();
    }
    phys.setup();
    _setup(); // user defined optional extra scene set up here (called first time scene is set up, use for setup thats not entity related)
    
    // TODO: change to searching supertype / setting up tag type map managed by TagManager (not done yet)
    currentCam = 0;
    if (camsInScene.size() == 0)
    {
        std::cout << "!!!!!      <  No cameras found in loaded scene?? > < Loading default camera >      !!!!!" << std::endl;
        camsInScene.push_back(new Camera(glm::vec3(0)));
    }
    Camera* cam = camsInScene[currentCam];

    cam->registerInputManager(inputManager);
    cam->setTargets(&camTargetsInScene);
    cam->setup();
    phys.addCam(cam);

    inputManager->setActiveEntities(entities);
    setupDone = true;
}

void Scene::update()
{
    for (Entity* ptr : entities) ptr->update();
    phys.update();
    _update(); // user defined optional scene update call (for behaviour not associated with entities but the scene itself)
}

void Scene::draw()
{
    if(!camsInScene.size())
    {
        std::cout << "!! Error: No camera found in scene!" << std::endl;
        return; // in case no camera in the scenes camera array (maybe the entity isn't being added to its taglist)??
    }
    camsInScene[currentCam]->camBegin();
    for (Entity* ptr : entities) ptr->draw();
    phys.draw();
    _draw(); // user defined optional scene draw call (for non entity related drawing)
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
        camTargetsInScene.push_back(static_cast<PhysicsEntity*>(ent));
        Player* playerPtr = static_cast<Player*>(ent);
        playersInScene.push_back(playerPtr);
        std::cout << "Added to players in scene: " << playerPtr->getPlayerName() << std::endl;
    }
    
    if(ent->hasTag("camera"))
    {
        camsInScene.push_back(static_cast<Camera*>(ent));
        std::cout << "Added existing cam in scene to camsInScene" << std::endl;
        return; // cam is handled specially when a scene is setup
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


template<typename T>
std::vector<T*> Scene::getEntitiesWithTag(const std::string& tag)
{ // switch to storing a map in tag manager when loading scene its better, runtime dictionary?
    static_assert(std::is_base_of<Entity, T>::value, "Return type and tag must be a supertype derived from Entity!");
    std::vector<T*> result;
    for(const auto& entity : entities)
        if(entity->hasTag(tag))
            if(T* cast = dynamic_cast<T*>(entity))
                result.push_back(cast);
    return result;
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

