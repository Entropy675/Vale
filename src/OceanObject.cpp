#include "Objects.h"

OceanObject::OceanObject(glm::vec3 dim, float nScale, float sp)
	: Entity(dim), noiseScale(nScale), spread(sp) {}

OceanObject::~OceanObject() {}


void OceanObject::setup() 
{
	material.setShininess(10);
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    material.setEmissiveColor(ofColor(0, 0, 0, 255));
    material.setDiffuseColor(ofColor(255, 255, 255, 255));
    material.setAmbientColor(ofColor(255, 255, 255, 255));
    
    // Create the terrain mesh
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
    for (int y = 0; y < dimensions.y; y++) 
	{
        for (int x = 0; x < dimensions.x; x++) 
		{
            float z = ofNoise(x * noiseScale, y * noiseScale, noiseZ) * 400.0f;
            mesh.addVertex(glm::vec3(x * spread, y * spread, z));
            mesh.addNormal(glm::vec3(0, 0, 1));
            mesh.addColor(ofFloatColor(0.2, 0.8, 0.2 + z*0.4));
        }
    }

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
}

void OceanObject::update() 
{
    noiseZ += 0.03f;
    for (int y = 0; y < dimensions.y; y++) 
    {
        for (int x = 0; x < dimensions.x; x++) 
        {
            // Calculate the index for the current vertex
            std::size_t index = x + y * dimensions.x;

            // Generate the wave height & update z with wave height
            float waveHeight = generateWaveHeight(x + ofGetElapsedTimef() * 20, y + ofGetElapsedTimef() * 20);
            float z = waveHeight + ofNoise(x * noiseScale, y * noiseScale, noiseZ) * 70.0f;

            ofPoint vertex = mesh.getVertex(index);
            vertex.z = z;

            mesh.setVertex(index, vertex);
        }
    }
}

void OceanObject::draw() 
{
    glm::vec3 translation(-(dimensions.x * 0.5) * spread , -(dimensions.y * 0.5) * spread, dimensions.z);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	
    ofPushMatrix(); // save state
    
    ofMatrix4x4 rotationMatrix;
    rotation.get(rotationMatrix);
    
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
	float wx = sin(x * 0.1f) * 50.0f;
	float wy = sin(y * 0.1f) * 50.0f;
	return wx + wy;
}
