#ifndef ENEMYBOX_H__
#define ENEMYBOX_H__

#include "PhysicsEntity.h"
#include "Player.h"

class EnemyBox : public PhysicsEntity
{
private:
    Player* player;
    int health;
    int moveSpeed;
    ofMaterial material;

    // should not be here
    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;
public:
    EnemyBox(glm::vec3 pos = glm::vec3(0, 0, 0), Player* plr = nullptr, int hp = 100, int ms = 10)
        : PhysicsEntity(pos), player(plr), health(hp), moveSpeed(ms)
    {
        addTag("Enemy");
        addTag("Box");
    }

    Entity* clone() const override { return new EnemyBox(position, player, health, moveSpeed); };
    void _collision(PhysicsEntity& target) override;
    void _input() override;
    void _setup() override;
    void _update() override;
    void _draw() override;
};

#endif
