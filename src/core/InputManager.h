#ifndef INPUTMANAGER_H__
#define INPUTMANAGER_H__
#include <vector>
#include <iostream>

#define NUM_KEYS    4096


class InputManager
{
private:
    bool keys[NUM_KEYS] = {};
    int mapKeys[NUM_KEYS] = {};
    std::vector<std::pair<int, float>> timeoutMap;
    
    bool debugInput = true;
    float maxTimeout = 3.0f;
    float decrementkeyPressTime = 0.1f; 

public:
    InputManager();
    ~InputManager();

    void update(); 
    
    // another way of accessing input...
    const bool (&getInputArray() const)[NUM_KEYS]   { return keys; }
    bool getPressed(int key) const
    {
        if (key < 0 || key >= NUM_KEYS) return false;

        for (int i = 0; i < timeoutMap.size(); i++)
        {
            // 3 second minimum on each key press, if time is >= 2.6, we consider the timer as the key just pressed so we still want to return true, can play around with that 0.4
            bool timerJustStarted = timeoutMap.at(i).second >= maxTimeout - (decrementkeyPressTime + 0.4);            
            if (timeoutMap.at(i).first == key && !timerJustStarted)
            {
                return false;
            }
        }
        return keys[key];
    };

    // have def here and put code in C++ file 

    bool getHeld(int keyPressed) const 
    {
        if (keyPressed < 0 || keyPressed >= NUM_KEYS) return false;
        return keys[keyPressed];
    };
    void map(int fromKey, int toKey)                
    {
        if (fromKey < 0 || fromKey >= NUM_KEYS) return;
        mapKeys[fromKey] = toKey; 
    };
    void unmap(int keyPressed)                             
    {
        if (keyPressed < 0 || keyPressed >= NUM_KEYS) return;
        mapKeys[keyPressed] = 0; 
    };
    void ofKeyPressed(int keyPressed);
    void ofKeyReleased(int keyPressed);
   
};

#endif
