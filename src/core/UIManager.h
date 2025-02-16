#ifndef UIMANAGER_H__
#define UIMANAGER_H__

#include "defs.h"
#include "Entity.h"
#include "Scene.h"

#define CLAY_IMPLEMENTATION
#include "clay.h"


class UIManager : public Entity
{
public:
    UIManager();
    ~UIManager();
    
    void render();
    
private:
    

};





#endif
