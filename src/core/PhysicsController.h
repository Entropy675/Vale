#ifndef PHYSICSCONTROLLER__H_
#define PHYSICSCONTROLLER__H_

#include "PhysicsEntity.h"

class PhysicsController : public Entity
{
	private:
	std::vector<PhysicsEntity*> physicsObjects;
	
	public:
    PhysicsController();
	~PhysicsController();
	
    void _setup() override;
    void _update() override; 
    void _draw() override;
};

#endif

