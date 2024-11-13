#include "InputManager.h"
#include <iostream>

#include "ofMain.h"

InputManager::InputManager()
{
    
}

InputManager::~InputManager()
{
    
}

bool InputManager::get(int key) const
{
    // this is a hacky work around, we need to compensate for shift changing chars
    if (keys[OF_KEY_SHIFT] && key != OF_KEY_SHIFT)
    {
        if(key + 32 >= NUM_KEYS) return false;
        key += 32;
        if(debugInput) std::cout << "Modified KeyGet: " << key << " ";
    }
    if ( key < 0 && key > NUM_KEYS) 
    {
        std::cout << "Invalid Key: " << key << " cannot fit in bounds." << std::endl;
        return false;
    }
    //std::cout << "Key: " << key << "Status: " << keys[key] << std::endl;
    return keys[key];
}

void InputManager::ofKeyPressed(int key)
{
    if(debugInput) std::cout << "KeyPress: " << key << " ";//std::endl;
    if (keys[OF_KEY_SHIFT] && key != OF_KEY_SHIFT) 
    {
        if(key + 32 >= NUM_KEYS) return;
        key += 32; 
        if(debugInput) std::cout << "Modified KeyPress: " << key << " ";
    }
    keys[key] = true;
}

void InputManager::ofKeyReleased(int key)
{
    if(debugInput) std::cout << "KeyRelease: " << key << std::endl;
    if (keys[OF_KEY_SHIFT] && key != OF_KEY_SHIFT) 
    {
        if(key + 32 >= NUM_KEYS) return;
        key += 32;
        if(debugInput) std::cout << "Modified KeyRelease: " << key << " ";
    }
    keys[key] = false;
}
