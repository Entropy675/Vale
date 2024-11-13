#ifndef INPUTMANAGER_H__
#define INPUTMANAGER_H__

#define NUM_KEYS    4096

#include <mutex>

class InputManager
{
private:
    bool keys[NUM_KEYS] = {0};
    
    bool debugInput = true;

public:
    InputManager();
    ~InputManager();

    //const bool[NUM_KEYS]& getInputArray();
    // accessing keys through this array will cause crashes (without locking mutex), but you can have it...
    const bool (&getInputArray() const)[NUM_KEYS] 
    {
        return keys;
    }
    
    bool get(int key) const;
    void ofKeyPressed(int key);
    void ofKeyReleased(int key);
    
};

#endif
