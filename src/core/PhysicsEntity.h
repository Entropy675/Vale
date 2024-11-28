#ifndef PHYSICSENTITY_H__
#define PHYSICSENTITY_H__

#include "ofMain.h"
#include "ofTexture.h"
#include "Entity.h"

class PhysicsEntity : public Entity
{
protected:
    // Newtonian physics (use glm::length(vec) for calculating magnitude)
    float mass = 1.0f;
    glm::vec3 velocity = glm::vec3(0, 0, 0);
    glm::vec3 acceleration = glm::vec3(0, 0, 0);

    glm::vec3 angularVelocity = glm::vec3(0, 0, 0);
    glm::vec3 angularAcceleration = glm::vec3(0, 0, 0);


    // current ideas:
    // - a type of function pointer for collision(PhysicsEntity) behaviour
    // - a map for default tag -> collision behaviour
    //   (ways to add to this map, automatically call based on tags in collision)

public:
    // Constructors
    PhysicsEntity(glm::vec3 dimension = glm::vec3(0, 0, 0));
    PhysicsEntity(const ofMesh& meshRef, glm::vec3 dimension = glm::vec3(0, 0, 0));
    virtual ~PhysicsEntity();

    void collision(PhysicsEntity& target); // internal

    virtual void _collision(PhysicsEntity& target) = 0;
    // Due to the nature of these, we will be copying them. Need a good, explicit copy ctor.
    // So write one in this clone method. This must return a new copy in heap of this entity.

    // --- Getters ---

    // physics
    glm::vec3 getPosition() const                               { return position; };
    glm::vec3 getVelocity() const                               { return velocity; };
    glm::vec3 getAcceleration() const                           { return acceleration; };

    glm::vec3 getAngularVelocity() const                        { return angularVelocity; };
    glm::vec3 getAngularAcceleration() const                    { return angularAcceleration; };

     // object properties
    glm::vec3 getFacingDirection() const                        { return rotation * glm::vec3(0, 0, 1); };
    ofQuaternion getFacingRotation() const                      { return rotation; };
    float getMass() const                                       { return mass; }; // mass

    // --- Setters ---

    // Physics functions
    void setMass(float massValue)                            { mass = massValue; };

    void moveTo(const glm::vec3& newPos)                        { position = newPos; }; // Adjust position
    void translate(const glm::vec3& offset)                     { position += offset; };

    void setVelocity(const glm::vec3& vel)                      { velocity = vel; }; // Adjust velocity
    void addVelocity(const glm::vec3& delta)                    { velocity += delta; };

    void setAcceleration(const glm::vec3& accel)                { acceleration = accel; };
    void applyForce(const glm::vec3& direction, float power)    { acceleration += direction * power / mass; }; // Apply force (accel)

    // Angular velocity and acceleration
    void setAngularVelocity(const glm::vec3& angVel)            { angularVelocity = angVel; };
    void addAngularVelocity(const glm::vec3& delta)             { angularVelocity += delta; };

    // All in one:
    void setPhysicsState(const glm::vec3& newPosition, const glm::vec3& newVelocity, const glm::vec3& newAcceleration);

};

#endif
