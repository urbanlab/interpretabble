#include "ofMain.h"
#include "ofApp.h"
#include "GuiApp.hpp"

//========================================================================
int main( ){
    
    ofGLWindowSettings settings;
    settings.width = 600;
    settings.height = 600;
    settings.setPosition(ofVec2f(2400,0));
    auto mainWindow = ofCreateWindow(settings);
    
    settings.width = 1280;
    settings.height = 1080;
    settings.setPosition(ofVec2f(0,0));
    auto guiWindow = ofCreateWindow(settings);
    
    auto mainApp = make_shared<ofApp>();
    auto guiApp = make_shared<GuiApp>();
    
    mainApp->gui = guiApp;
    
    ofRunApp(guiWindow, guiApp);
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
    
    /*
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
     
     */

}
