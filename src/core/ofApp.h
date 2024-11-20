#ifndef OFAPP_H__
#define OFAPP_H__

#include "ofMain.h"
#include "ofTexture.h"
#include "SceneManager.h"
#include "PhysicsController.h"
#include "InputManager.h"

class ofApp : public ofBaseApp
{
public:
    ~ofApp();
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(ofMouseEventArgs& mouse);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    // light
    ofLight sun;

    // objects (entities)
    // std::vector<Entity*> entities; replaced with manager
    SceneManager sceneManager;

    // cam
    ofCamera cam;
    float cameraFloor;
    float moveSpeed; // player camera movement speed
    int currentTargetIndex;
    float cameraSpeed; // auto camera movement speed
    std::vector<glm::vec3> path;
    glm::vec3 cameraPosition;
    bool followPathBlindly = true;

    // controls
    bool isCursorHidden = false;
    InputManager inputManager;
    bool mainInputContext[NUM_KEYS] = {};

private:
    void followPath(std::vector<glm::vec3>& pathPoints);
};

#endif
