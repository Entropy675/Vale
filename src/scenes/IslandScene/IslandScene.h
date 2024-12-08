#ifndef ISLANDSCENE_H__
#define ISLANDSCENE_H__

#include "Entity.h"
#include "Scene.h"
#include "Hero.h"

// ----------  Scene objects  ----------
class OceanObject : public Entity
{
    float noiseScale; // how big is noise
    float spread; // how far apart is each node
    float noiseZ; // z offset
    ofMaterial material;
    glm::vec3 lightDir;
    std::vector<glm::vec3> vertices;
    ofQuaternion waterRotation;
    glm::vec3 dimensions = glm::vec3(500, 10, 500);
    int floatHeightOffset = 25;

    // helpers
    void updateNormals();
    float generateWaveHeight(float x, float y);

public:
    OceanObject(glm::vec3 pos = glm::vec3(0, -330, 0), float nScale = 15.5f, float sp = 450.0f);
    ~OceanObject();

    Entity* clone() const override;
    void _setup() override;
    void _update() override;
    void _draw() override;
};

class IslandObject : public Entity
{
    ofSpherePrimitive stone;
    ofMaterial stoneMaterial;
    ofMaterial material;
    float radius;
    int resolution;
    float spread;
    float noiseScale;
    glm::vec3 dimensions = glm::vec3(100, 10, 100);

public:
    IslandObject(glm::vec3 pos = glm::vec3(0, 0, 0), float rad = 700, int res = 32, float sp = 10, float ns = 3.5f);
    ~IslandObject();

    Entity* clone() const override;
    void _setup() override;
    void _update() override;
    void _draw() override;
};

class MountainObject : public Entity
{
    ofMaterial material;
    float radius;
    int resolution;
    float spread;
    float noiseScale;
    glm::vec3 dimensions = glm::vec3(100, 10, 100);

public:
    MountainObject(glm::vec3 pos = glm::vec3(0, 0, 0), float rad = 700, int res = 32, float sp = 10, float ns = 3.5f);
    ~MountainObject();

    Entity* clone() const override;
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
    //  defaultCameraOffset = glm::vec3(0.0f, -700.0f, 0.0f);

        TagManager::addTag("ocean");
        TagManager::addTag("island");

        addEntity(new OceanObject());
        addEntity(new IslandObject());
        // mountainObject WIP 
        //addEntity(new MountainObject());


        TagManager::addTag("hero");
        Hero* heroTest1 = new Hero("Gilbert", glm::vec3(600.0f, 1000.0f, 100.0f), 10.0f, 1500);


         Hero* heroTest2 = new Hero("Filbert", glm::vec3(100.0f, 1000.0f, 600.0f), 10.0f, 1000);


        // camera and player association test
        addEntity(heroTest1);
        addEntity(heroTest2);

    };
};


#endif
