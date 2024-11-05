#include "IslandScene.h"

OceanObject::OceanObject(glm::vec3 dim, float nScale, float sp)
	: Entity(dim), noiseScale(nScale), spread(sp) 
{
	noiseZ = 0.0f;
}

OceanObject::~OceanObject() {}

void OceanObject::updateNormals()
{
    // Clear previous normals
    mesh.clearNormals();
    
    // Define a light direction for calculating shading effect
    glm::vec3 lightDir = glm::normalize(glm::vec3(0.5f, 1.5f, 0.5f)); // Adjust direction as needed
    
    // Calculate normals based on adjacent vertices
    for (int y = 0; y < dimensions.y; y++) 
    {
        for (int x = 0; x < dimensions.x; x++) 
        {
            int i = x + y * dimensions.x;

            // Get neighboring vertices with boundary checks
            glm::vec3 left   = x > 0 ? vertices[i - 1] : vertices[i];
            glm::vec3 right  = x < dimensions.x - 1 ? vertices[i + 1] : vertices[i];
            glm::vec3 up     = y > 0 ? vertices[i - dimensions.x] : vertices[i];
            glm::vec3 down   = y < dimensions.y - 1 ? vertices[i + dimensions.x] : vertices[i];

            // Calculate vectors from neighboring vertices
            glm::vec3 dx = right - left;
            glm::vec3 dy = down - up;
            
            // Compute normal with cross product of dx and dy
            glm::vec3 normal = glm::normalize(glm::cross(dx, dy));
            
            // Calculate angle-based shadow intensity
            float angleFactor = glm::dot(normal, lightDir);
            float shadowIntensity = glm::clamp(0.0f, angleFactor * 0.5f + 0.5f, 0.0f); // Adjust range for effect
            glm::vec3 shadedNormal = normal * shadowIntensity;

			// Adjust normals to make the shadows darker based on angle to light
			float shadowBoost = 2.5f;  // Increase this factor for darker shadows
			shadedNormal *= shadowBoost;

            mesh.addNormal(glm::normalize(shadedNormal));
        }
    }
}


void OceanObject::_setup() 
{
    material.setShininess(1);
    material.setSpecularColor(ofColor(220, 220, 220, 255));
    material.setEmissiveColor(ofColor(0, 0, 0, 255));
    material.setDiffuseColor(ofColor(100, 100, 200, 145));
    material.setAmbientColor(ofColor(90, 90, 160, 255));

    // Create the terrain mesh
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    vertices.clear();
    vertices.reserve(dimensions.x * dimensions.y);
    
    for (int y = 0; y < dimensions.y; y++) 
    {
        for (int x = 0; x < dimensions.x; x++) 
        {
            float z = ofNoise(x * noiseScale, y * noiseScale, noiseZ) * 400.0f;
            glm::vec3 vertex = glm::vec3(x * spread, y * spread, z);
            vertices.push_back(vertex);
            mesh.addVertex(vertex);
            mesh.addColor(ofFloatColor(0.2, 0.8, 0.2 + z * 0.4));
        }
    }

    // Set up indices for the triangles
    for (int y = 0; y < dimensions.y - 1; y++) 
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
}

void OceanObject::_update() 
{
    noiseZ += 0.03f;
    for (int y = 0; y < dimensions.y; y++) 
    {
        for (int x = 0; x < dimensions.x; x++) 
        {
            // Calculate the index for the current vertex
            std::size_t index = x + y * dimensions.x;

            // Generate the wave height & update z with wave height
            int floatHeightOffset = 30;
            float waveHeight = generateWaveHeight(x + ofGetElapsedTimef() * floatHeightOffset, y + ofGetElapsedTimef() * floatHeightOffset);
            float z = waveHeight + ofNoise(x * noiseScale, y * noiseScale, noiseZ) * 70.0f;

            vertices[index].z = z;
            mesh.setVertex(index, vertices[index]);
        }
    }
    
    updateNormals();
}

void OceanObject::_draw() 
{
    glm::vec3 translation(-(dimensions.x * 0.5) * spread , -(dimensions.y * 0.5) * spread, dimensions.z);
	glm::vec3 scale = glm::vec3(1, 1, 1);
    ofPushMatrix(); // save state
    
    ofQuaternion waterRotation = ofQuaternion(0, 0, 0, 1);
	waterRotation.makeRotate(90, 1, 0, 0);
	
    ofMatrix4x4 rotationMatrix;
    waterRotation.get(rotationMatrix);
    
	// SRT inverse
	ofScale(scale);
	ofMultMatrix(rotationMatrix); // with this one
	ofTranslate(translation); // maybe switch this if the water mesh is invisible
	
	// draw water
    material.begin();
    mesh.draw();
    material.end();
    
    ofPopMatrix(); // go back to state
}


// sinosodal function generates height at a given position for uniform-ish waves
float OceanObject::generateWaveHeight(float x, float y)
{
	float wx = sin(x * 0.1f) * 40.0f + cos(x*0.09f + y*0.03f) * 40.0f;
	float wy = sin(y * 0.1f) * 40.0f + cos(y*0.09f + x*0.03f) * 40.0f;
	return wx + wy;
}
