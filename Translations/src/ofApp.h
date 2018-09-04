#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxJSONElement.h"
#include "GuiApp.hpp"
#include "ofxLibwebsockets.h"
#include "SceneManager.hpp"
#include "ofxFontStash.h"

#define NUM_MESSAGES 2

struct translated {
    
    string uniqueID;
    string raw, trans;
    
    
};

//#define DATASETMODE

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void exit();
    
        // websocket methods
        void onConnect( ofxLibwebsockets::Event& args );
        void onOpen( ofxLibwebsockets::Event& args );
        void onClose( ofxLibwebsockets::Event& args );
        void onIdle( ofxLibwebsockets::Event& args );
        void onMessage( ofxLibwebsockets::Event& args );
        void onBroadcast( ofxLibwebsockets::Event& args );
    
        void drawContourFinder();
        void drawTranslations();
    
        void parseTranslation( ofxLibwebsockets::Event& args);
    
        void sendScenariosToSocket();
    
        translated * getTranslatedForID(string uniqueID);
    
        // common
        shared_ptr<GuiApp> gui;
    
        // interface
        ofImage tache_init, tache_draw, pattern, avatar ;
        ofFbo translationsFbo;
        vector<ofImage> accueil;
        ofTrueTypeFont font;
        ofxFontStash fontStash;

        int background;
    
        int   appFPS;
        float sequenceFPS;
    
        // sockets
        ofxLibwebsockets::Server server;
        bool bSetup;
        vector<string> messages;
        vector<translated> translations;
        int translationFontSize;
    
        // scene
    
        SceneManager sceneManager;
        string currentLabel;
    
        bool bEnableDetection;
    
    
#ifdef DATASETMODE
    
        ofImage screen;
        vector<quickDrawShape> shapes;
        int currentIndex;
        ofImage templateImg;
        string label;
    
        void ndJsonTest();
        void saveImages();
    
#endif
    
    
    
};
