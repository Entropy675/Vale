#ifndef INPUTMANAGER_H__
#define INPUTMANAGER_H__
#include <vector>
#include <iostream>
#include "defs.h"
#include "Entity.h"
#include "PhysicsEntity.h"

/*
 * InputManager
 * Manages keyboard input states with support for:
 * - PressedOnce: Triggered only once when a key is initially pressed.
 * - Pressed: Triggered between the initial press and when the key is considered "held."
 * - Held: Triggered as long as the key remains pressed.
 *
 * Includes support for key mapping and context-specific input handling.
 */

class InputManager
{
private:
    bool keys[NUM_KEYS] = {};
    bool pressed[NUM_KEYS] = {}; // could be replaced with bitfields for efficiency
    bool defaultInputContext[NUM_KEYS] = {}; // default, everything using this is bound together...

    int mapKeys[NUM_KEYS] = {};

    std::vector<Entity*>* activeEntities = nullptr; // for callbacks
    std::vector<PhysicsEntity*>* activePhysicsEntities = nullptr;

    bool debugInput = false;
    ofMouseEventArgs mouseArgs;

public:
    InputManager();
    ~InputManager();
    void setActiveEntities(std::vector<Entity*>* entities) { activeEntities = entities; }
    void setActivePhysicsEntities(std::vector<PhysicsEntity*>* physicsEntities) { activePhysicsEntities = physicsEntities; }

    // another way of accessing input... a very raw way...
    const bool (&getInputArray() const)[NUM_KEYS]   { return keys; }

    // ** Use these if you want something to happen once
    // This should be based off the context of the caller, can be based on any context thats useful
    // Shared contexts share key presses - the functionality triggered in response occurs for both.
    bool getPressedOnce(int key, bool (&pressedContext)[NUM_KEYS]);
    bool getPressedOnce(int key, Entity& ent); // use this for local context
    bool getPressedOnce(int key); // uses input manager context (may be unreliable anywhere other then ofApp.cpp)

    // ** Use this if you want something to happen a couple times depending on how long the key is held
    // more specifically: returns true for the gap between when a key is pressed and considered held,
    // false immediately when released early. This allows you to trigger a function a couple times
    // depending on how long the user held a key.
    bool getPressed(int key)
    {
        if (key < 0 || key >= NUM_KEYS) return false;
        return pressed[key];
    };

    // ** Use this if you want something to happen while a key is held down
    // Immediately triggers, then triggers once a fraction of time passes.
    // During this time, getPressed for this key returns true, see above.
    bool getHeld(int keyPressed) const
    {
        if (keyPressed < 0 || keyPressed >= NUM_KEYS) return false;
        return keys[keyPressed];
    };
    void map(int fromKey, int toKey)
    {
        if (fromKey < 0 || fromKey >= NUM_KEYS) return;
        mapKeys[fromKey] = toKey;
    };
    void unmap(int keyPressed)
    {
        if (keyPressed < 0 || keyPressed >= NUM_KEYS) return;
        mapKeys[keyPressed] = 0;
    };

    // forwarded input functions
    void ofKeyPressed(int key);
    void ofKeyReleased(int key);
    void ofMouseMoved(ofMouseEventArgs& mouse);
    void ofMouseDragged(int x, int y, int button);
    void ofMousePressed(int x, int y, int button);
    void ofMouseReleased(int x, int y, int button);
    void ofMouseEntered(int x, int y);
    void ofMouseExited(int x, int y);
    void ofWindowResized(int w, int h);
    void ofGotMessage(ofMessage msg);
    void ofDragEvent(ofDragInfo dragInfo);     // openframeworks file dragged into window event
};

#endif
