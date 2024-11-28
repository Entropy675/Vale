#include "InputManager.h"
#include <iostream>

#include "ofMain.h"

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

bool InputManager::getPressedOnce(int key, bool (&pressedContext)[NUM_KEYS])
{
    if (key < 0 || key >= NUM_KEYS) return false;
    if (!pressed[key])
    {
        pressedContext[key] = false;
        return false;
    }
    if (pressedContext[key]) return false;
    pressedContext[key] = true;
    return pressed[key];
}

bool InputManager::getPressedOnce(int key, Entity& ent)
{
    return getPressedOnce(key, ent.inputContext);
}

bool InputManager::getPressedOnce(int key)
{
    return getPressedOnce(key, defaultInputContext);
}

void InputManager::ofKeyPressed(int key)
{
    if(debugInput) std::cout << "KeyPress: " << (char)key << " ";//std::endl; // easier to visually understand without it
    if(mapKeys[key] != 0)
    {
        key = mapKeys[key];
        if(debugInput) std::cout << "-> " << (char)key << std::endl;
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
    if(debugInput) std::cout << "KeyRelease: " << (char)key << " ";
    if(mapKeys[key] != 0)
    {
        key = mapKeys[key];
        if(debugInput) std::cout << "-> " << (char)key << std::endl;
    }
    if(debugInput) std::cout << std::endl;
    if (key < 0 || key >= NUM_KEYS) return;
    keys[key] = false;
    pressed[key] = false;
}
