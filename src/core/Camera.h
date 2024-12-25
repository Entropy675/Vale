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
    float moveSpeed = 10;
    int currPlayer;
    vector <Player*> playersInScene;

public:
    Camera(glm::vec3 offset) : PhysicsEntity(offset) {}
    Entity* clone() const override { return new Camera(position); };
    void _setup() override;
    void _update() override;
    void _draw() override {};
    void _input() override;
    void _mouseMoved(ofMouseEventArgs&) override;
    void _collision(PhysicsEntity& ) override {  };
    void camBegin();
    void camEnd();

    bool setPlayer(std::vector<PhysicsEntity*>* loadedEntities);

};

#endif
