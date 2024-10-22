#ifndef ENTITY_H__
#define ENTITY_H__

#include "ofMain.h"
#include "ofTexture.h"

class Entity {
private:
    // Newton physics
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;

    // Inertia (mass or moment of inertia)
    float inertia;

    // Angular velocity and acceleration
    glm::vec3 angularVelocity;
    glm::vec3 angularAcceleration;

    ofMesh mesh;
	ofMaterial material;

public:
    // Constructors
    Entity(); // Default constructor (creates a cube mesh)
    Entity(const ofMesh& meshRef); // Constructor that takes a const reference to ofMesh
    ~Entity(); 

    virtual void setup();
    virtual void update();
    virtual void draw();

    // Getters
    glm::vec3 getPosition() const;
    glm::vec3 getVelocity() const;
    glm::vec3 getAcceleration() const;
    float getInertia() const;
    glm::vec3 getAngularVelocity() const;
    glm::vec3 getAngularAcceleration() const;
    
    // references
    ofMesh& getMesh(); // Return a const reference to ofMesh
    ofMaterial& getMaterial(); // Return a reference to ofMaterial

    // Setters for material properties
    void setMaterial(const ofMaterial& mat); // Set the material
    void setMaterialColor(const ofColor& color); // Set the material color
    void setMaterialShininess(float shininess); // Set the material shininess

    // Adjust position
    void moveTo(const glm::vec3& newPos);
    void translate(const glm::vec3& offset);

    // Adjust velocity
    void setVelocity(const glm::vec3& vel);
    void addVelocity(const glm::vec3& delta);

    // Inertia
    void setInertia(float inertiaValue);

    // Angular velocity and acceleration
    void setAngularVelocity(const glm::vec3& angVel);
    void addAngularVelocity(const glm::vec3& delta);
    
    // Apply force
    void applyForce(const glm::vec3& direction, float power);
};

#endif // ENTITY_H__
