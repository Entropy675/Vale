#ifndef PHYSICSCONTROLLER__H_
#define PHYSICSCONTROLLER__H_

#include "PhysicsEntity.h"

class PhysicsController : public Entity
{
	private:
	std::vector<PhysicsEntity*> physicsObjects; // not mem managed by this class
	
	public:
    PhysicsController();
	~PhysicsController();
	
    void _setup() override;
    void _update() override; 
    void _draw() override;
};

#endif

