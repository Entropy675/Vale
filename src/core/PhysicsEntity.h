#ifndef PHYSICSENTITY_H__
#define PHYSICSENTITY_H__

#include "ofMain.h"
#include "ofTexture.h"
#include "PhysicsMetadata.h"
#include "Entity.h"
#include <unordered_map>

class PhysicsEntity : public Entity
{
    friend class TagManager;
    
protected:
    // properties
    ofMesh mesh;
    float mass = 1.0f;
    bool drawDefaultMaterial = false;
    glm::vec3 scale = glm::vec3(1, 1, 1);
    ofQuaternion rotation;
    glm::vec3 translation = glm::vec3(0, 0, 0);
    
    // physics
    glm::vec3 velocity = glm::vec3(0, 0, 0);
    glm::vec3 acceleration = glm::vec3(0, 0, 0);

    glm::vec3 angularVelocity = glm::vec3(0, 0, 0);
    glm::vec3 angularAcceleration = glm::vec3(0, 0, 0);

    std::unordered_map<std::string, PhysicsMetadata> LocalPhysicsMetadata; // Map from tag index to PhysicsMetadata

    // Check if a context exists for a given tag index
    bool hasPhysicsMetadata(const std::string& physicsTag) const                                { return LocalPhysicsMetadata.find(physicsTag) != LocalPhysicsMetadata.end(); }
    const std::unordered_map<std::string, PhysicsMetadata>& getLocalPhysicsMetadata() const     { return LocalPhysicsMetadata; }; // may be empty if all physics tags use static context

    bool addPhysicsMetadata(const std::string& physicsTag, const PhysicsMetadata& context); // returns false if tag doesn't exist
    bool getPhysicsMetadata(const std::string& physicsTag, PhysicsMetadata& out); // Retrieve a PhysicsMetadata by tag index
    bool removePhysicsMetadata(const std::string& physicsTag);
    
public:
    // Constructors
    PhysicsEntity(glm::vec3 dimension = glm::vec3(0, 0, 0));
    PhysicsEntity(const ofMesh& meshRef, glm::vec3 dimension = glm::vec3(0, 0, 0));
    virtual ~PhysicsEntity();
    
    void draw() override; // for automatic transformation logic

    void collision(PhysicsEntity& target); // internal
    virtual void _collision(PhysicsEntity& target) {}; // define your collision behavior

    // --- Getters ---

    // physics & properties
    ofMatrix4x4 getTransformationMatrix() const;
    bool getPhysicsMetadata(const std::string& physicsTag, PhysicsMetadata& out) const;
    virtual const ofMesh& getMesh() const                                                       { return mesh; };
    
    glm::vec3 getScale() const                                                                  { return scale; };
    ofQuaternion getRotation() const                                                            { return rotation; };
    glm::vec3 getTranslation() const                                                            { return translation + position; };

    glm::vec3 getVelocity() const                                                               { return velocity; };
    glm::vec3 getAcceleration() const                                                           { return acceleration; };
    glm::vec3 getAngularVelocity() const                                                        { return angularVelocity; };
    glm::vec3 getAngularAcceleration() const                                                    { return angularAcceleration; };

    glm::vec3 getFacingDirection() const                                                        { return rotation * glm::vec3(0, 0, 1); };
    ofQuaternion getFacingRotation() const                                                      { return rotation; };
    float getMass() const                                                                       { return mass; }; // mass


    // --- Setters ---
    void removeTag(const std::string& tag) override;

    // Physics functions
    void setMass(float massValue)                                                               { mass = massValue; };

    void moveTo(const glm::vec3& newPos)                                                        { position = newPos; }; // Adjust position
    void translate(const glm::vec3& offset)                                                     { position += offset; };

    void setVelocity(const glm::vec3& vel)                                                      { velocity = vel; }; // Adjust velocity
    void addVelocity(const glm::vec3& delta)                                                    { velocity += delta; };

    void setAcceleration(const glm::vec3& accel)                                                { acceleration = accel; };
    void applyForce(const glm::vec3& direction, float power)                                    { acceleration += direction * power / mass; }; // Apply force (accel)

    void setAngularVelocity(const glm::vec3& angVel)                                            { angularVelocity = angVel; };
    void addAngularVelocity(const glm::vec3& delta)                                             { angularVelocity += delta; };

    // All in one:
    void setPhysicsState(const glm::vec3& newPosition, const glm::vec3& newVelocity, const glm::vec3& newAcceleration);
};

#endif
