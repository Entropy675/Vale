#ifndef HERO_H__
#define HERO_H__

#include "Player.h"

class Hero : public Player {
private:
    bool isOnGround = true;

    const glm::vec3 gravity = glm::vec3(0, -29.6f, 0);
    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;
    ofColor heroColor;  // Member variable to store the color

public:
    Hero(std::string name, glm::vec3 pos, float health, float moveSpeed, ofColor color)
        : Player(name, pos, health, moveSpeed), heroColor(color) 
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