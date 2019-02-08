#include "ofMain.h"
#include "ofApp.h"
#include "GuiApp.hpp"

//========================================================================
int main( ){
    
    ofGLWindowSettings settings;
    settings.setSize(600,600);
    settings.setPosition(ofVec2f(2400,0));
    auto mainWindow = ofCreateWindow(settings);
    
    settings.setSize(1280,1080);
    settings.setPosition(ofVec2f(0,0));
    auto guiWindow = ofCreateWindow(settings);
    
    auto mainApp = make_shared<ofApp>();
    auto guiApp = make_shared<GuiApp>();
    
    mainApp->gui    = guiApp;
    guiApp->app     = mainApp;
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
