#ifndef ENTITY_H__
#define ENTITY_H__

#include "ofMain.h"
#include "ofTexture.h"

class Entity 
{
private:
	bool setupDone = false;
	static long long uniqueCounter;
	
protected:
    const long long hashId;
    glm::vec3 dimensions; // (length, width, height) = (x, y, z)
    ofMesh mesh; // Assuming this is a member variable
	ofMaterial material;
	
    // Inertia (mass) 
    float inertia = 1.0f;
    
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
    Entity(glm::vec3 dimension = glm::vec3(0, 0, 0));
    Entity(const ofMesh& meshRef, glm::vec3 dimension = glm::vec3(0, 0, 0));
    
    virtual ~Entity();
    
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void setup();

    // references
    ofMesh& getMesh(); // Return a const reference to ofMesh
    ofMaterial& getMaterial(); // Return a reference to ofMaterial

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
    float getInertia() const; // mass
    
    // --- Setters ---
    
    // material properties
    void setMaterial(const ofMaterial& mat); // Set the material
    void setMaterialColor(const ofColor& color); // Set the material color
    void setMaterialShininess(float shininess); // Set the material shininess
    
    void setInertia(float inertiaValue);
	void addFacingRotation(const glm::vec4& rot);
    void setFacingRotation(const glm::vec4& rot);
    
    // Physics functions
    void moveTo(const glm::vec3& newPos); // Adjust position
    void translate(const glm::vec3& offset);
    void setVelocity(const glm::vec3& vel); // Adjust velocity
    void addVelocity(const glm::vec3& delta);
    void setAcceleration(const glm::vec3& accel);
    void applyForce(const glm::vec3& direction, float power); // Apply force (accel)
    // All in one:
    void setPhysicsState(const glm::vec3& newPosition, const glm::vec3& newVelocity, const glm::vec3& newAcceleration);
    
    
    // Angular velocity and acceleration
    void setAngularVelocity(const glm::vec3& angVel);
    void addAngularVelocity(const glm::vec3& delta);
    
    
};

#endif // ENTITY_H__
