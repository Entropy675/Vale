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

bool InputManager::getPressedOnce(int key, Entity& e)
{
    return getPressedOnce(key, e.inputContext);
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
    if (!activeEntities) return;

    if(debugInput)
    {
        std::cout << "Input Manager: Mouse Moved (" << mouse.scrollX << ", " << mouse.scrollY << ")."
        << " " << mouse.button << " " << mouse.modifiers << " " << mouse.type << std::endl;
    }

    for (Entity* entity : *activeEntities)
        entity->_mouseMoved(mouse);
}

void InputManager::ofMouseDragged(int x, int y, int button)
{
    if (!activeEntities) return;

    if(debugInput) std::cout << "Input manager detected mouse dragged window at position: (" << x << ", " << y << ") with button: " << button << "." << std::endl;

    for (Entity* entity : *activeEntities)
        entity->_mouseDragged(x, y, button);
}

void InputManager::ofMousePressed(int x, int y, int button)
{
    if (!activeEntities) return;
    //for (Entity* e : *activeEntities)
    //    e->mousePressed(x, y, button);
    if(debugInput) std::cout << "Input manager detected mouse pressed window at position: (" << x << ", " << y << ") with button: " << button << "." << std::endl;


    for (Entity* entity : *activeEntities)
        entity->_mousePressed(x, y, button);
}

void InputManager::ofMouseReleased(int x, int y, int button)
{
    if (!activeEntities) return;

    if(debugInput) std::cout << "Input manager detected mouse released window at position: (" << x << ", " << y << ") with button: " << button << "." << std::endl;

    for (Entity* entity : *activeEntities)
        entity->_mouseReleased(x, y, button);
}

void InputManager::ofMouseEntered(int x, int y)
{
    if (!activeEntities) return;

    if(debugInput) std::cout << "Input manager detected mouse entering window at position: (" << x << ", " << y << ")." << std::endl;

    for (Entity* entity : *activeEntities)
        entity->_mouseEntered(x, y);
}

void InputManager::ofMouseExited(int x, int y)
{
    if (!activeEntities) return;

    if(debugInput) std::cout << "Input manager detected mouse leaving window at position: (" << x << ", " << y << ")." << std::endl;

    for (Entity* entity : *activeEntities)
        entity->_mouseExited(x, y);
}

void InputManager::ofWindowResized(int w, int h)
{
    if (!activeEntities) return;

    if(debugInput) std::cout << "Input manager recieved window resized event, new width: " << w << " new height: " << h;

    for (Entity* entity : *activeEntities)
        entity->_windowResized(w, h);
}

void InputManager::ofDragEvent(ofDragInfo dragInfo)
{
    if (!activeEntities) return;

    if(debugInput)
    {
        std::cout << "InputManager recieved ofDragInfo (dragged into window file paths):" << std::endl;
        for (std::string& i : dragInfo.files)
            std::cout << i << std::endl;
        std::cout << "At position: " << dragInfo.position << std::endl;
    }

    for (Entity* entity : *activeEntities)
        entity->_dragEvent(dragInfo);
}

void InputManager::ofGotMessage(ofMessage msg)
{
    if (!activeEntities) return;

    if(debugInput)
    {
        std::cout << "InputManager recieved ofMessage:" << std::endl;
        std::cout <<  msg.message << std::endl;
    }

    for (Entity* entity : *activeEntities)
        entity->_gotMessage(msg);
}

