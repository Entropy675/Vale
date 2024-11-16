#ifndef INPUTMANAGER_H__
#define INPUTMANAGER_H__

#define NUM_KEYS    4096


class InputManager
{
private:
    bool keys[NUM_KEYS] = {0};
    int mapKeys[NUM_KEYS] = {0};
    bool debugInput = true;

public:
    InputManager();
    ~InputManager();
    
    // another way of accessing input...
    const bool (&getInputArray() const)[NUM_KEYS]   { return keys; }
    bool get(int key) const                         { return keys[key]; };
    void map(int fromKey, int toKey)                { mapKeys[fromKey] = toKey; };
    void unmap(int key)                             { mapKeys[key] = 0; };
    void ofKeyPressed(int key);
    void ofKeyReleased(int key);
    
};

#endif
