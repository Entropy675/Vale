#include "Entity.h"

long long Entity::uniqueCounter = 0;
std::unordered_map<int, Entity*> Entity::idToEntityMap;

Entity::Entity(glm::vec3 pos)
    : position(pos), hashId(++uniqueCounter)
{
    addTag("entity");
    idToEntityMap.insert({hashId, this});
}

Entity::Entity(const ofMesh& meshRef, glm::vec3 pos)
    : position(pos), hashId(++uniqueCounter), mesh(meshRef)
{
    addTag("entity");
    idToEntityMap.insert({hashId, this});
}

Entity::~Entity()
{
    idToEntityMap.erase(hashId);
}; // each entity manages its own cleanup in its dtor

void Entity::update()
{
    if(inputManager != nullptr) _input();
    _update();

#ifdef PRINTALLENTITIES
    std::cout << "Id: " << getId() << " Tags: ";
    for (std::string s : tags) std::cout << "[" << s << "], ";
    std::cout << std::endl;
#endif
}

void Entity::draw()
{
    ofPushMatrix();
    /* SRT
    ofScale(scale);
    ofMatrix4x4 rotationMatrix;
    rotation.get(rotationMatrix);
    ofMultMatrix(rotationMatrix);
    ofTranslate(translation);
    */
    ofMultMatrix(getTransformationMatrix());

    _draw();

    ofPopMatrix();
}

void Entity::setup()
{
    std::cout << "Id: " << getId() << " attempted to call setup... ";
    if (!setupDone) setupDone = true; // prevent any repeated setup
    else
    {
        std::cout << std::endl;
        return; // ofLogWarning("Entity") << "Setup has already been called!"; // ignore because who cares
    }

    std::cout << "First Time Pass! Entity setup: " <<  hashId << std::endl;
    _setup();
}

// helpers
ofMatrix4x4 Entity::getTransformationMatrix() const
{
    ofMatrix4x4 transformation;
    transformation.scale(scale.x, scale.y, scale.z);

    ofMatrix4x4 rotationMatrix;
    rotation.get(rotationMatrix);
    transformation.preMult(rotationMatrix);
    transformation.translate(position + translation);
    return transformation;
}
