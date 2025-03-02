#ifndef JOLFBASESCENE_H__
#define JOLFBASESCENE_H__

#include "Entity.h"
#include "Scene.h"

// ----------  Scene objects  ----------
class JolfOceanObject : public PhysicsEntity
{
    float noiseScale; // how big is noise
    float spread; // how far apart is each node
    float noiseZ; // z offset
    ofMaterial material;
    glm::vec3 lightDir;
    std::vector<glm::vec3> vertices;
    ofQuaternion waterRotation;
    glm::vec3 dimensions = glm::vec3(500, 10, 500);

    // helpers
    void updateNormals();
    float generateWaveHeight(float x, float y);

public:
    JolfOceanObject(glm::vec3 pos = glm::vec3(0, -330, 0), float nScale = 3.8f, float sp = 230.0f);
    ~JolfOceanObject();

    Entity* clone() const override;
    void _collision(PhysicsEntity& target) override;

    void _setup() override;
    void _update() override;
    void _draw() override;
};

// ---------- Scene definition ----------
class JolfBaseScene : public Scene
{
public:
    // cleanup is handled in parent
    JolfBaseScene(EnvironmentObject& env) : Scene(env)
    {
        addEntity(new JolfOceanObject());
        //sceneObjects.push_back(new IslandObject());
    };
};


#endif
