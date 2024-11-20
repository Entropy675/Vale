#include "InputManager.h"
#include <iostream>

#include "ofMain.h"

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

bool InputManager::getPressedOnce(int key, bool (&pressedImpulse)[NUM_KEYS])
{
    if (key < 0 || key >= NUM_KEYS) return false;
    if (!pressed[key])
    {
        pressedImpulse[key] = false;
        return false;
    }
    if (pressedImpulse[key]) return false;
    pressedImpulse[key] = true;
    return pressed[key];
}

bool InputManager::getPressedOnce(int key, Entity& ent)
{
    return getPressedOnce(key, ent.getInputContext());
}

void InputManager::ofKeyPressed(int key)
{
    if(debugInput) std::cout << "KeyPress: " << key << " ";//std::endl; // easier to visually understand without it
    if(mapKeys[key] != 0)
    {
        key = mapKeys[key];
        if(debugInput) std::cout << "-> " << key << std::endl;
    }
    if (key < 0 || key >= NUM_KEYS) return;
    if (keys[key])
    {
        if (pressed[key]) pressed[key] = false; // if held, only first iter is pressed true
        return;
    }
    keys[key] = true;
    pressed[key] = true;
}

void InputManager::ofKeyReleased(int key)
{
    if(debugInput) std::cout << "KeyRelease: " << key << " ";
    if(mapKeys[key] != 0)
    {
        key = mapKeys[key];
        if(debugInput) std::cout << "-> " << key << std::endl;
    }
    if(debugInput) std::cout << std::endl;
    if (key < 0 || key >= NUM_KEYS) return;
    keys[key] = false;
    pressed[key] = false;
}
