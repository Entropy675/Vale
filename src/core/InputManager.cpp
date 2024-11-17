#include "InputManager.h"
#include <iostream>

#include "ofMain.h"

InputManager::InputManager()
{
    
}

InputManager::~InputManager()
{

}

void InputManager::update() {
    for (int i = 0; i < timeoutMap.size(); i++) {
        timeoutMap.at(i).second -= decrementkeyPressTime;

        // Print the current key and its timeout value
        std::cout << "Key: " << timeoutMap.at(i).first
            << ", Timeout: " << timeoutMap.at(i).second << std::endl;

        if (timeoutMap.at(i).second <= 0) {
            std::cout << "Removing Key: " << timeoutMap.at(i).first << std::endl;
            timeoutMap.erase(timeoutMap.begin() + i);
            i--; // Adjust the index 
        }
    }

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
    keys[key] = true;
    
    bool check = false;
    for (pair lol : timeoutMap) 
        if (lol.first == key) check = true; 

    if (!check) timeoutMap.push_back(pair<int, float>(key, maxTimeout));
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
}
