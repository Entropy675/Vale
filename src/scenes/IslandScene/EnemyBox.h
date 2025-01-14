#ifndef ENEMYBOX_H__
#define ENEMYBOX_H__

#include "Enemy.h"

class EnemyBox : public Enemy
{
private:
    Player* player = nullptr;

public:
    EnemyBox(glm::vec3 pos = glm::vec3(0, 0, 0), std::vector<Player*>* plrs = nullptr, int hp = 100, int ms = 1)
        : Enemy(pos, plrs, hp, ms)
    {
        addTag("EnemyBox");
    }

    Entity* clone() const override { return new EnemyBox(position, players, health, moveSpeed); };
    void _setPlayersList(std::vector<Player*>* plrs) override;
    void _collision(PhysicsEntity& target) override;
    void _input() override;
    void _setup() override;
    void _update() override;
    void _draw() override;
};

#endif
