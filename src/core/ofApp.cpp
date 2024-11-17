#include "ofApp.h"
#include "IslandScene.h"
#include "PopperScene.h"

// cleanup
ofApp::~ofApp()
{
    ofRemoveListener(ofEvents().mouseMoved, this, &ofApp::mouseMoved);
}
//--------------------------------------------------------------
//-----------------------SETUP-FUNCTIIONS-----------------------
//--------------------------------------------------------------

void ofApp::setup() 
{
    // listeners
    ofAddListener(ofEvents().mouseMoved, this, &ofApp::mouseMoved);
    
    cameraFloor = 0;
    cameraSpeed = 5.0f;
    currentTargetIndex = 0;
    
    // Light setup
    sun.setPointLight();
    sun.setAmbientColor(ofColor::darkSlateGrey);
    sun.setDiffuseColor(0.4);
    ofEnableSeparateSpecularLight();

    sun.setSpecularColor(0.8);
    sun.setSpotConcentration(30);
    sun.setGlobalPosition(0, 1330, 0);
    sun.enable();
    
    // add scenes / scene setups
    sceneManager.registerInputManager(&inputManager);
    sceneManager.addScene(new PopperScene());
    sceneManager.addScene(new IslandScene());
    sceneManager.setup();
    
    /// map these keys so that shift works properly
    inputManager.map('W', 'w');
    inputManager.map('S', 's');
    inputManager.map('A', 'a');
    inputManager.map('D', 'd');
    
    // cam
    cam.move(0, 400, 0);
    cam.setFov(60);
    cam.setNearClip(1.0f);  // Minimum distance from the camera to render objects (near clipping plane)
    cam.setFarClip(31000.0f);
    moveSpeed = 60.0f;

    // Setup terrain object variables
    ofEnableLighting();
    ofEnableDepthTest();
}

//--------------------------------------------------------------
//----------------------HELPER-FUNCTIIONS-----------------------
//--------------------------------------------------------------

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
//-----------------------UPDATE-CALLBACK------------------------
//--------------------------------------------------------------

void ofApp::update() 
{
    sceneManager.update();
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
    //keys[key] = true;
    // Camera movement based on key presses (WASD)
    if (inputManager.getPressed(OF_KEY_SHIFT)) cam.boom(-moveSpeed); 
    if (inputManager.getPressed('1')) sceneManager.loadScene(0); // Move down (Shift key)
    else if (inputManager.getPressed('2')) {
        std::cout << "LOADING" << std::endl;
        sceneManager.loadScene(1);
        std::cout << "LOADING DOne" << std::endl;
    }
    else if (inputManager.getPressed('t')) sceneManager.toggleStaticMesh();
    else if (inputManager.getPressed('q')) path.push_back(cam.getPosition());
    else if (inputManager.getPressed('e')) followPathBlindly = !followPathBlindly;
    else if (inputManager.getPressed(' ')) cam.move(0, moveSpeed, 0); // Move up (Space key)
    
    if (inputManager.getHeld('w')) cam.dolly(-moveSpeed); // Move forward
    if (inputManager.getHeld('s')) cam.dolly(moveSpeed); // Move backward
    if (inputManager.getHeld('a')) cam.truck(-moveSpeed); // Move left
    if (inputManager.getHeld('d')) cam.truck(moveSpeed); // Move right'
}

//--------------------------------------------------------------
//------------------------DRAW-CALLBACKS------------------------
//--------------------------------------------------------------

void ofApp::draw() 
{
    cam.begin();
    sceneManager.draw();
    cam.end();
}

//--------------------------------------------------------------
//-----------------------INPUT-CALLBACKS------------------------
//--------------------------------------------------------------
void ofApp::keyPressed(int key) { inputManager.ofKeyPressed(key); } // use inputManager.get(key) to check if a key is pressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key) { inputManager.ofKeyReleased(key); }

//--------------------------------------------------------------
void ofApp::mouseMoved(ofMouseEventArgs& mouse) 
{
    // Reset to a default forward-facing orientation
    cam.setGlobalOrientation(glm::quat());

    float deltaX = mouse.x - ofGetWidth() / 2;
    float deltaY = mouse.y - ofGetHeight() / 2;

    // Rotate the camera based on mouse movement
    float sensitivity = .35f; 
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
