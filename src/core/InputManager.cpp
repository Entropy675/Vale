#include "InputManager.h"
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


void InputManager::ofMouseMoved(ofMouseEventArgs& mouse)
{
    std::cout << "Input Manager: Mouse Moved (" << mouse.scrollX << ", " << mouse.scrollY << ")."
    << " " << mouse.button << " " << mouse.modifiers << " " << mouse.type << std::endl;
}

void InputManager::ofMouseDragged(int x, int y, int button)
{
    std::cout << "Input manager detected mouse dragged window at position: (" << x << ", " << y << ") with button: " << button << "." << std::endl;
}

void InputManager::ofMousePressed(int x, int y, int button)
{
    //for (Entity* e : *activeEntities)
    //    e->mousePressed(x, y, button);
    std::cout << "Input manager detected mouse pressed window at position: (" << x << ", " << y << ") with button: " << button << "." << std::endl;
}

void InputManager::ofMouseReleased(int x, int y, int button)
{
    std::cout << "Input manager detected mouse released window at position: (" << x << ", " << y << ") with button: " << button << "." << std::endl;
}

void InputManager::ofMouseEntered(int x, int y)
{
    std::cout << "Input manager detected mouse entering window at position: (" << x << ", " << y << ")." << std::endl;
}

void InputManager::ofMouseExited(int x, int y)
{
    std::cout << "Input manager detected mouse leaving window at position: (" << x << ", " << y << ")." << std::endl;
}

void InputManager::ofWindowResized(int w, int h)
{
    std::cout << "Input manager recieved window resized event, new width: " << w << " new height: " << h;
}

void InputManager::ofDragEvent(ofDragInfo dragInfo)
{
    std::cout << "InputManager recieved ofDragInfo (dragged into window file paths):" << std::endl;
    for (std::string& i : dragInfo.files)
        std::cout << i << std::endl;
    std::cout << "At position: " << dragInfo.position << std::endl;
}

void InputManager::ofGotMessage(ofMessage msg)
{

    std::cout << "InputManager recieved ofMessage:" << std::endl;
    std::cout <<  msg.message << std::endl;
}

