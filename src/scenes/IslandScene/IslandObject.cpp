#include "IslandScene.h"

IslandObject::IslandObject(glm::vec3 dim, float rad, int res, float sp, float ns)
	: Entity(dim), radius(rad), resolution(res), spread(sp), noiseScale(ns) {}

IslandObject::~IslandObject() {}

void IslandObject::_setup()
{
    // Set up wood material
    stoneMaterial.setDiffuseColor(ofColor(139, 139, 139)); // Brown color for wood
    stoneMaterial.setSpecularColor(ofColor(255, 255, 255)); // White specular highlights
    stoneMaterial.setShininess(64); // Set shininess for specular highlights

    // Setup the sand material
    material.setDiffuseColor(ofColor(194, 178, 128));  // Sand-like color (light brown/beige)
    material.setAmbientColor(ofColor(190, 170, 120));  // Slightly darker ambient color
    material.setSpecularColor(ofColor(200, 200, 200)); // Specular highlights
    material.setShininess(10);  // Low shininess for a matte, rough look
    
    stone.setRadius(400);
    stone.setResolution(64);
    
    mesh = ofMesh::sphere(radius, resolution);
    
    // Loop through all vertices and apply noise to z-coordinate (height)
    for (std::size_t i = 0; i < mesh.getNumVertices(); i++) 
    {
        glm::vec3 vertex = mesh.getVertex(i);
        
        // Apply Perlin noise to perturb the vertex position
        float noiseValue = ofNoise(vertex.x * noiseScale, vertex.y * noiseScale, vertex.z * noiseScale);
        vertex.x += noiseValue * spread;  // You can scale this value to control roughness
        vertex.y += noiseValue * spread * 10 - 500;
        vertex.z += noiseValue * spread;
        
        mesh.setVertex(i, vertex);
    }
}

void IslandObject::_update()
{

}

void IslandObject::_draw()
{
    ofPushMatrix();
	
    ofPushMatrix();
    ofScale(glm::vec3(4, 1, 4));
    ofTranslate(glm::vec3(0, -70, 0));
    stoneMaterial.begin();
    //ofDrawSphere(0, -70, 0, 400); // center platform
    stone.draw();
    stoneMaterial.end();
    ofPopMatrix();
    
    ofPushMatrix();
    ofScale(glm::vec3(10, 1, 10));
    material.begin();
    mesh.draw();
    material.end();
    ofPopMatrix();
    
    
    ofPopMatrix();
}
