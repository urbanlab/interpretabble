//
//  GuiApp.cpp
//  Translations
//
//  Created by Martial Geoffre-Rouland on 22/03/2018.
//

#include "GuiApp.hpp"
#include "ofApp.h"

using namespace ofxCv;
using namespace cv;
//--------------------------------------------------------------
void GuiApp::setup(){
    
    
    cam.setDeviceID(1);
    cam.setup(1920, 1080);
    contourFinder.setMinAreaRadius(10);
    contourFinder.setMaxAreaRadius(150);
    //contourFinder.setInvert(true); // find black instead of white
    
    gui.setup();
    parameters.add(scale.set("Scale", 0.8, 0.1, 1.0));
    //parameters.add(targetColor.set("color", ofColor(255)));
    parameters.add(brightness.set("blur", 3, 0, 255));

    parameters.add(threshold.set("Threshold", 128, 0, 255));
    parameters.add(trackHs.set("Track Hue/Saturation", false));
    parameters.add(calibrationMode.set("Calibration", false));
    
    parameters.add(croppedRectX.set("Cropped X", 333, 0, cam.getWidth()));
    parameters.add(croppedRectY.set("Cropped Y", 548, 0, cam.getHeight()));
    parameters.add(croppedRectW.set("Cropped W", 365, 0,cam.getWidth()));
    parameters.add(croppedRectH.set("Cropped H", 254, 0, cam.getHeight()));
    parameters.add(backgroundOpacity.set("Opacity", 0, 0, 255));

    
    parameters.add(lerpAmt.set("Prediction lerp", 0.2, 0.01, 1.0));


  //  parameters.add(translateX.set("Translate X", 100, 0, cam.getHeight()));
   // parameters.add(translateY.set("translate Y ", 100, 0, cam.getHeight()));
   // parameters.add(saveCropped.set("Save cropped ", false));
    parameters.add(camThresoldSlider.set("thresold cam", 120, 0.0, 255));

    //parameters.add(videoSettings.set("Video settings ", false));
    

    gui.add(parameters);
    
   // gui.add(tRecord.setup("Record", false));
   // gui.add(bClear.setup("Clear training data"));
    //gui.add(bTrain.setup("Train"));
   // gui.add(tPredict.setup("Predict", false));
    //gui.add(bSave.setup("Save model"));
   // gui.add(bLoad.setup("Load model"));
    gui.add(bCameraSettings.setup("Camera settings"));
    
    //gui.add(frameDelay.set("Frame delay", 200, 0, 1000));
    //gui.add(brightness.set("brightness cam", 0.0, 0.0, 1.0));

    

    gui.loadFromFile("settings.xml");
    
    tRecord = false;
    tPredict = false;

    guiSliders.setup();
    guiSliders.setPosition(580, 10);
    guiSliders.setName("Outputs");
    //guiSliders.add(bAddCategorical.setup("Add Categorical"));
   // guiSliders.add(bAddLabel.setup("set label"));

   // bTrain.addListener(this, &GuiApp::train);
   // bSave.addListener(this, &GuiApp::eSave);
   // bLoad.addListener(this, &GuiApp::eLoad);
   // bClear.addListener(this, &GuiApp::clear);
   // bAddSlider.addListener(this, &GuiApp::eAddSlider);
   // bAddCategorical.addListener(this, &GuiApp::eAddCategorical);
   // bAddLabel.addListener(this, &GuiApp::eAddLabel);
    bCameraSettings.addListener(this, &GuiApp::changeCamera);

    labels.reserve(999);
    string emptyLabel = "No label";
    for(int i=0; i<999; i++) {
        labels.push_back(emptyLabel);
    }
    
    // CCV
    string ccvPath = ofToDataPath("models/image-net-2012.sqlite3");
    ccv.setup(ccvPath);
    if (!ccv.isLoaded()) return;
    ccv.setEncode(true);
    ccv.start();
    
    tRecord = false;
    tPredict = false;
    
    // caption OSC
    osc.setup(2223);
    sender.setup("localhost", 2222);
    
    ofxOscMessage mess;
    mess.setAddress("/img");
    mess.addStringArg(ofToDataPath("output.jpg", true));
    sender.sendMessage(mess, false);
    
}

void GuiApp::update() {
    
    
    app->background = backgroundOpacity;
    app->sceneManager.nFramesDelaymax = frameDelay;
    cam.update();
    
    if(cam.isFrameNew()) {
        
        
        // clear
        for(int i=0; i<croppedDrawings.size(); i++)
            croppedDrawings[i].clear();
        
        croppedDrawings.clear();
        croppedIds.clear();
        
        // get image from camera
        
        camImage.setFromPixels(cam.getPixels());
        camImage.crop((int)croppedRectX, (int)croppedRectY, (int)croppedRectW, (int)croppedRectH);
        
        convertColor(camImage, camThresold, CV_RGB2GRAY);
        float thresoldValue = camThresoldSlider;
        ofxCv::blur(camThresold, camThresold, brightness);
        ofxCv::threshold(camThresold,thresoldValue);
        camThresold.update();
        
       // camImage.resize(camImage.getWidth() * scale, camImage.getHeight() * scale);
        
        //contourFinder.setTargetColor(targetColor, trackHs ? TRACK_COLOR_HS : TRACK_COLOR_RGB);
        //contourFinder.setThreshold(threshold);
        //contourFinder.findContours(camImage);
        
        /*
        for(int i=0; i < contourFinder.getContours().size(); i++ ) {
            
            cv::Rect boundingBox = contourFinder.getBoundingRect(i);
            ofImage camImageClone = camImage;
            camImageClone.crop(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height);
            croppedDrawings.push_back(camImageClone);
            croppedIds.push_back(contourFinder.getLabel(i));
            
        }
         */
        //updateCCV();
        
        
       
        

    }
    
    // update the cameras
    while(osc.hasWaitingMessages()){
        
        
        // get the next message
        ofxOscMessage m;
        osc.getNextMessage(m);
        
        //  ofLogNotice("message") << m.getAddress();
        string caption = m.getAddress();
        // check for mouse moved message
        // both the arguments are int32's
        //caption = m.getArgAsString(0);
        
        //vector<string> splitted = ofSplitString(caption, "_");
        //string id = splitted[0];
        //caption = splitted[1];

        ofLogNotice("Message received ") << caption;
        
        app->currentLabel = caption;
        
        camThresold.save("output.jpg");
        ofxOscMessage mess;
        mess.setAddress("/img");
        mess.addStringArg(ofToDataPath("output.jpg", true));
        sender.sendMessage(mess, false);
        
        
        
    }
    
}

void GuiApp::updateCCV() {
    // check if ccv loaded
    if (!ccv.isLoaded()) {
        return;
    }
    
    // update training status
    if (isTraining) {
        isTraining = false;
        for (int p=0; p<learners.size(); p++) {
            if (learners[p]->training) {
                isTraining = true;
            }
        }
        if (!isTraining) {
            infoText = "Pipeline trained";
            for (int p=0; p<learners.size(); p++) {
                if (!learners[0]->success) {
                    infoText =  "WARNING: Failed to train pipeline";
                }
            }
            ofBackground(150);
        }
        else if (ofGetFrameNum() % 15 == 0) {
            ofBackground(ofRandom(255),ofRandom(255),ofRandom(255));
        }
    }
    
    // update parameters
    else if (tPredict) {
        updateParameters();
    }
    
    if (!cam.isFrameNew()) {
        return;
    }
    else if (cam.isFrameNew() && ccv.isReady()) {
        
        int value = -1;
        if(learners.size() == 1)
            value = ((CategoricalThreaded *) learners[0])->slider;

        
        ccv.update(camThresold, ccv.numLayers()-1, value, tRecord);
    }
    
    // record or predict
    if ((tRecord||tPredict) && ccv.hasNewResults()) {
        featureEncoding = ccv.getEncoding();
        VectorFloat inputVector(featureEncoding.size());
        for (int i=0; i<featureEncoding.size(); i++) {
            inputVector[i] = featureEncoding[i];
        }
        if(tRecord) {
            for (int p=0; p<learners.size(); p++) {
                bool success = learners[p]->addSample(&inputVector);
                if (!success){
                    infoText = "WARNING: Failed to add training sample to training data!";
                }
            }
            numSamples++;
        }
        else if (tPredict){
            for (int p=0; p<learners.size(); p++) {
                bool success = learners[p]->predict(&inputVector);
                if (!success) {
                    infoText = "ERROR: Failed to run prediction!";
                }
            }
        }
    }
}

void GuiApp::draw() {


    ofBackground(255);
    ofSetColor(255);

    
    ofPushMatrix();
    ofScale(scale,scale,scale);
    
    if(cam.isInitialized())
        cam.draw(0, 0);
    
    
    ofSetColor(255);
    float yPos = 0.0;
    for(int i=0; i<croppedDrawings.size(); i++) {
        
        croppedDrawings[i].draw(cam.getWidth(),yPos);
        yPos += croppedDrawings[i].getHeight();
        
        if(saveCropped)
            croppedDrawings[i].save("output/" + ofToString(croppedIds[i]) + ".jpg");
        count++;
        
    }
    
    ofEnableAlphaBlending();
    ofSetColor(255);
    ofNoFill();
    //ofxCv::invert(camThresold, camThresold);
    camThresold.draw(croppedRectX,croppedRectY);
    ofSetColor(255,0,0);
    ofDrawRectangle(croppedRectX, croppedRectY, croppedRectW, croppedRectH);
    
    ofTranslate(0, 0);
    ofSetLineWidth(1);
    contourFinder.draw();
    ofFill();
    ofPopMatrix();
    
    
    
    ofDrawBitmapStringHighlight( "Num samples recorded: " + ofToString(numSamples), 20, 0 + cam.getHeight() * scale );

    if (infoText != ""){
        ofDrawBitmapStringHighlight( infoText, 20, 50 + cam.getHeight() * scale );
    }
    if (learners.size() == 1 && learners[0]->isClassifier()) {
        int sliderValue = ((CategoricalThreaded *) learners[0])->slider;
        if(sliderValue < labels.size())
            ofDrawBitmapStringHighlight( "Current label: " + labels[sliderValue-1], 20, 30 + cam.getHeight() * scale);
    }
    
    if (learners.size() == 1 && learners[0]->isClassifier() && learners[0]->getTrained() && tPredict) {
        
        int sliderValue = ((CategoricalThreaded *) learners[0])->slider;
        string label = ofToString(sliderValue);
         if(sliderValue < labels.size())
             label = labels[sliderValue-1];
        
        if(app->bEnableDetection)
            app->sceneManager.setCurrentLabel(label);
        
        //app->currentLabel = label;
             
        string txt = "Predicted Class: " + ofToString(sliderValue) + " " + label;
        ofSetColor(0,255,0);
        ofDrawBitmapStringHighlight( txt, 20, 100 + cam.getHeight() * scale );

       // largeFont.drawString(txt, 237, 92 + cam.getHeight());
    }
    
    if(app->bEnableDetection)
        app->sceneManager.setCurrentLabel(app->currentLabel );
    
    gui.draw();
    guiSliders.draw();
    
    string txt = "Predicted Class: " + app->currentLabel;
    ofSetColor(0,255,0);
    ofDrawBitmapStringHighlight( txt, 20, 100 + cam.getHeight() * scale );
    

    
}

void GuiApp::exit() {
    
    // save xml settings
    //xmlSettings.serialize(parameters);
    xmlSettings.save("settings.xml");
    saveLabels();
    
    // save convnet settings
   // gui.saveToFile(ofToDataPath("settings_convnetP.xml"));
    ccv.setEncode(false);
    ccv.stop();
    for (int p=0; p<learners.size(); p++) {
        learners[p]->stopThread();
    }
    
}
// CCV

//--------------------------------------------------------------
RegressionThreaded * GuiApp::addSlider() {
    RegressionThreaded *learner = new RegressionThreaded();
    string name = "y"+ofToString(1+learners.size());
    learner->setup(name, &guiSliders, SIZE_INPUT_VECTOR);
    learner->slider.addListener(this, &GuiApp::eSlider);
    learners.push_back(learner);
    return learner;
}

void GuiApp::eAddSlider() {
    addSlider();
}
//--------------------------------------------------------------

CategoricalThreaded * GuiApp::addCategorical(int numClasses) {
    
    
    CategoricalThreaded *learner = new CategoricalThreaded();
    string name = "y"+ofToString(1+learners.size());
    learner->setup(name, &guiSliders, SIZE_INPUT_VECTOR, numClasses);
    learner->slider.addListener(this, &GuiApp::eCategorical);
    learners.push_back(learner);
        
    return learner;
        
   
}

void GuiApp::eAddCategorical() {
    
    if(learners.size() == 0) {

        string numClasses = ofSystemTextBoxDialog("How many classes?");
        if (numClasses != "") {
            addCategorical(ofToInt(numClasses));
        }
        
        //labels.clear();
        //for(int i=0; i<ofToInt(numClasses); i++)
          //  labels.push_back("Label missing");
    }
}

//--------------------------------------------------------------
void GuiApp::updateParameters() {
    for (int i=0; i<learners.size(); i++) {
        if (learners[i]->getTrained()) {
            learners[i]->update(lerpAmt);
        }
    }
}

//--------------------------------------------------------------
void GuiApp::train() {
    ofLog(OF_LOG_NOTICE, "Training...");
    tRecord = false;
    tPredict = false;
    
    for (int p=0; p<learners.size(); p++) {
        learners[p]->setupModel(0);
        learners[p]->startTraining();
        learners[p]->startThread();
    }
    
    infoText = "Training!! please wait.";
    isTraining = true;
    ofLog(OF_LOG_NOTICE, "Done training...");
}

void GuiApp::eAddLabel() {
    
    
    int current     =  ((CategoricalThreaded *) learners[0])->slider;
    string label    = ofSystemTextBoxDialog("Name the label for " + ofToString(current) );
    
    if (label != "") {
        labels[current] = label;
    }
    
}

//--------------------------------------------------------------
void GuiApp::eSave() {
    string modelName = ofSystemTextBoxDialog("Name the model");
    if (modelName != "") {
        save(modelName);
    }
}

//--------------------------------------------------------------
void GuiApp::eLoad() {
    ofFileDialogResult result = ofSystemLoadDialog("Which model to load?", true);
    if (result.bSuccess) {
        load(result.filePath);
    }
}

//--------------------------------------------------------------
void GuiApp::save(string modelName) {
    ofSystem("mkdir "+ofToDataPath(modelName+"/"));
    for (int p=0; p<learners.size(); p++) {
        string modelType = learners[p]->isClassifier() ? "c" : "r";
        learners[p]->save(ofToDataPath(modelName+"/p"+ofToString(p)+"_"+modelType+".grt"));
    }
    gui.saveToFile(ofToDataPath(modelName+"/settings.xml"));
    saveLabels();
}

//--------------------------------------------------------------
void GuiApp::load(string modelPath) {
    
    ofDirectory dir;
    dir.open(modelPath);
    int n = dir.listDir();
    for (int i=0; i<n; i++) {
        string fullPath = dir.getFile(i).getAbsolutePath();
        vector<string> path = ofSplitString(dir.getFile(i).getFileName(), "_");
        if (path.size() < 2) {
            continue;
        }
        string fullName = path[path.size()-1];
        if (fullName == "r.grt") {
            RegressionThreaded *learner = addSlider();
            learner->load(fullPath);
        }
        else if (fullName == "c.grt") {
            CategoricalThreaded * learner = addCategorical(2);
            learner->load(fullPath);
            learner->slider.set(learner->slider.getName(), 1, 1, learner->getNumClasses());
        }
    }
    if (n > 0) {
        tPredict = true;
        numSamples = learners[0]->getNumTrainingSamples();
    }
    gui.loadFromFile(modelPath+"/settings.xml");
   // loadLabels();
    
}

//--------------------------------------------------------------
void GuiApp::saveLabels(){
    
    string labels = "";
    for(int i=0; i<labels.size(); i++) {
        labels += labels[i];
        
        if(i < labels.size() - 1 )
            labels += "\n";
    }
    
    ofFile labelFile;
    labelFile.open("labels.txt",ofFile::WriteOnly);
    labelFile << labels;
    
    
    
    
    
}

void GuiApp::loadLabels(){
    
    vector < string > linesOfTheFile;
    ofBuffer buffer = ofBufferFromFile("labels.txt");
    
   // labels.clear();
   // labels.push_back("rien");
    for (auto line : buffer.getLines()){
       // labels.push_back(line);
    }
    
}

//--------------------------------------------------------------
void GuiApp::clear() {
    for (int p=0; p<learners.size(); p++) {
        learners[p]->clear();
        learners[p]->clear();
    }
    infoText = "Training data cleared";
    tPredict = false;
    numSamples = 0;
}


//--------------------------------------------------------------
void GuiApp::changeCamera() {
    string msg = "Select camera:";
    int idx = 0;
    for (auto d : cam.listDevices()) {
        msg += "\n "+ofToString(idx++)+": "+d.deviceName;
    }
    string selection = ofSystemTextBoxDialog(msg);
    if (selection != "") {
        int newDeviceId = ofToInt(selection);
        cam.setDeviceID(newDeviceId);
        cam.initGrabber(320, 240);
        //gDeviceId.set(ofToString(newDeviceId));
    }
}

void GuiApp::mousePressed(int x, int y, int button) {
    targetColor = cam.getPixels().getColor(x, y);
}

void GuiApp::keyPressed(int key) {
    
    if ( key == 'n' ) {
        app->sceneManager.nextLabel();
    }
    
    if ( key == 'o' ) {
        ofxOscMessage mess;
        mess.setAddress("/img");
        mess.addStringArg(ofToDataPath("output.jpg", true));
        sender.sendMessage(mess, false);
    }
    
}


//--------------------------------------------------------------
void GuiApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void GuiApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void GuiApp::mouseDragged(int x, int y, int button){
    
}



//--------------------------------------------------------------
void GuiApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void GuiApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void GuiApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void GuiApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void GuiApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void GuiApp::dragEvent(ofDragInfo dragInfo){
    
}
