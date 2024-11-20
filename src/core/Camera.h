#ifndef CAMERA_H__
#define CAMERA_H__

#include "Entity.h"
#include "Player.h"

class Camera : public Entity 
{
	
private:
    ofCamera camera; 
    Player* currPlayer;

public:
    Entity* clone() const override;
    void _setup() override;
    void _update() override;
    void _draw() override;
};

#endif