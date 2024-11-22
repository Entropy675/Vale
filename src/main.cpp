#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( )
{

    //Use ofGLFWWindowSettings for more options like multi-monitor fullscreen
    ofGLWindowSettings settings;
    settings.setSize(1024, 768);
    settings.windowMode = OF_FULLSCREEN; //can also be OF_WINDOW

    auto window = ofCreateWindow(settings);

    ofRunApp(window, make_shared<ofApp>());
    ofRunMainLoop();

}
