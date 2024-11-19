#include "InputManager.h"
#include <iostream>

#include "ofMain.h"

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

void InputManager::ofKeyPressed(int key)
{
    if(debugInput) std::cout << "KeyPress: " << key << " ";//std::endl; // easier to visually understand without it
    if(mapKeys[key] != 0)
    {
        key = mapKeys[key];
        if(debugInput) std::cout << "-> " << key << std::endl;
    }
    if (key > NUM_KEYS) return;
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
    if (key > NUM_KEYS) return;
    keys[key] = false;
    pressed[key] = false;
}
