#ifndef RAYTRACINGSCENE_H__
#define RAYTRACINGSCENE_H__

#include "Scene.h"
#include "Entity.h"
#include "ofImage.h"

class RaytracingObject : public Entity
{
private:
    ofImage renderedImage; // To store the ray-traced image
    int imageWidth;
    int imageHeight;

    struct Sphere
    {
        glm::vec3 center;
        float radius;
        ofColor color; // Base color of the sphere
        float ambient, diffuse, specular, shininess;
        ofMaterial material;
    };

    struct Light
    {
        glm::vec3 position;
        ofColor color;
    };

    std::vector<Sphere> spheres;
    std::vector<Light> lights;

    void render(); // Perform the ray tracing
    ofColor traceRay(const glm::vec3& origin, const glm::vec3& direction);
    bool intersectSphere(const glm::vec3& origin, const glm::vec3& direction, const Sphere& sphere, float& t);
    glm::vec3 computeLighting(const glm::vec3& point, const glm::vec3& normal, const glm::vec3& viewDir, const Sphere& sphere);

public:
    RaytracingObject(glm::vec3 position = glm::vec3(-50, -1, 300), int width = 800, int height = 600);
    ~RaytracingObject();

    void _setup() override;
    void _update() override;
    void _draw() override;

    Entity* clone() const override { return new RaytracingObject(position, imageWidth, imageHeight); }
};


// ---------- Scene definition ----------
class RaytracingScene : public Scene
{
public:
    RaytracingScene(EnvironmentObject& env) : Scene(env) { addEntity(new RaytracingObject()); };
};

#endif // RAYTRACINGSCENE_H__
