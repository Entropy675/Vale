#ifndef ENTITY_H__
#define ENTITY_H__
#include "defs.h"
#include "ofMain.h"
#include "ofTexture.h"
#include "TagManager.h"

// #define PRINTALLENTITIES
class InputManager;

class Entity
{
    friend class InputManager; // for input context direct access
    friend class TagManager;   // for tags direct access

private:
    bool setupDone = false;
    static long long uniqueCounter;
    static std::unordered_map<int, Entity*> idToEntityMap;
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

    // you must declare an explicit copy for clarity
    virtual Entity* clone() const = 0;

    // internal engine callbacks (DO NOT OVERRIDE UNLESS YOU ARE MODIFYING ENGINE)
    virtual void update();
    virtual void draw();
    virtual void setup();

    // user created callbacks
    virtual void _update() = 0; // define your entities behaviour
    virtual void _draw() = 0;
    virtual void _setup() = 0;

    // optional, if your entity needs input
    virtual void registerInputManager(InputManager* input)              { inputManager = input; }; // everything below is optional and requires a registered input manager
    virtual void _input()                                               { return; }; // called before update only when an inputManager is registered
    virtual void _mouseMoved()                                          { return; }; // called whenever mouseMoved event occurs, this and following mouse events are forwarded from open frameworks
    virtual void _mouseMoved(ofMouseEventArgs& mouse)                   { return; };
    virtual void _mouseDragged(int x, int y, int button)                { return; };
    virtual void _mousePressed(int x, int y, int button)                { return; };
    virtual void _mouseReleased(int x, int y, int button)               { return; };
    virtual void _mouseEntered(int x, int y)                            { return; };
    virtual void _mouseExited(int x, int y)                             { return; };
    virtual void _windowResized(int w, int h)                           { return; };
    virtual void _gotMessage(ofMessage msg)                             { return; }; // openframeworks arbitrary message string
    virtual void _dragEvent(ofDragInfo dragInfo)                        { return; }; // of file dragged into window event

    // tags
    const vector<std::string>& getTags() const                          { return tags; }
    virtual void addTag(const std::string& tag)                         { TagManager::applyTag(this, tag); };
            // { if (!hasTag(tag)) tags.push_back(tag); };
    virtual bool hasTag(const std::string& tag) const                   { return std::find(tags.begin(), tags.end(), tag) != tags.end(); };
    virtual void removeTag(const std::string& tag)
    {
        auto it = std::find(tags.begin(), tags.end(), tag);
        if (it != tags.end())
            tags.erase(it);
    }

    // getters
    glm::vec3 getPosition() const                                       { return position; };
    ofQuaternion getRotation() const                                    { return rotation; };
    glm::vec3 getScale() const                                          { return scale; };
    glm::vec3 getTranslation() const                                    { return translation + position; };
    virtual const ofMesh& getMesh() const                               { return mesh; }; // can override for custom or strange meshes
    static Entity* getEntityById(int id)                                { return idToEntityMap[id]; };

    // helpers
    ofMesh copyMesh() const                                             { return mesh; };
    long long getId() const                                             { return hashId; };
    ofMatrix4x4 getTransformationMatrix() const;
};

#endif
