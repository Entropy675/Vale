#include "Entity.h"

Entity::Entity() 
    : position(0, 0, 0), 
      velocity(0, 0, 0), 
      acceleration(0, 0, 0), 
      inertia(1.0f), // Default inertia
      angularVelocity(0, 0, 0), 
      angularAcceleration(0, 0, 0) 
{
    // Mesh is left uninitialized (can be set later)
}

Entity::Entity(const ofMesh& meshRef) 
    : position(0, 0, 0), 
      velocity(0, 0, 0), 
      acceleration(0, 0, 0), 
      inertia(1.0f), // Default inertia
      angularVelocity(0, 0, 0), 
      angularAcceleration(0, 0, 0), 
      mesh(meshRef) {} // Copy mesh

Entity::~Entity() {
    // No need for cleanup since we're not using dynamic memory
}

// Getters
glm::vec3 Entity::getPosition() const 				{ return position; } 
glm::vec3 Entity::getVelocity() const 				{ return velocity; }
glm::vec3 Entity::getAcceleration() const 			{ return acceleration; }
float Entity::getInertia() const 					{ return inertia; }
glm::vec3 Entity::getAngularVelocity() const 		{ return angularVelocity; }
glm::vec3 Entity::getAngularAcceleration() const 	{ return angularAcceleration; }

// references
ofMesh& Entity::getMesh() 			{ return mesh; } 
ofMaterial& Entity::getMaterial() 	{ return material; }

// Setters for material properties
void Entity::setMaterial(const ofMaterial& mat) 
{
    material = mat;
}

void Entity::setMaterialColor(const ofColor& color) 
{
    material.setDiffuseColor(color);
}

void Entity::setMaterialShininess(float shininess) 
{
    material.setShininess(shininess);
}

// Adjust position
void Entity::moveTo(const glm::vec3& newPos) 		
{ 
	position = newPos; 
}

void Entity::translate(const glm::vec3& offset)
{
	position += offset;
}

// Adjust velocity
void Entity::setVelocity(const glm::vec3& vel)
{
	velocity = vel;
}

void Entity::addVelocity(const glm::vec3& delta)
{
	velocity += delta;
}

// Inertia
void Entity::setInertia(float inertiaValue)
{
	inertia = inertiaValue;
}

// Angular velocity and acceleration
void Entity::setAngularVelocity(const glm::vec3& angVel)
{
	angularVelocity = angVel;
}

void Entity::addAngularVelocity(const glm::vec3& delta)
{
	angularVelocity += delta;
}

// Apply force
void Entity::applyForce(const glm::vec3& direction, float power) 
{
    acceleration += direction * power / inertia; // Assuming inertia is mass
}
