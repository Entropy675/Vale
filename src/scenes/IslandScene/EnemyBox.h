#ifndef ENEMYBOX_H__
#define ENEMYBOX_H__

#include "PhysicsEntity.h"

class EnemyBox : public PhysicsEntity
{
private:
    int health;
    int moveSpeed;

    // should not be here
    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;
public:
    EnemyBox(glm::vec3 pos = glm::vec3(0, 0, 0), int hp = 100, int ms = 10)
        : PhysicsEntity(position), health(hp), moveSpeed(ms)
    {
        TagManager::addTag("Enemy");
        TagManager::addTag("Box");
        addTag("Enemy");
        addTag("Box");
    }

    Entity* clone() const override { return new EnemyBox(position, health, moveSpeed); };
    void _collision(PhysicsEntity& target) override;
    void _input() override;
    void _setup() override;
    void _update() override;
    void _draw() override;
};

#endif
