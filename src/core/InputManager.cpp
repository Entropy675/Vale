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
    if (key > NUM_KEYS || keys[key]) return;
    keys[key] = true;
    pressed[key] = frameOffset;
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
    pressed[key] = 0;
}
