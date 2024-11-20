#ifndef ENTITY_H__
#define ENTITY_H__
#include "defs.h"
#include "ofMain.h"
#include "ofTexture.h"

// #define PRINTALLENTITIES
class InputManager;

class Entity
{
private:
    bool setupDone = false;
    static long long uniqueCounter;
    bool inputContext[NUM_KEYS] = {};

protected:
    glm::vec3 position = glm::vec3(0, 0, 0);
    const long long hashId;
    ofMesh mesh;
    bool drawDefaultMaterial = false;


    vector<std::string> tags;
    InputManager* inputManager = nullptr;

    // object level transformations applied
    glm::vec3 scale = glm::vec3(1, 1, 1);
    ofQuaternion rotation;
    glm::vec3 translation = glm::vec3(0, 0, 0); // x, y, z
public:
    // Constructors
    Entity(glm::vec3 position = glm::vec3(0, 0, 0));
    Entity(const ofMesh& meshRef, glm::vec3 dimension = glm::vec3(0, 0, 0));
    virtual ~Entity();

    void update(); // internal
    void draw();
    void setup();
    virtual Entity* clone() const = 0; // you must declare an explicit copy for clarity

    virtual void _update() = 0; // define your entities behaviour
    virtual void _draw() = 0;
    virtual void _setup() = 0;

    virtual void registerInputManager(InputManager* input)              { inputManager = input; };
    virtual void _input()                                               { return; }; // optional: called before update only when an inputManager is registered.

    // references
    virtual const ofMesh& getMesh() const                               { return mesh; }; // Return a const reference to ofMesh
    virtual ofMesh& getMeshRef()                                        { return mesh; }; // Return a const reference to ofMesh
    bool (&getInputContext())[NUM_KEYS]                                 { return inputContext; }

    // setters
    void addTag(const std::string& tag)                                 { if (!hasTag(tag)) tags.push_back(tag); };

    // getters
    ofQuaternion getRotation() const                                    { return rotation; };
    glm::vec3 getScale() const                                          { return scale; };
    glm::vec3 getTranslation() const                                    { return translation + position; };
    bool hasTag(const std::string& tag) const                           { return std::find(tags.begin(), tags.end(), tag) != tags.end(); };

    // helpers
    ofMesh copyMesh() const                                             { return mesh; };
    long long getId() const                                             { return hashId; };
    ofMatrix4x4 getTransformationMatrix() const;
};

#endif // ENTITY_H__
