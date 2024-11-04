#ifndef OBJECTS_H__
#define OBJECTS_H__

#include "Entity.h"
#include "Scene.h"
#include <vector>

class OceanObject : public Entity
{
    float noiseScale; // how big is noise
    float spread; // how far apart is each node
    float noiseZ; // z offset
    std::vector<glm::vec3> vertices;
    
    
	// helpers
	void updateNormals();
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
    ofSpherePrimitive stone;
	ofMaterial stoneMaterial;
    float radius;
    int resolution;
    float spread;
    float noiseScale;

public:
    IslandObject(glm::vec3 dim = glm::vec3(100, 100, 10), float rad = 700, int res = 64, float sp = 10, float ns = 3.5f);
	~IslandObject();
	
    void setup() override;
    void update() override; 
    void draw() override;
};


class IslandScene : public Scene
{
public:
	OceanObject* ocean;
	IslandObject* island;
	
	IslandScene();
	void loadScene(std::vector<Entity*>* list) override;
};

#endif
