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
        {"hero", "Tag for player entities (should be attached to player)"},
        {"blob", "Tag for enemy entities"}
    };
    std::unordered_map<std::string, PhysicsMetadata> customPhysicsTags = {
        {"hero_physics", PhysicsMetadata("hero_physics", nullptr, nullptr, nullptr)},
        {"blob_physics", PhysicsMetadata("blob_physics", nullptr, nullptr, nullptr)}
    }; // PhysicsMetadata("physics_tag", bounding equation, normal equation, extra context void*)


    // Initialize TagManager (you can call this as many times as you want to add more tags)
    TagManager::initialize(customTags, customPhysicsTags);

    // Light setup (next system after cleanup)
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
    sceneManager.addScene<IslandScene>();
    sceneManager.addScene<PopperScene>();
    sceneManager.addScene<JolfBaseScene>();
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
//-----------------------UPDATE-CALLBACK------------------------
//--------------------------------------------------------------

void ofApp::update()
{
    sceneManager.update();

    if (inputManager.getPressedOnce('1')) sceneManager.prev();
    if (inputManager.getPressedOnce('2')) sceneManager.next();
    if (inputManager.getPressedOnce('t')) sceneManager.toggleStaticMesh();
    if (inputManager.getPressedOnce('e')) followPathBlindly = !followPathBlindly;
}

//--------------------------------------------------------------
//------------------------DRAW-CALLBACK-------------------------
//--------------------------------------------------------------

void ofApp::draw()                                      { sceneManager.draw(); }

//--------------------------------------------------------------
//-----------------------INPUT-CALLBACKS------------------------
//--------------------------------------------------------------
void ofApp::keyPressed(int key)                         { inputManager.ofKeyPressed(key); } // use inputManager.get(key) to check if a key is pressed
void ofApp::keyReleased(int key)                        { inputManager.ofKeyReleased(key); }
void ofApp::mouseMoved(ofMouseEventArgs& mouse)         { inputManager.ofMouseMoved(mouse); }
void ofApp::mouseDragged(int x, int y, int button)      { inputManager.ofMouseDragged(x, y, button); }
void ofApp::mousePressed(int x, int y, int button)
{
    inputManager.ofMousePressed(x, y, button);

    if ( button == 0)
    {
        if (isCursorHidden)
            ofShowCursor();
        else
            ofHideCursor();
        isCursorHidden = !isCursorHidden;
    }
}
void ofApp::mouseReleased(int x, int y, int button)     { inputManager.ofMouseReleased(x, y, button); }
void ofApp::mouseEntered(int x, int y)                  { inputManager.ofMouseEntered(x, y); }
void ofApp::mouseExited(int x, int y)                   { inputManager.ofMouseExited(x, y); }
void ofApp::windowResized(int w, int h)                 { inputManager.ofWindowResized(w, h); }
void ofApp::gotMessage(ofMessage& msg)                   { inputManager.ofGotMessage(msg); }
void ofApp::dragEvent(ofDragInfo& dragInfo)              { inputManager.ofDragEvent(dragInfo); }

