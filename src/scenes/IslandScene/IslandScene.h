#ifndef ISLANDSCENE_H__
#define ISLANDSCENE_H__

#include "Entity.h"
#include "Scene.h"
#include "EnemyBox.h"
#include "Hero.h"

glm::vec3 generateRandomVector(float totalDistance);

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

// ---------- Scene definition ----------
class IslandScene : public Scene
{

public:
    // cleanup is handled in parent
    IslandScene()
    {
        addEntity(new OceanObject());
        addEntity(new IslandObject());

        Hero* hero1 = new Hero("Gilbert", glm::vec3(1800.0f, 1000.0f, 300.0f), 10.0f, 1500, ofColor::green);


        Hero* hero2 = new Hero("Filbert", glm::vec3(3000.0f, 1000.0f, 1200.0f), 10.0f, 1000, ofColor::blue);

        Hero* hero3 = new Hero("Tilbert", glm::vec3(300.0f, 1000.0f, 1800.0f), 10.0f, 1000, ofColor::red);

        Hero* hero4 = new Hero("Bilbert", glm::vec3(1200.0f, 1000.0f, 3000.0f), 10.0f, 1000, ofColor::purple);


        addEntity(hero1);
        addEntity(hero2);
        addEntity(hero3);
        addEntity(hero4);

        for (int i = 0; i < 25; i++)
        {
            glm::vec3 randomVector = generateRandomVector(3600);
            if (randomVector.y < 0)
                randomVector.y = -randomVector.y;
            randomVector.y -= 400;

            EnemyBox* enemyBox = new EnemyBox(randomVector);
            scenePhysicsObjects.push_back(enemyBox);
        }
    };
};


#endif
