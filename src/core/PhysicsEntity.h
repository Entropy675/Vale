#ifndef PHYSICSENTITY_H__
#define PHYSICSENTITY_H__

#include "ofMain.h"
#include "ofTexture.h"
#include "Entity.h"

/*
if this has been called: physicsEntity.addTag("sphere");
Something like this happens in that func:
physicsEntity.addPhysicsContext(0, {
    [](const glm::vec3& point) { return glm::length(point) < 5.0f; }, // Bounding equation
    [](const glm::vec3& point) { return glm::normalize(point); }      // Collision normal
});
where those funcs are looked up from a table and the values are passable in the tag itself.
*/

struct PhysicsContext
{
    std::function<bool(const glm::vec3&)> boundingEquation; // Checks for collision
    std::function<glm::vec3(const glm::vec3&)> collisionNormal; // Computes collision normal
};

class PhysicsEntity : public Entity
{
protected:
    // Newtonian physics (use glm::length(vec) for calculating magnitude)
    float mass = 1.0f;
    glm::vec3 velocity = glm::vec3(0, 0, 0);
    glm::vec3 acceleration = glm::vec3(0, 0, 0);

    glm::vec3 angularVelocity = glm::vec3(0, 0, 0);
    glm::vec3 angularAcceleration = glm::vec3(0, 0, 0);

    std::unordered_map<size_t, PhysicsContext> tagsIndexToContext; // Map from tag index to PhysicsContext

    // Check if a context exists for a given tag index
    bool hasPhysicsContext(size_t tagIndex) const { return tagsIndexToContext.find(tagIndex) != tagsIndexToContext.end(); }
    const std::unordered_map<size_t, PhysicsContext>& getAllPhysicsContexts() const { return tagsIndexToContext; };
    void addPhysicsContext(size_t tagIndex, const PhysicsContext& context)
    {
        if (tagsIndexToContext.find(tagIndex) != tagsIndexToContext.end()) { return; }
        tagsIndexToContext[tagIndex] = context;
    }
    // Retrieve a PhysicsContext by tag index
    PhysicsContext* getPhysicsContext(size_t tagIndex)
    {
        auto it = tagsIndexToContext.find(tagIndex);
        return (it != tagsIndexToContext.end()) ? &it->second : nullptr; // Return nullptr if not found
    }

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
