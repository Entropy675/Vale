#ifndef DEFAULTPLAYER_H__
#define DEFAULTPLAYER_H__

#include "Player.h"

class DefaultPlayer : public Player
{
private: 
    ofSpherePrimitive pillBody; 
public: 
    DefaultPlayer::DefaultPlayer(glm::vec3 pos);
    ~DefaultPlayer();
    void _setup() override;
    void _update() override;
    void _draw() override;
    Entity* clone() const override;

    virtual void _collision(PhysicsEntity& target) override; 
};

#endif

