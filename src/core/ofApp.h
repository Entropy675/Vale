#ifndef OFAPP_H__
#define OFAPP_H__

#include "ofMain.h"
#include "ofTexture.h"
#include "IslandScene.h"
#include "SceneManager.h"
#include "PhysicsController.h"

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
    // std::vector<Entity*> entities;
    SceneManager sceneManager;
	
	// scenes (loads into entities)
	IslandScene islandMap;
	
	
	// cam
    ofCamera cam;
    float cameraFloor;
    float moveSpeed; // camera movement speed
    int currentTargetIndex;
    float cameraSpeed; // auto camera movement speed
	std::vector<glm::vec3> path;
	glm::vec3 cameraPosition;
	bool followPathBlindly = true;
	
	// controls
	bool isCursorHidden = false;
    bool keys[256]; // to track which keys are pressed
    
private:
	glm::vec3 generateRandomVector(float totalDistance);
	void followPath(std::vector<glm::vec3>& pathPoints);
	void setupSandIslandMesh(ofMesh& sandMesh, float radius, int resolution, float spread, float noiseScale);
	void drawIslandMesh(ofMesh& islandMesh);
};

#endif