#include "JolfBaseScene.h"

JolfOceanObject::JolfOceanObject(glm::vec3 pos, float nScale, float sp)
    : PhysicsEntity(pos), noiseScale(nScale), spread(sp)
{
    noiseZ = 0.0f;
    addTag("ocean");
}

JolfOceanObject::~JolfOceanObject() {}


Entity* JolfOceanObject::clone() const
{
    return new JolfOceanObject(position, noiseScale, spread);
}

void JolfOceanObject::updateNormals()
{
    // Clear previous normals
    mesh.clearNormals();

    // Define a light direction that changes gradually over time (simulate sun movement)
    // lightDir = glm::normalize(glm::vec3(sin(ofGetElapsedTimef() * 0.2f) * 0.5f, -0.8f, cos(ofGetElapsedTimef() * 0.2f) * 0.5f));
    lightDir = glm::vec3(0.0f, -0.35f, -0.5f);
    glm::vec3 viewDir = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)); // Adjust as needed for camera direction

    // Calculate normals based on adjacent vertices
    for (int y = 0; y < dimensions.z; y++)
    {
        for (int x = 0; x < dimensions.x; x++)
        {
            int i = x + y * dimensions.x;

            // Get neighboring vertices with boundary checks
            glm::vec3 left   = x > 0 ? vertices[i - 1] : vertices[i];
            glm::vec3 right  = x < dimensions.x - 1 ? vertices[i + 1] : vertices[i];
            glm::vec3 up     = y > 0 ? vertices[i - dimensions.x] : vertices[i];
            glm::vec3 down   = y < dimensions.z - 1 ? vertices[i + dimensions.x] : vertices[i];

            // Calculate vectors from neighboring vertices
            glm::vec3 dx = right - left;
            glm::vec3 dy = down - up;

            // Compute normal with cross product of dx and dy
            glm::vec3 normal = glm::normalize(glm::cross(dx, dy));

            // Calculate angle-based shadow intensity
            float angleFactor = glm::dot(normal, lightDir);
            float shadowIntensity = glm::clamp(0.3f + angleFactor * 0.8f, 0.0f, 1.0f); // Fine-tuned range
            glm::vec3 shadedNormal = normal * shadowIntensity;

            // Fresnel effect for edge reflectivity
            float fresnelPower = 3.0f;  // Controls sharpness of reflectivity
            float fresnelEffect = pow(1.0f - glm::dot(viewDir, normal), fresnelPower);
            shadedNormal = glm::mix(shadedNormal, lightDir, fresnelEffect * 0.6f);

            // Add the shaded normal to the mesh
            mesh.addNormal(glm::normalize(shadedNormal));
        }
    }
}

void JolfOceanObject::_collision(PhysicsEntity& target)
{
    // if(!target.hasTag("ball")) // use tags to determine behaviour when needed
}

void JolfOceanObject::_setup()
{
    material.setShininess(32);  // Higher shininess for sharper specular highlights
    material.setSpecularColor(ofColor(255, 255, 255, 180)); // Subtle white highlight
    material.setEmissiveColor(ofColor(0, 0, 0, 255));
    material.setDiffuseColor(ofColor(50, 100, 200, 145)); // Darker diffuse color
    material.setAmbientColor(ofColor(90, 90, 160, 255));

    // properties
    //translation = glm::vec3(-(dimensions.x * 0.5) * spread, -(dimensions.y * 0.5) * spread, dimensions.z);
    translation = glm::vec3(-(dimensions.x * 0.5) * spread, dimensions.y, -(dimensions.z * 0.5) * spread);
    scale = glm::vec3(1, 1, 1); // -1 for rotation in z axis
    rotation.makeRotate(90, 1, 0, 0);

    // Create the terrain mesh
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    vertices.clear();
    vertices.reserve(dimensions.x * dimensions.z);

    for (int y = 0; y < dimensions.z; y++)
    {
        for (int x = 0; x < dimensions.x; x++)
        {
            float z = ofNoise(x * noiseScale, y * noiseScale, noiseZ) * 170.0f;
            glm::vec3 vertex = glm::vec3(x * spread, y * spread, z);
            vertices.push_back(vertex);
            mesh.addVertex(vertex);
            mesh.addColor(ofFloatColor(0.2, 0.8, 0.2 + z * 0.4));
        }
    }

    // Set up indices for the triangles
    for (int y = 0; y < dimensions.z - 1; y++)
    {
        for (int x = 0; x < dimensions.x - 1; x++)
        {
            int i = x + y * dimensions.x;
            mesh.addIndex(i);
            mesh.addIndex(i + 1);
            mesh.addIndex(i + dimensions.x);

            mesh.addIndex(i + 1);
            mesh.addIndex(i + 1 + dimensions.x);
            mesh.addIndex(i + dimensions.x);
        }
    }
    // Calculate normals initially
    updateNormals();
    _update();
}

void JolfOceanObject::_update()
{
    noiseZ += 0.03f;
    for (int y = 0; y < dimensions.z; y++)
    {
        for (int x = 0; x < dimensions.x; x++)
        {
            // Calculate the index for the current vertex
            std::size_t index = x + y * dimensions.x;

            // Generate the wave height & update z with wave height
            int floatHeightOffset = 30;
            float waveHeight = generateWaveHeight(x + ofGetElapsedTimef() * floatHeightOffset, y + ofGetElapsedTimef() * floatHeightOffset);
            float z = waveHeight + ofNoise(x * noiseScale, y * noiseScale, noiseZ) * 130.0f;

            vertices[index].z = z;
            mesh.setVertex(index, vertices[index]);
        }
    }
    updateNormals();

}

void JolfOceanObject::_draw()
{
    // draw water
    material.begin();
    mesh.draw();
    material.end();
}

// Sinosoidal function generates height at a given position for uniform-ish waves
float JolfOceanObject::generateWaveHeight(float x, float y)
{
    float wx = sin(x * 0.1f) * 40.0f + cos(x * 0.09f + y * 0.03f) * 45.0f;
    float wy = sin(y * 0.1f) * 35.0f + cos(y * 0.09f + x * 0.03f) * 40.0f;
    return wx + wy;
}
