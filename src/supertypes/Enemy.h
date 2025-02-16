#ifndef ENEMY_H__
#define ENEMY_H__

#include "PhysicsEntity.h"
#include "Player.h"

class Enemy : public PhysicsEntity
{
protected:
    std::vector<Player*>* players;
    int health;
    int moveSpeed;
    ofMaterial material;

public:
    Enemy(glm::vec3 pos = glm::vec3(0, 0, 0), std::vector<Player*>* plrs = nullptr, int hp = 100, int ms = 10)
        : PhysicsEntity(pos), players(plrs), health(hp), moveSpeed(ms)
    {
        addTag("Enemy");
        if (plrs != nullptr) _setPlayersList(plrs);
    }

    // update is not called unless there is a player in the scene (maybe this will be remove later, for now I guaruntee it)
    void update() override
    {
        if (players == nullptr) return;
        Entity::update();
    }

    virtual void _setPlayersList(std::vector<Player*>* plrs) {}; // callback for if you want to do something when this list is updated
    void setPlayersList(std::vector<Player*>* plrs)
    {
        players = plrs;
        _setPlayersList(plrs);
    }



};

#endif
