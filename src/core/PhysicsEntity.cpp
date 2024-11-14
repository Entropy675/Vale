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


void PhysicsEntity::collision(const PhysicsEntity& target) 
{
    _collision(target);
}

// Getters
glm::vec3 PhysicsEntity::getPosition() const                { return position; } 
glm::vec3 PhysicsEntity::getVelocity() const                { return velocity; }
glm::vec3 PhysicsEntity::getAcceleration() const            { return acceleration; }
float PhysicsEntity::getMass() const                        { return mass; }
glm::vec3 PhysicsEntity::getAngularVelocity() const         { return angularVelocity; }
glm::vec3 PhysicsEntity::getAngularAcceleration() const     { return angularAcceleration; }
ofQuaternion PhysicsEntity::getFacingRotation() const       { return rotation; }

// setters
void PhysicsEntity::setMass(float massValue)                { mass = massValue; }
void PhysicsEntity::setFacingRotation(const glm::vec4& rot) { rotation.makeRotate(rot.w, rot.x, rot.y, rot.z); }

// rot = specify % rotation in axis, (%x, %y, %z, angle)
void PhysicsEntity::addFacingRotation(const glm::vec4& rot) 
{
    ofQuaternion rotationQuat;
    rotationQuat.makeRotate(rot.w, rot.x, rot.y, rot.z);
    rotation *= rotationQuat; // Accumulate the rotation
    rotation.normalize();
}

glm::vec3 PhysicsEntity::getFacingDirection() const
{
    // Extract the forward direction from the quaternion
    return rotation * glm::vec3(0, 0, 1);
}

// Adjust position
void PhysicsEntity::moveTo(const glm::vec3& newPos)         { position = newPos; }
void PhysicsEntity::translate(const glm::vec3& offset)      { position += offset; }

// Adjust velocity
void PhysicsEntity::setVelocity(const glm::vec3& vel)       { velocity = vel; }
void PhysicsEntity::addVelocity(const glm::vec3& delta)     { velocity += delta; }

// Apply force/acceleration
void PhysicsEntity::setAcceleration(const glm::vec3& accel) { acceleration = accel; }
void PhysicsEntity::applyForce(const glm::vec3& direction, float power) 
{
    acceleration += direction * power / mass; // mass is mass
}

// all
void PhysicsEntity::setPhysicsState(const glm::vec3& newPosition, const glm::vec3& newVelocity, const glm::vec3& newAcceleration)
{
    position = newPosition;
    velocity = newVelocity;
    acceleration = newAcceleration;
}
