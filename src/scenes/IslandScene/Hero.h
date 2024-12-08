#ifndef HERO_H__
#define HERO_H__

#include "Player.h"

class Hero : public Player {
private: 
    bool isOnGround = true;

    const glm::vec3 gravity = glm::vec3(0, -25.6f, 0);
    // should not be here
    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;
public:
    Hero(std::string name, glm::vec3 pos, float health, float moveSpeed)
        : Player(name, pos, health, moveSpeed)
    {
        addTag("hero");
    }

    Entity* clone() const override;
    void _collision(PhysicsEntity& target) override;
    void _input() override;
    void _setup() override;
    void _update() override;
    void _draw() override;

    void setPlayerJump() { isOnGround = false; }
};

#endif