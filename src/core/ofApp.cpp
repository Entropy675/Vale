#include "ofApp.h"
#include "IslandScene.h"
#include "JolfBaseScene.h"
#include "PopperScene.h"
#include "RaytracingScene.h"

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


    // Custom tags (you can add more whenever you want, TagManager::addTag(tag, context))
    std::unordered_map<std::string, std::string> customTags = {
        {"player", "Tag for player entities (should be attached to player)"},
        {"enemy", "Tag for enemy entities"}
    };
    std::unordered_map<std::string, PhysicsMetadata> customPhysicsTags = {
        {"player_physics", PhysicsMetadata("player_physics", nullptr, nullptr, nullptr)},
        {"enemy_physics", PhysicsMetadata("enemy_physics", nullptr, nullptr, nullptr)}
    }; // PhysicsMetadata("physics_tag", bounding equation, normal equation, extra context void*)


    // Initialize TagManager (you can call this as many times as you want to add more tags)
    TagManager::initialize(customTags, customPhysicsTags);

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
    sceneManager.addScene(new JolfBaseScene());
    sceneManager.setup();

    /// map these keys so that shift works properly
    inputManager.map('W', 'w');
    inputManager.map('S', 's');
    inputManager.map('A', 'a');
    inputManager.map('D', 'd');

    // Setup terrain object variables
    ofEnableLighting();
    ofEnableDepthTest();
}

//--------------------------------------------------------------
//----------------------HELPER-FUNCTIIONS-----------------------
//--------------------------------------------------------------

/*
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
*/



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
        //followPath(path);
        if (path.empty()) followPathBlindly = false;
        return;
    }
    //keys[key] = true;
    // Camera movement based on key presses (WASD)
    if (inputManager.getPressedOnce('1')) sceneManager.prev();
    else if (inputManager.getPressedOnce('2')) sceneManager.next();

    if (inputManager.getPressedOnce('t')) sceneManager.toggleStaticMesh();
  //  if (inputManager.getPressedOnce('q')) path.push_back(cam.getPosition());
    if (inputManager.getPressedOnce('e')) followPathBlindly = !followPathBlindly;
}

//--------------------------------------------------------------
//------------------------DRAW-CALLBACKS------------------------
//--------------------------------------------------------------

void ofApp::draw()
{
    sceneManager.draw();
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
    sceneManager.mouseMoved(mouse);
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
