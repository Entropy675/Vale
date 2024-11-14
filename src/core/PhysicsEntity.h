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

    void collision(const PhysicsEntity& target); // internal
    
    virtual void _collision(const PhysicsEntity& target) = 0;
    virtual PhysicsEntity* clone() const = 0; 
    // Due to the nature of these, we will be copying them. Need a good, explicit copy ctor.
    // So write one in this clone method. This must return a new copy in heap of this entity. 
    
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
