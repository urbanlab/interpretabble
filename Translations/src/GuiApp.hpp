//
//  GuiApp.hpp
//  Translations
//
//  Created by Martial Geoffre-Rouland on 22/03/2018.
//

#pragma once

#include "ofMain.h"
//#include "ofApp.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxJSONElement.h"
#include "ofxXmlSettings.h"
#include "ofxCcvThreaded.h"
#include "PipelineThreaded.h"

#define SIZE_INPUT_VECTOR 4096

struct quickDrawShape {
    
    vector<ofPolyline> polylines;
    
};

class ofApp;

class GuiApp : public ofBaseApp{
    
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
    
    void changeCamera();

    
    int count;
    ofVideoGrabber cam;
    ofxCv::ContourFinder contourFinder;
    //ofColor targetColor;
    vector<ofImage> croppedDrawings;
    vector<int> croppedIds;
    
    ofxPanel gui;
    ofParameterGroup parameters;
    ofParameter<float> scale;
    ofParameter<float> brightness;
    
    ofParameter<float> lerpAmt;

    ofParameter<float> threshold;
    ofParameter<ofColor> targetColor;

    ofParameter<bool> trackHs;
    ofParameter<bool> calibrationMode;
    
    ofParameter<int> croppedRectX,croppedRectY, croppedRectW, croppedRectH;
    ofParameter<int> translateX, translateY;
    ofParameter<bool> saveCropped;
    ofParameter<int> backgroundOpacity;
    ofParameter<bool> videoSettings;
    
    ofParameter<float> camThresoldSlider, brigthness;

    ofImage camImage, camThresold;
    
    ofxXmlSettings xmlSettings;
    
    //----- CCV
    // learning
    vector<PipelineThreaded*> learners;
    vector<ofParameter<float> > sliders, targetSliders;
    vector<int> categoricals;
    
    // input/cv
    ofxCcvThreaded ccv;
    vector<float> featureEncoding;
    bool isTraining;
    int numSamples;
    
    ofxPanel guiSliders;
    
    ofxButton bTrain, bSave, bLoad, bClear, bAddSlider, bAddCategorical, bOscSettings, bCameraSettings, bAddLabel;
    ofxToggle tRecord, tPredict;

    
    void updateCCV();
    void updateParameters();
    RegressionThreaded * addSlider();
    CategoricalThreaded * addCategorical(int numClasses);
    void eAddSlider();
    void eAddCategorical();
    void eSlider(float & v){};
    void eCategorical(int & v){};
    void eAddLabel();
    void save(string modelName);
    void load(string modelPath);
    void train();

    void eSave();
    void eLoad();
    void clear();
    
    void saveLabels();
    void loadLabels();
    string infoText;
    
    vector<string> labels;
    
    shared_ptr<ofApp>  app;
  
};

