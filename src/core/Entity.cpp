#include "Entity.h"

long long Entity::uniqueCounter = 0;

Entity::Entity(glm::vec3 dim)
    : dimensions(dim), hashId(std::hash<std::size_t>{}(++uniqueCounter))
{ 
    rotation = ofQuaternion(0, 0, 0, 1);
}

Entity::Entity(const ofMesh& meshRef, glm::vec3 dim)
	: dimensions(dim), hashId(std::hash<std::size_t>{}(++uniqueCounter)), mesh(meshRef) 
{
    rotation = ofQuaternion(0, 0, 0, 1);
} 

Entity::~Entity() {};

void Entity::setup()
{
    if (!setupDone) 
        setupDone = true;
    else
        ofLogWarning("Entity") << "Setup has already been called!";
}

// Getters
glm::vec3 Entity::getPosition() const 				{ return position; } 
glm::vec3 Entity::getVelocity() const 				{ return velocity; }
glm::vec3 Entity::getAcceleration() const 			{ return acceleration; }
float Entity::getInertia() const 					{ return inertia; }
glm::vec3 Entity::getAngularVelocity() const 		{ return angularVelocity; }
glm::vec3 Entity::getAngularAcceleration() const 	{ return angularAcceleration; }

// references
ofMesh& Entity::getMesh() 							{ return mesh; } 
ofMaterial& Entity::getMaterial() 					{ return material; }

// Setters for material properties
void Entity::setMaterial(const ofMaterial& mat) 	{ material = mat; }
void Entity::setMaterialColor(const ofColor& color) { material.setDiffuseColor(color); }
void Entity::setMaterialShininess(float shininess) 	{ material.setShininess(shininess); }
void Entity::setInertia(float inertiaValue)			{ inertia = inertiaValue; }

void Entity::setFacingRotation(const glm::vec4& rot){ rotation.makeRotate(rot.w, rot.x, rot.y, rot.z); }
ofQuaternion Entity::getFacingRotation() const		{ return rotation; }

// rot = specify % rotation in axis, (%x, %y, %z, angle)
void Entity::addFacingRotation(const glm::vec4& rot) 
{
    ofQuaternion rotationQuat;
    rotationQuat.makeRotate(rot.w, rot.x, rot.y, rot.z);
    rotation *= rotationQuat; // Accumulate the rotation
    rotation.normalize();
}

glm::vec3 Entity::getFacingDirection() const
{
    // Extract the forward direction from the quaternion
    return rotation * glm::vec3(0, 0, 1);
}

// Adjust position
void Entity::moveTo(const glm::vec3& newPos) 		{ position = newPos; }
void Entity::translate(const glm::vec3& offset)		{ position += offset; }

// Adjust velocity
void Entity::setVelocity(const glm::vec3& vel)		{ velocity = vel; }
void Entity::addVelocity(const glm::vec3& delta)	{ velocity += delta; }

// Apply force/acceleration
void Entity::setAcceleration(const glm::vec3& accel){ acceleration = accel; }
void Entity::applyForce(const glm::vec3& direction, float power) 
{
    acceleration += direction * power / inertia; // inertia is mass
}

// all
void Entity::setPhysicsState(const glm::vec3& newPosition, const glm::vec3& newVelocity, const glm::vec3& newAcceleration)
{
    position = newPosition;
    velocity = newVelocity;
    acceleration = newAcceleration;
}