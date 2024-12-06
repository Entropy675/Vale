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
    vector <Player*>* playersInScene; 
    glm::vec3 startingPos;

public:
    Camera(glm::vec3 startingPos) : startingPos(startingPos) {}
    Entity* clone() const override { return new Camera(startingPos); };
    void _setup() override;
    void _update() override;
    void _draw() override {};
    void _input() override;
    void _collision(PhysicsEntity& ) override {  };
    void camBegin(); 
    void camEnd(); 
    void mouseMoved(ofMouseEventArgs&);
    bool setPlayer(Player*); 
    void setPlayersInScene(std::vector<Player*>* players) { playersInScene = players;
    }

};

#endif