#ifndef CAMERA_H__
#define CAMERA_H__

#include "PhysicsEntity.h"
#include "IslandScene.h"
#include "InputManager.h"
#include "defs.h"


class Camera : public PhysicsEntity 
{
	
private:
    ofCamera camera; 
    Player* currPlayer;

public:
    Camera(glm::vec3 startingPos = glm::vec3(0, 0, 0)) {};
    Entity* clone() const override { return new Camera(); };
    void _setup() override;
    void _update() override {};
    void _draw() override {};
    void _input() override;
    void _collision(PhysicsEntity& target) override {  };
    void camBegin(); 
    void camEnd(); 
    void mouseMoved(ofMouseEventArgs& mouseMovement);


};

#endif