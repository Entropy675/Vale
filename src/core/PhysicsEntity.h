#ifndef PHYSICSENTITY_H__
#define PHYSICSENTITY_H__

#include "ofMain.h"
#include "ofTexture.h"
#include "Entity.h"

class PhysicsEntity : Entity
{
protected:
    // Inertia (mass) 
    float mass = 1.0f;
    
    // Newtonian physics (use glm::length(vec) for calculating magnitude)
    glm::vec3 position = glm::vec3(0, 0, 0);
    glm::vec3 velocity = glm::vec3(0, 0, 0);
    glm::vec3 acceleration = glm::vec3(0, 0, 0);

    glm::vec3 angularVelocity = glm::vec3(0, 0, 0);
    glm::vec3 angularAcceleration = glm::vec3(0, 0, 0);
    
    // actual values used to draw frame by frame, calculated via above
    glm::vec3 scale = glm::vec3(1, 1, 1);
	ofQuaternion rotation;
    glm::vec3 translation = glm::vec3(0, 0, 0);
    
public:
    // Constructors
    PhysicsEntity(glm::vec3 dimension = glm::vec3(0, 0, 0));
    PhysicsEntity(const ofMesh& meshRef, glm::vec3 dimension = glm::vec3(0, 0, 0));
    virtual ~PhysicsEntity();
    
    // --- Getters ---
    
    // physics
    glm::vec3 getPosition() const;
    glm::vec3 getVelocity() const;
    glm::vec3 getAcceleration() const;

    glm::vec3 getAngularVelocity() const;
    glm::vec3 getAngularAcceleration() const;
    
     // object properties
    glm::vec3 getFacingDirection() const;
    ofQuaternion getFacingRotation() const;
    float getMass() const; // mass
    
    // --- Setters ---
    
    // Physics functions
    void setMass(float inertiaValue);
    
	void addFacingRotation(const glm::vec4& rot);
    void setFacingRotation(const glm::vec4& rot);
    
    void moveTo(const glm::vec3& newPos); // Adjust position
    void translate(const glm::vec3& offset);
    
    void setVelocity(const glm::vec3& vel); // Adjust velocity
    void addVelocity(const glm::vec3& delta);
    
    void setAcceleration(const glm::vec3& accel);
    void applyForce(const glm::vec3& direction, float power); // Apply force (accel)
    
    // Angular velocity and acceleration
    void setAngularVelocity(const glm::vec3& angVel);
    void addAngularVelocity(const glm::vec3& delta);
    
    // All in one:
    void setPhysicsState(const glm::vec3& newPosition, const glm::vec3& newVelocity, const glm::vec3& newAcceleration);
    
};

#endif // ENTITY_H__
