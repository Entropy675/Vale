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
    ofMesh mesh; // Assuming this is a member variable
	ofMaterial material;
	
    glm::vec3 position = glm::vec3(0, 0, 0);
    
    // object level transformations applied 
    glm::vec3 scale = glm::vec3(1, 1, 1);
    ofQuaternion rotation;
    glm::vec3 translation = glm::vec3(0, 0, 0);
    
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
    
    // references
    ofMesh& getMesh(); // Return a const reference to ofMesh
    ofMaterial& getMaterial(); // Return a reference to ofMaterial

    // material properties
    void setMaterial(const ofMaterial& mat); // Set the material
    void setMaterialColor(const ofColor& color); // Set the material color
    void setMaterialShininess(float shininess); // Set the material shininess
};

#endif // ENTITY_H__
