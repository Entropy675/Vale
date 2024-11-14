#include "Entity.h"

long long Entity::uniqueCounter = 0;

Entity::Entity(glm::vec3 pos)
    : position(pos), hashId(++uniqueCounter) { addTag("entity"); }

Entity::Entity(const ofMesh& meshRef, glm::vec3 pos)
    : position(pos), hashId(++uniqueCounter), mesh(meshRef) {}

Entity::~Entity() {}; // each entity manages its own cleanup in its dtor

void Entity::update()
{
    _input();
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
    if (!setupDone) setupDone = true; // prevent any repeated setup
    else return; // ofLogWarning("Entity") << "Setup has already been called!"; // ignore because who cares
    
    std::cout << "Entity setup: " <<  hashId << std::endl;
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
