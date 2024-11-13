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
    
    keys[key] = true;
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
    
    keys[key] = false;
}
