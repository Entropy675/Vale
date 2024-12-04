#include "RaytracingScene.h"
#include <algorithm>

glm::vec3 generateRandomVec(float totalDistance)
{
    float x = ofRandom(-totalDistance, totalDistance);
    float y = ofRandom(-totalDistance, totalDistance);

    while (x * x + y * y > totalDistance * totalDistance)
    {
        x = ofRandom(-totalDistance, totalDistance);
        y = ofRandom(-totalDistance, totalDistance);
    }

    float z = std::sqrt(totalDistance * totalDistance - x * x - y * y);

    // Randomly choose the sign of z to ensure the vector can point in all directions
    z *= (ofRandom(0.0f, 1.0f) > 0.5f) ? 1.0f : -1.0f;

    return glm::vec3(x, y, z);
}


RaytracingObject::RaytracingObject(glm::vec3 position, int width, int height)
    : Entity(position), imageWidth(width), imageHeight(height) {}

RaytracingObject::~RaytracingObject() {}
void RaytracingObject::_setup()
{
    // Shiny red sphere
    Sphere shinyRedSphere = { glm::vec3(1, 0, -5) + generateRandomVec(1), 1.0f, ofColor::red, 0.1f, 0.7f, 0.5f, 32.0f };
    shinyRedSphere.material.setShininess(128.0f);
    shinyRedSphere.material.setDiffuseColor(ofFloatColor::red);
    shinyRedSphere.material.setSpecularColor(ofFloatColor(1.0f, 0.5f, 0.5f));

    // Large shiny gold sphere
    Sphere shinyGoldSphere = { glm::vec3(-9, 2, -9) + generateRandomVec(1), 2.0f, ofColor(255, 215, 0), 0.5f, 0.6f, 1.0f, 128.0f };
    shinyGoldSphere.material.setShininess(200.0f);
    shinyGoldSphere.material.setDiffuseColor(ofFloatColor(1.0f, 0.843f, 0.0f)); // Gold tint
    shinyGoldSphere.material.setSpecularColor(ofFloatColor(1.0f, 0.9f, 0.7f));

    // Small matte purple sphere
    Sphere mattePurpleSphere = { glm::vec3(3, -0.5f, -4) + generateRandomVec(1), 0.75f, ofColor(128, 0, 128), 0.3f, 0.5f, 0.2f, 8.0f };
    mattePurpleSphere.material.setShininess(5.0f);
    mattePurpleSphere.material.setDiffuseColor(ofFloatColor(0.5f, 0.0f, 0.5f));
    mattePurpleSphere.material.setSpecularColor(ofFloatColor::black);

    // Large glass-like transparent sphere
    Sphere glassSphere = { glm::vec3(0, 0, -10) + generateRandomVec(1), 3.0f, ofColor(192, 192, 255), 0.6f, 0.4f, 0.9f, 64.0f };
    glassSphere.material.setShininess(300.0f);
    glassSphere.material.setDiffuseColor(ofFloatColor(0.8f, 0.8f, 1.0f));
    glassSphere.material.setSpecularColor(ofFloatColor(1.0f, 1.0f, 1.0f));

    // Add spheres to the scene
    spheres.push_back(shinyRedSphere);
    spheres.push_back(shinyGoldSphere);
    spheres.push_back(mattePurpleSphere);
    spheres.push_back(glassSphere);

    // Initialize lights to illuminate the scene
    lights.push_back({ glm::vec3(-10, 10, -10), ofColor::white });  // Top-left white light
    lights.push_back({ glm::vec3(0, 5, -3), ofColor(255, 69, 0) }); // orange light
    lights.push_back({ glm::vec3(-5, -5, -5), ofColor(0, 0, 255) }); // blue light from below

    render(); // Render the scene once during setup
}

void RaytracingObject::_update(){}

void RaytracingObject::_draw()
{
    renderedImage.draw(position.x, position.y);
}

void RaytracingObject::render()
{
    renderedImage.allocate(imageWidth, imageHeight, OF_IMAGE_COLOR);

    float aspectRatio = float(imageWidth) / imageHeight;
    float viewPlaneHalfWidth = tan(glm::radians(45.0f)); // FOV of 90 degrees
    float viewPlaneHalfHeight = viewPlaneHalfWidth / aspectRatio;

    glm::vec3 cameraOrigin = glm::vec3(0, 0, 0);

    for (int j = 0; j < imageHeight; ++j)
    {
        for (int i = 0; i < imageWidth; ++i)
        {
            // Map pixel to view plane
            float u = (2.0f * i / imageWidth - 1.0f) * viewPlaneHalfWidth;
            float v = (1.0f - 2.0f * j / imageHeight) * viewPlaneHalfHeight;

            glm::vec3 rayDir = glm::normalize(glm::vec3(u, v, -1));
            ofColor color = traceRay(cameraOrigin, rayDir);
            renderedImage.setColor(i, j, color);
        }
    }

    renderedImage.update(); // Finalize the image
}

ofColor RaytracingObject::traceRay(const glm::vec3& origin, const glm::vec3& direction)
{
    float closestT = std::numeric_limits<float>::max();
    const Sphere* hitSphere = nullptr;

    // Check intersections with all spheres
    for (const auto& sphere : spheres)
    {
        float t;
        if (intersectSphere(origin, direction, sphere, t) && t < closestT)
        {
            closestT = t;
            hitSphere = &sphere;
        }
    }

    if (!hitSphere)
    {
        // Return background color
        return ofColor(135, 206, 250); // Sky blue
    }

    // Compute intersection point and normal
    glm::vec3 hitPoint = origin + closestT * direction;
    glm::vec3 normal = glm::normalize(hitPoint - hitSphere->center);
    glm::vec3 viewDir = -direction;

    glm::vec3 color = computeLighting(hitPoint, normal, viewDir, *hitSphere);
    return ofColor(color.x * 255, color.y * 255, color.z * 255);
}

bool RaytracingObject::intersectSphere(const glm::vec3& origin, const glm::vec3& direction, const Sphere& sphere, float& t)
{
    glm::vec3 oc = origin - sphere.center;
    float a = glm::dot(direction, direction);
    float b = 2.0f * glm::dot(oc, direction);
    float c = glm::dot(oc, oc) - sphere.radius * sphere.radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) return false;

    float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
    float t2 = (-b + sqrt(discriminant)) / (2.0f * a);

    t = (t1 < t2) ? t1 : t2;
    return t >= 0;
}

glm::vec3 RaytracingObject::computeLighting(const glm::vec3& point, const glm::vec3& normal, const glm::vec3& viewDir, const Sphere& sphere)
{
    // Get material properties
    const ofFloatColor diffuseColor = sphere.material.getDiffuseColor();
    const ofFloatColor specularColor = sphere.material.getSpecularColor();
    float shininess = sphere.material.getShininess();

    glm::vec3 ambient = sphere.ambient * glm::vec3(sphere.color.r, sphere.color.g, sphere.color.b) / 255.0f;

    glm::vec3 result = ambient;

    for (const auto& light : lights)
    {
        glm::vec3 lightDir = glm::normalize(light.position - point);

        // Diffuse lighting
        float diff = std::max(glm::dot(normal, lightDir), 0.0f);
        glm::vec3 diffuse = diff * sphere.diffuse * glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);

        // Specular lighting
        glm::vec3 reflectDir = glm::reflect(-lightDir, normal);
        float spec = pow(std::max(glm::dot(viewDir, reflectDir), 0.0f), shininess);
        glm::vec3 specular = spec * sphere.specular * glm::vec3(specularColor.r, specularColor.g, specularColor.b);

        result += diffuse + specular;
    }

    return result;
}

