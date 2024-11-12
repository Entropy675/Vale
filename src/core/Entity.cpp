#include "Entity.h"

long long Entity::uniqueCounter = 0;

Entity::Entity(glm::vec3 pos)
    : position(pos), hashId(++uniqueCounter) {}

Entity::Entity(const ofMesh& meshRef, glm::vec3 pos)
    : position(pos), hashId(++uniqueCounter), mesh(meshRef) {}

Entity::~Entity() {}; // each entity manages its own cleanup in its dtor

void Entity::update()
{
    _update();
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

void Entity::keyPressed(int key)                    { _keyPressed(key); }
void Entity::_keyPressed(int key)                   { return; }

// getters
ofQuaternion Entity::getRotation() const            { return rotation; }
glm::vec3 Entity::getScale() const                  { return scale; }
glm::vec3 Entity::getTranslation() const            { return translation + position; }

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

long long Entity::getId() const                     { return hashId; }
ofMesh Entity::copyMesh() const                     { return mesh; }

// references
ofMesh& Entity::getMesh()                           { return mesh; } 
ofMaterial& Entity::getMaterial()                   { return material; }

// Setters for material properties
void Entity::toggleDefaultMaterial()                { drawDefaultMaterial = !drawDefaultMaterial; }
void Entity::setMaterial(const ofMaterial& mat)     { material = mat; }
void Entity::setMaterialColor(const ofColor& color) { material.setDiffuseColor(color); }
void Entity::setMaterialShininess(float shininess)  { material.setShininess(shininess); }
