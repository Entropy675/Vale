#ifndef OBJECTS_H__
#define OBJECTS_H__

#include "Entity.h"

class OceanObject : public Entity
{
    float noiseScale = 3.5f; // how big is noise
    float spread = 130.0f; // how far apart is each node
    float noiseZ = 0.0f; // z offset
    
	// helpers
	float generateWaveHeight(float x, float y);

public:
    OceanObject(glm::vec3 dim = glm::vec3(100, 100, 10), float nScale = 3.8f, float sp = 130.0f);
	~OceanObject();
	
    void setup() override;
    void update() override; 
    void draw() override;
};

class IslandObject : public Entity
{
public:
    void setup() override;
    void update() override; 
    void draw() override;
};

/*

class DullObject : public Entity
{
public:
    void setup() override;
    void update() override; 
    void draw() override;
};

class WoodObject : public Entity
{
public:
    void setup() override;
    void update() override; 
    void draw() override;
};

*/


#endif
