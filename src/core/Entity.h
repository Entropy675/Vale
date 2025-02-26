#ifndef ENTITY_H__
#define ENTITY_H__
#include "defs.h"
#include "ofMain.h"
#include "ofTexture.h"
#include "TagManager.h"

// #define PRINTALLENTITIES
class InputManager;

// an entity is simply a valid tag/id combo + potential input handling, kept track of by idToEntityMap. 
class Entity
{
    friend class InputManager; // for input context direct access
    friend class TagManager;   // for apply tags direct access

private:
    static long long uniqueCounter;
    static std::unordered_map<int, Entity*> idToEntityMap; // consider replacing with tuple for low values
    bool inputContext[NUM_KEYS] = {};

protected:
    bool setupDone = false;
    glm::vec3 position = glm::vec3(0, 0, 0);
    const long long hashId;
    std::vector<std::string> tags;
    InputManager* inputManager = nullptr;
    
public:
    // Constructors
    Entity(glm::vec3 position = glm::vec3(0, 0, 0));
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

    // ------
    // optional, if your entity or its children need input
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

    // ------
    
    // Tags
    bool hasTag(const std::string& tag) const;
    bool addTag(const std::string& tag);
    virtual void removeTag(const std::string& tag); 
    // virtual in case of child supertype special cleanup (remember to call parent removeTag) 
    // (TODO: reconsider this, might be only PhysicsEntity that needs this)
    
    // getters
    const std::vector<std::string>& getTags() const                     { return tags; };
    long long getId() const                                             { return hashId; };
    glm::vec3 getPosition() const                                       { return position; };
    static Entity* getEntityById(int id)                                { return idToEntityMap[id]; };
    static const std::unordered_map<int, Entity*>& getIdToEntityMap()   { return idToEntityMap; };
};

#endif
