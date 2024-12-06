#include "PhysicsEntity.h"

PhysicsEntity::PhysicsEntity(glm::vec3 dim) : Entity(dim)
{
    rotation = ofQuaternion(0, 0, 0, 1);
    addTag("physics");
}

PhysicsEntity::PhysicsEntity(const ofMesh& meshRef, glm::vec3 dim) : Entity(meshRef, dim)
{
    rotation = ofQuaternion(0, 0, 0, 1);
}

PhysicsEntity::~PhysicsEntity() {}; // each PhysicsEntity manages its own cleanup in its dtor


void PhysicsEntity::collision(PhysicsEntity& target)
{
    _collision(target);
}

// all
void PhysicsEntity::setPhysicsState(const glm::vec3& newPosition, const glm::vec3& newVelocity, const glm::vec3& newAcceleration)
{
    position = newPosition;
    velocity = newVelocity;
    acceleration = newAcceleration;
}
