#include "ofApp.h"

// cleanup
ofApp::~ofApp()
{
    ofRemoveListener(ofEvents().mouseMoved, this, &ofApp::mouseMoved);
}

//--------------------------------------------------------------
void ofApp::update() 
{
	updateWaterMesh(bucketWaterMesh, glm::vec2(400, 400));
    
	// Threshold values for the camera's Y position
	float minY = 10.0f;   
	
	if (!path.empty())
	{
		for (const auto& point : path) 
		    std::cout << "{" << point.x << ", " << point.y << ", " << point.z << "}, "; // Print each point
		std::cout << std::endl;
    }
    
    if(followPathBlindly) 
    {
    	followPath(path);
    	if (path.empty()) followPathBlindly = false;
    	return;
    }
    
	// Camera movement based on key presses (WASD)
	if (keys['w']) cam.dolly(-moveSpeed); // Move forward
	if (keys['s']) cam.dolly(moveSpeed); // Move backward
	if (keys['a']) cam.truck(-moveSpeed); // Move left
	if (keys['d']) cam.truck(moveSpeed); // Move right
	if (keys[OF_KEY_SHIFT]) cam.boom(-moveSpeed); // Move down (Shift key)
	if (keys[' ']) cam.move(0, moveSpeed, 0); // Move up (Space key)
	
	/*
	if (cam.getPosition().y < minY) // floor
	    cam.move(0, minY - cam.getPosition().y, 0);
	
    if(cameraFloor < cam.getPosition().y) // grav
	    cam.move(0, -10, 0);
    */
}

void ofApp::updateWaterMesh(ofMesh& waterMesh, glm::vec2 size)
{
    noiseZ += 0.03f;
    for (int y = 0; y < size.y; y++) 
    {
        for (int x = 0; x < size.x; x++) 
        {
            // Calculate the index for the current vertex
            std::size_t index = x + y * size.x;

            // Generate the wave height
            float waveHeight = generateWaveBallHeight(x + ofGetElapsedTimef() * 20, y + ofGetElapsedTimef() * 20);
            float z = waveHeight + ofNoise(x * noiseScale, y * noiseScale, noiseZ) * 90.0f;

            // Get the current vertex, apply rotation, translation, and update the z value
            ofPoint vertex = waterMesh.getVertex(index);
            //vertex = rotationMatrix * vertex; // Apply rotation
            //vertex += translation;             // Apply translation
            vertex.z = z;                     // Update z with wave height

            // Set the updated vertex back to the mesh
            waterMesh.setVertex(index, vertex);
        }
    }
}

//--------------------------------------------------------------
//-----------------------SETUP-FUNCTIIONS-----------------------
//--------------------------------------------------------------

void ofApp::setup() 
{
    cameraFloor = 0;
    cameraSpeed = 5.0f;
    currentTargetIndex = 0;
    noiseScale = 0.1f;
    
    // Light setup
    sun.setPointLight();
    sun.setAmbientColor(ofColor::darkSlateGrey);
    sun.setDiffuseColor(0.4);
    ofEnableSeparateSpecularLight();

    sun.setSpecularColor(0.8);
    sun.setSpotConcentration(30);
    sun.setGlobalPosition(330, 3830, 700);
    sun.enable();
    
	waterMaterial.setShininess(10);
    waterMaterial.setSpecularColor(ofColor(255, 255, 255, 255));
    waterMaterial.setEmissiveColor(ofColor(0, 0, 0, 255));
    waterMaterial.setDiffuseColor(ofColor(255, 255, 255, 255));
    waterMaterial.setAmbientColor(ofColor(255, 255, 255, 255));
    
    material.setShininess(10);
    material.setSpecularColor(ofColor(255, 255, 255, 255));
    material.setEmissiveColor(ofColor(0, 0, 0, 255));
    material.setDiffuseColor(ofColor(255, 255, 255, 255));
    material.setAmbientColor(ofColor(255, 255, 255, 255));
    
    // Set up wood material
    woodMaterial.setDiffuseColor(ofColor(139, 69, 19)); // Brown color for wood
    woodMaterial.setSpecularColor(ofColor(255, 255, 255)); // White specular highlights
    woodMaterial.setShininess(64); // Set shininess for specular highlights

    // Set up fire material
    fireMaterial.setDiffuseColor(ofColor(255, 165, 0)); // Orange color for fire
    fireMaterial.setSpecularColor(ofColor(255, 255, 0)); // Yellow specular highlights
    fireMaterial.setShininess(32); // Set shininess for fire
    
    // Setup the sand material
    sandMaterial.setDiffuseColor(ofColor(194, 178, 128));  // Sand-like color (light brown/beige)
    sandMaterial.setAmbientColor(ofColor(190, 170, 120));  // Slightly darker ambient color
    sandMaterial.setSpecularColor(ofColor(200, 200, 200)); // Specular highlights
    sandMaterial.setShininess(10);  // Low shininess for a matte, rough look
    
    // setup mesh
    setupWaterMesh(bucketWaterMesh, glm::vec2(400,400), 130, 3.5f);
    setupSandIslandMesh(sandIslandMesh, 700, 64, 10, 3.5);
    
	// cam
    cam.move(0, 400, 0);
    cam.setFov(60);
    cam.setNearClip(1.0f);  // Minimum distance from the camera to render objects (near clipping plane)
	cam.setFarClip(10000.0f);
    moveSpeed = 30.0f;
    memset(keys, 0, sizeof(keys));


    // Setup terrain object variables
    ofEnableLighting();
    ofEnableDepthTest();
    
	// listeners
    ofAddListener(ofEvents().mouseMoved, this, &ofApp::mouseMoved);
}

void ofApp::setupSandIslandMesh(ofMesh& sandMesh, float radius, int resolution, float spread, float noiseScale)
{
    // Create a sphere mesh
    sandMesh = ofMesh::sphere(radius, resolution);
    
    // Loop through all vertices and apply noise to z-coordinate (height)
    for (std::size_t i = 0; i < sandMesh.getNumVertices(); i++) 
    {
        glm::vec3 vertex = sandMesh.getVertex(i);
        
        // Apply Perlin noise to perturb the vertex position
        float noiseValue = ofNoise(vertex.x * noiseScale, vertex.y * noiseScale, vertex.z * noiseScale);
        vertex.x += noiseValue * spread;  // You can scale this value to control roughness
        vertex.y += noiseValue * spread * 10 - 500;
        vertex.z += noiseValue * spread;
        std::cout << vertex << std::endl;
        
        sandMesh.setVertex(i, vertex);
    }
}


void ofApp::setupWaterMesh(ofMesh& waterMesh, glm::vec2 waterDimensions, float spread, float noiseScale)
{
    // Create the terrain mesh
    waterMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    for (int y = 0; y < waterDimensions.y; y++) 
	{
        for (int x = 0; x < waterDimensions.x; x++) 
		{
            float z = ofNoise(x * noiseScale, y * noiseScale, noiseZ) * 400.0f;
            waterMesh.addVertex(glm::vec3(x * spread, y * spread, z));
            waterMesh.addNormal(glm::vec3(0, 0, 1));
            waterMesh.addColor(ofFloatColor(0.2, 0.8, 0.2 + z*0.4));
        }
    }

    for (int y = 0; y < waterDimensions.y - 1; y++) 
	{
        for (int x = 0; x < waterDimensions.x - 1; x++) 
		{
            int i = x + y * waterDimensions.x;
            waterMesh.addIndex(i);
            waterMesh.addIndex(i + 1);
            waterMesh.addIndex(i + waterDimensions.x);

            waterMesh.addIndex(i + 1);
            waterMesh.addIndex(i + 1 + waterDimensions.x);
            waterMesh.addIndex(i + waterDimensions.x);
        }
    }
}


//--------------------------------------------------------------
//----------------------HELPER-FUNCTIIONS-----------------------
//--------------------------------------------------------------


// Generate random vector to somewhere on the surface of the eye
glm::vec3 ofApp::generateRandomVector(float totalDistance) 
{
    float x = ofRandom(-totalDistance, totalDistance);
    float y = ofRandom(-totalDistance, totalDistance);

    // Calculate the remaining z value to ensure the vector's length is totalDistance
    float z = std::sqrt(totalDistance * totalDistance - x * x - y * y);

    // Randomly choose the sign of z to ensure the vector can point in all directions
    z *= (ofRandom(0.0f, 1.0f) > 0.5f) ? 1.0f : -1.0f;

    return glm::vec3(x, y, z);
}

// sinosodal function generates height at a given position 
float ofApp::generateWaveBallHeight(float x, float y)
{
	float wx = sin(x * 0.1f) * 50.0f;
	float wy = sin(y * 0.1f) * 50.0f;
	return wx + wy;
}

void ofApp::followPath(std::vector<glm::vec3>& pathPoints) 
{
    float minY = 10.0f;   
    if (pathPoints.empty()) return;

    // Check the current target point
    glm::vec3 currentTarget = pathPoints.front(); // Get the first point
    float distanceToTarget = glm::distance(glm::vec2(cam.getPosition().x, cam.getPosition().z) , glm::vec2(currentTarget.x, currentTarget.z));

    // Move towards the current target point
    if (distanceToTarget > 26.0f) 
    {
        glm::vec3 direction = glm::normalize(currentTarget - cam.getPosition());
        cam.move(direction * cameraSpeed); 
    }
    else
        pathPoints.erase(pathPoints.begin()); // Remove the first point from the vector

    if (cam.getPosition().y < minY) 
        cam.move(0, minY - cam.getPosition().y, 0);
}

//--------------------------------------------------------------
//------------------------DRAW-FUNCTIIONS-----------------------
//--------------------------------------------------------------


//--------------------------------------------------------------
void ofApp::draw() 
{
    //glDisable(GL_COLOR_MATERIAL);

    cam.begin();
    
    drawIslandMesh(sandIslandMesh);
    drawWaterMesh(bucketWaterMesh);
    cam.end();
}

void ofApp::drawIslandMesh(ofMesh& islandMesh)
{	// this should be members in a class
    ofPushMatrix();
    glm::vec3 scale(4, 1, 4);
    ofScale(scale);
    ofDrawSphere(0, -70, 0, 400); // center platform
    ofPopMatrix();
    
    ofPushMatrix();
    scale = glm::vec3(10, 1, 10);
    ofScale(scale);
    sandMaterial.begin();
    sandIslandMesh.draw();
    sandMaterial.end();
    ofPopMatrix();
}

void ofApp::drawWaterMesh(ofMesh& waterMesh)
{	// this should be members in a class
    glm::vec3 translation(-400.01f * 0.5f * 130 , -400.01f * 0.5f * 130, 10.0f);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	ofMatrix4x4 rotationMatrix;
	rotationMatrix.rotate(90, 1, 0, 0);  
	
    waterMaterial.begin();
    ofPushMatrix();
	ofMultMatrix(rotationMatrix);
	ofTranslate(translation);
	ofScale(scale);
    bucketWaterMesh.draw();
    ofPopMatrix();
    waterMaterial.end();
}


//--------------------------------------------------------------
//-----------------------INPUT-FUNCTIONS------------------------
//--------------------------------------------------------------
void ofApp::keyPressed(int key) 
{
    keys[key] = true;
    if (key == 'q') path.push_back(cam.getPosition());
    if (key == 'e') followPathBlindly = !followPathBlindly;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) 
{
    keys[key] = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(ofMouseEventArgs& mouse) 
{
    // Reset to a default forward-facing orientation
    cam.setGlobalOrientation(glm::quat());

    // Calculate the relative mouse position to the screen center
    float deltaX = mouse.x - ofGetWidth() / 2;
    float deltaY = mouse.y - ofGetHeight() / 2;

    // Rotate the camera based on mouse movement
    float sensitivity = .35f;  // You can tweak the sensitivity as needed
    cam.panDeg(-sensitivity*1.4 * deltaX);   // Pan (left-right rotation)
    cam.tiltDeg(-sensitivity * deltaY);   // Tilt (up-down rotation)
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) 
{
    // Toggle cursor visibility
    if (isCursorHidden) 
        ofShowCursor();
	else
        ofHideCursor();
    
    isCursorHidden = !isCursorHidden; // Update the state
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}