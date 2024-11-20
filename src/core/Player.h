#ifndef PLAYER_H__
#define PLAYER_H__

#include "PhysicsEntity.h"


class Player : public PhysicsEntity
{
    protected:
        float health;
        float radius;
        int resolution;
        ofTexture playerSkin; 
        float playerFloor;

    public:
        // Each player will have their own unique features. 
        Player(glm::vec3 pos, float health, float radius, int resolution, float playerFloor)
            : PhysicsEntity(pos), health(health), radius(radius), resolution(resolution), playerFloor(playerFloor) {}
};	

#endif