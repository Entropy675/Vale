#ifndef ISLANDSCENE_H__
#define ISLANDSCENE_H__

#include "Entity.h"
#include "Scene.h"

// ----------  Scene objects  ----------
class OceanObject : public Entity
{
    float noiseScale; // how big is noise
    float spread; // how far apart is each node
    float noiseZ; // z offset
    glm::vec3 lightDir;
    std::vector<glm::vec3> vertices;
    ofQuaternion waterRotation;
    glm::vec3 dimensions = glm::vec3(500, 500, 10);
    
	// helpers
	void updateNormals();
	float generateWaveHeight(float x, float y);

public:
    OceanObject(glm::vec3 pos = glm::vec3(0, 0, -330), float nScale = 3.8f, float sp = 130.0f);
	~OceanObject();
	
    void _setup() override;
    void _update() override; 
    void _draw() override;
};

class IslandObject : public Entity
{
    ofSpherePrimitive stone;
	ofMaterial stoneMaterial;
    float radius;
    int resolution;
    float spread;
    float noiseScale;
    glm::vec3 dimensions = glm::vec3(100, 100, 10);

public:
    IslandObject(glm::vec3 pos = glm::vec3(0, 0, 0), float rad = 700, int res = 64, float sp = 10, float ns = 3.5f);
	~IslandObject();
	
    void _setup() override;
    void _update() override; 
    void _draw() override;
};


// ---------- Scene definition ----------
class IslandScene : public Scene
{
public:
	// cleanup is handled in parent 
	IslandScene() 
	{
		sceneObjects.push_back(new OceanObject());
		sceneObjects.push_back(new IslandObject());
	};
};


#endif
