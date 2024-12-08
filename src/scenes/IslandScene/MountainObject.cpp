#include "IslandScene.h"

MountainObject::MountainObject(glm::vec3 pos, float rad, int res, float sp, float ns)
    : Entity(pos), radius(rad), resolution(res), spread(sp), noiseScale(ns) {
    addTag("mountain");
}

MountainObject::~MountainObject() {}

Entity* MountainObject::clone() const
{
    return new MountainObject(position, radius, resolution, spread, noiseScale);
}
void MountainObject::_setup()
{
    material.setDiffuseColor(ofColor(105, 139, 34));  // Greenish rocky color
    material.setAmbientColor(ofColor(34, 55, 23));    // Shadow tones
    material.setSpecularColor(ofColor(70, 70, 70));   // Subtle highlights
    material.setShininess(5);                         // Rougher texture
    scale = glm::vec3(20, 2, 20);
    mesh = ofMesh::cone(20, 100, 100);

    for (std::size_t i = 0; i < mesh.getNumVertices(); i++)
    {
        glm::vec3 vertex = mesh.getVertex(i);

        float noiseValue = ofNoise(vertex.x * noiseScale, vertex.y * noiseScale, vertex.z * noiseScale);

        // Compute a smoother height emphasis factor
        float baseHeight = 100.0f; // Max height of the cone
        float distanceFromBase = (vertex.y + baseHeight) / (2 * baseHeight); // Normalize to [0, 1]
        float heightFactor = std::sin(distanceFromBase * glm::pi<float>()); // Peak near the middle

        // Smooth out the entire noise application
        float smoothFactor = glm::smoothstep(0.0f, 1.0f, distanceFromBase); // Gradual smoothing across height
        noiseValue *= heightFactor * smoothFactor;

        // Reduce overall deformation
        vertex.x += noiseValue * spread * 0.5f;  // Halved for smoother x deformation
        vertex.y += noiseValue * spread * 1000;   // Reduced height scaling for smoother peaks
        vertex.z += noiseValue * spread * 0.5f;  // Halved for smoother z deformation

        mesh.setVertex(i, vertex);
    }

}

void MountainObject::_update()
{
}

void MountainObject::_draw()
{
    ofPushMatrix();
    ofTranslate(glm::vec3(0, 0, 1000));
    ofScale(glm::vec3(50.0f, 1, 30.0f));
    material.begin();
    mesh.draw();
    material.end();
    ofPopMatrix();
}
