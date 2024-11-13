#ifndef ENTITY_H__
#define ENTITY_H__

#include "ofMain.h"
#include "ofTexture.h"
#include "InputManager.h"

class Entity 
{
private:
    bool setupDone = false;
    static long long uniqueCounter;
    
protected:
    ofMesh mesh;
    ofMaterial material;
    bool drawDefaultMaterial = false;
    
    glm::vec3 position = glm::vec3(0, 0, 0);
    const long long hashId;
    
    // object level transformations applied 
    glm::vec3 scale = glm::vec3(1, 1, 1);
    ofQuaternion rotation;
    glm::vec3 translation = glm::vec3(0, 0, 0); // x, y, z
    InputManager* inputManager = nullptr;
public:
    // Constructors
    Entity(glm::vec3 position = glm::vec3(0, 0, 0));
    Entity(const ofMesh& meshRef, glm::vec3 dimension = glm::vec3(0, 0, 0));
    virtual ~Entity();
    
    void update(); // internal
    void draw();
    void setup();
    
    virtual void _update() = 0; // define your entities behaviour
    virtual void _draw() = 0;
    virtual void _setup() = 0;
    
    virtual void registerInputManager(InputManager* input) { inputManager = input; };
    virtual void _input() { return; }; // optional: called at the end of update only when an inputManager is registered.
    
    // references
    virtual ofMesh& getMesh(); // Return a const reference to ofMesh
    ofMaterial& getMaterial(); // Return a reference to ofMaterial

    // material properties
    void toggleDefaultMaterial();
    void setMaterial(const ofMaterial& mat); // Set the material
    void setMaterialColor(const ofColor& color); // Set the material color
    void setMaterialShininess(float shininess); // Set the material shininess
    
    // getters
    ofQuaternion getRotation() const;
    glm::vec3 getScale() const;
    glm::vec3 getTranslation() const;
    
    // helpers
    ofMatrix4x4 getTransformationMatrix() const;
    ofMesh copyMesh() const;
    long long getId() const;
};

#endif // ENTITY_H__
