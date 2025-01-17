#ifndef PLAYER_H__
#define PLAYER_H__

#include "PhysicsEntity.h"
#include "InputManager.h"
#include "Scene.h"

class Camera;

class Player : public PhysicsEntity
{
protected:
    std::string name;
    float health;
    float moveSpeed;
    bool cameraAssigned = false;
    glm::vec3 playerOrientation = glm::vec3(0, 0, 0);

public:
    Player(std::string name, glm::vec3 pos = glm::vec3(0, 0, 0), float health = 100.0f, float moveSpeed = 50.0f)
        : PhysicsEntity(pos), name(name), health(health), moveSpeed(moveSpeed) { addTag("player"); };

    virtual Entity* clone() const = 0;
    virtual void _collision(PhysicsEntity& target) = 0;
    virtual void _setup() = 0;
    virtual void _update() = 0;
    virtual void _draw() = 0;


    std::string getPlayerName() const                   { return name; };
    float getPlayerSpeed() const                        { return moveSpeed; };

    void toggleCameraAssignment() { cameraAssigned = !cameraAssigned; }

    void setPlayerOrientation(glm::vec3 facingDirection) { playerOrientation = facingDirection; }
};
#endif
