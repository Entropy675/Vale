#include "ofApp.h"
#include "IslandScene.h"

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
    sceneManager.addScene(new IslandScene());
    sceneManager.setup();
    
    // cam
    cam.move(0, 400, 0);
    cam.setFov(60);
    cam.setNearClip(1.0f);  // Minimum distance from the camera to render objects (near clipping plane)
    cam.setFarClip(21000.0f);
    moveSpeed = 60.0f;
    memset(keys, 0, sizeof(keys));


    // Setup terrain object variables
    ofEnableLighting();
    ofEnableDepthTest();
    
    // listeners
    ofAddListener(ofEvents().mouseMoved, this, &ofApp::mouseMoved);
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
    
    // Camera movement based on key presses (WASD)
    if (keys['w']) cam.dolly(-moveSpeed); // Move forward
    if (keys['s']) cam.dolly(moveSpeed); // Move backward
    if (keys['a']) cam.truck(-moveSpeed); // Move left
    if (keys['d']) cam.truck(moveSpeed); // Move right
    if (keys[OF_KEY_SHIFT]) cam.boom(-moveSpeed); // Move down (Shift key)
    if (keys[' ']) cam.move(0, moveSpeed, 0); // Move up (Space key)
    
    /*
    // Gravity
    float minY = 10.0f; // Threshold values for the camera's Y position
    if (cam.getPosition().y < minY) // floor
        cam.move(0, minY - cam.getPosition().y, 0);
    
    if(cameraFloor < cam.getPosition().y) // grav
        cam.move(0, -10, 0);
    */
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