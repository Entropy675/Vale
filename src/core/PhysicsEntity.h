#ifndef PHYSICSENTITY_H__
#define PHYSICSENTITY_H__

#include "ofMain.h"
#include "ofTexture.h"
#include "PhysicsMetadata.h"
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

    std::unordered_map<size_t, PhysicsMetadata> tagsIndexToContext; // Map from tag index to PhysicsMetadata

    // Check if a context exists for a given tag index
    bool hasPhysicsMetadata(size_t tagIndex) const { return tagsIndexToContext.find(tagIndex) != tagsIndexToContext.end(); }
    const std::unordered_map<size_t, PhysicsMetadata>& getAllPhysicsMetadata() const { return tagsIndexToContext; };
    void addPhysicsMetadata(size_t tagIndex, const PhysicsMetadata& context)
    {
        if (tagsIndexToContext.find(tagIndex) != tagsIndexToContext.end()) { return; }
        tagsIndexToContext[tagIndex] = context;
    }
    // Retrieve a PhysicsMetadata by tag index
    bool getPhysicsMetadata(size_t tagIndex, PhysicsMetadata& out)
    {
        auto it = tagsIndexToContext.find(tagIndex);
        if (it != tagsIndexToContext.end())
        {
            out = it->second;
            return true;
        }
        return false;
    }

    bool removePhysicsMetadata(size_t tagIndex)
    {
        auto it = tagsIndexToContext.find(tagIndex);
        if (it != tagsIndexToContext.end())
        {
            tagsIndexToContext.erase(it);
            return true; // Successfully removed
        }
        return false; // No metadata found for the given tag index
    }

    friend class TagManager;
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

    bool getPhysicsMetadata(int entityId, PhysicsMetadata& out) const
    {
        auto it = tagsIndexToContext.find(entityId);
        if (it != tagsIndexToContext.end())
        {
            out = it->second;
            return true;
        }
        return false;
    }


    // object properties
    glm::vec3 getFacingDirection() const                        { return rotation * glm::vec3(0, 0, 1); };
    ofQuaternion getFacingRotation() const                      { return rotation; };
    float getMass() const                                       { return mass; }; // mass

    // --- Setters ---
    void removeTag(const std::string& tag) override
    {
        auto it = std::find(tags.begin(), tags.end(), tag);
        if (it != tags.end())
            tags.erase(it);
        if (TagManager::hasPhysicsTag(tag))
            removePhysicsMetadata(std::distance(tags.begin(), it));
    }

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
