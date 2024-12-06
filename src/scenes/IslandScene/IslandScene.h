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


class Player : public PhysicsEntity {
protected:
    std::string name;       
    float health;           
    float radius;           
    int resolution;       
    ofTexture playerSkin; 
    float playerFloor;      
    float moveSpeed;       

public:
    // Constructor to initialize all members
    Player(glm::vec3 pos, std::string name, float health, float radius, int resolution, float playerFloor, float moveSpeed)
        : PhysicsEntity(pos),
        name(name),
        health(health),
        radius(radius),
        resolution(resolution),
        playerFloor(playerFloor),
        moveSpeed(moveSpeed),
        playerSkin(playerSkin) {
        addTag("player"); // Mark this entity as a player
    }

    Entity* clone() const override { return new Player(*this); }
    void _collision(PhysicsEntity& target) override { }
    void _input() override { }
    void _setup() override { }
    void _update() override { }
    void _draw() override { }

    float getPlayerSpeed() const { return moveSpeed; }
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

        Player* playerTest = new Player(
            glm::vec3(0.0f, 0.0f, 0.0f), // Position
            "Player1",                   // Name
            100.0f,                      // Health
            1.0f,                        // Radius
            32,                          // Resolution
            0.0f,                        // Player floor level
            5.0f                       // Movement speed
        );

        scenePhysicsObjects.push_back(playerTest);

    
    };
};


#endif