#ifndef OBJECT_H__
#define OBJECT_H__

#include "Entity.h"

class Object : public Entity
{
public:
	void setup() override;
	void update() override;
	void draw() override;
};


#endif