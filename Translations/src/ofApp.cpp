#include "ofApp.h"

using namespace ofxCv;
using namespace cv;
//--------------------------------------------------------------
void ofApp::setup(){
    
    
    ofSetVerticalSync(true);
    
    ofxLibwebsockets::ServerOptions options = ofxLibwebsockets::defaultServerOptions();
    options.port = 9092;
    options.bUseSSL = false; // you'll have to manually accept this self-signed cert if 'true'!
    options.documentRoot = ofToDataPath("../../translator");
    bSetup = server.setup( options );
    server.addListener(this);
    
    bEnableDetection = true;
    
    translationFontSize = 12;
    font.load("assets/fonts/CALVERTMTSTD-BOLD.OTF", translationFontSize);
    fontStash.setup("assets/fonts/CALVERTMTSTD-BOLD (1).ttf", 1.0, 1024, false, 8, 1.5);
    sceneManager.setup();

#ifdef DATASETMODE
    
    ndJsonTest();
    
#else
    
    tache.load("assets/images/tache.png");
    pattern.load("assets/images/fond.png");
    avatar.load("assets/images/avatar.png");

#endif
    
    ofToggleFullscreen();
    sendScenariosToSocket();
    
}

void ofApp::update() {
    
    sceneManager.update();

   
    
}

void ofApp::draw() {
    
#ifdef DATASETMODE
    saveImages();
    return;
#endif
    
    ofBackground(180);
    ofSetColor(255);
    ofEnableAlphaBlending();
    tache.draw(0.0, 0.0);
    pattern.draw(0.0,0.0);
    avatar.draw(0.0, 0.0);
    
    drawTranslations();
    
    sceneManager.draw();
    
    ofSetColor(0);
    font.drawString(currentLabel, 340,  ofGetHeight() - translationFontSize );
    
    ofEnableAlphaBlending();
    ofSetColor(0, background);
    ofDrawRectangle(0.0,0.0, ofGetWidth(), ofGetHeight());

    if ( !bSetup ){
        ofSetColor(255,0,0);
        ofDrawBitmapString("WebSocket server is not setup!!!", 20, 20);
    }
    
}

void ofApp::drawContourFinder() {
    
    ofSetColor(0,255,255);
    ofPushMatrix();
    ofTranslate(gui->translateX ,gui->translateY );
    vector<ofPolyline> polys = gui->contourFinder.getPolylines();
    for(int i=0; i<polys.size(); i++) {
        polys[i].draw();
        ofDrawBitmapString(gui->contourFinder.getLabel(i), polys[i].getBoundingBox().x, polys[i].getBoundingBox().y);
    }
    ofPopMatrix();
    
}

void ofApp::drawTranslations() {
    
    ofPushMatrix();
    ofTranslate(20, 800);
    float x = 0;
    float y = 0;
    
    int size = translations.size();
    for (int i = 0; i < size; i++ ){
        
        int index =  translations.size() - i - 1;
        
        int numLines = 0;
        bool wordsWereCropped;
        ofSetColor(0,0,0);
        ofRectangle column;
         if( translations[index].raw.size() > 0) {
             
            // ofLogNotice("hey") << translations[index].raw;
             column = fontStash.drawMultiLineColumn(    translations[index].raw,            /*string*/
                                                     translationFontSize * 1.5,            /*size*/
                                                     x, y,        /*where*/
                                                     300, /*column width*/
                                                     numLines,    /*get back the number of lines*/
                                                     false,        /* if true, we wont draw (just get bbox back) */
                                                     9,            /* max number of lines to draw, crop after that */
                                                     true,        /*get the final text formatting (by adding \n's) in the supplied string;
                                                                   BE ARWARE that using TRUE in here will modify your supplied string! */
                                                     &wordsWereCropped, /* this bool will b set to true if the box was to small to fit all text*/
                                                     false        /*centered*/
                                                     );
        
             y += column.height + 10;
        }
        
        if(translations[index].trans.size() > 0) {
            
            ofSetColor(200);
            column = fontStash.drawMultiLineColumn(   translations[index].trans,            /*string*/
                                                               translationFontSize * 1.5,            /*size*/
                                                               x, y,        /*where*/
                                                               300, /*column width*/
                                                               numLines,    /*get back the number of lines*/
                                                               false,        /* if true, we wont draw (just get bbox back) */
                                                               9,            /* max number of lines to draw, crop after that */
                                                               true,        /*get the final text formatting (by adding \n's) in the supplied string;
                                                                             BE ARWARE that using TRUE in here will modify your supplied string! */
                                                               &wordsWereCropped, /* this bool will b set to true if the box was to small to fit all text*/
                                                               false        /*centered*/
                                                               );
            y += column.height + 20;
        }

        /*
        if(translations[index].trans.size() > 0) {
            ofSetColor(255,0,0);
            font.drawString(translations[index].raw , x, i * ( translationFontSize  * 4) );
        }
        
        if(translations[index].raw.size() > 0) {
            ofSetColor(0);
            font.drawString(translations[index].trans , x, i * ( translationFontSize  * 4 ) + translationFontSize *2);
        }
         
         */
        
    }
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(320, 260);
    ofRotateZ(180);
    x = 0;
    y = 0;
    
    for (int i = 0; i < translations.size(); i++ ){
        
        int index =  translations.size() - i - 1;
        
        int numLines = 0;
        bool wordsWereCropped;
        ofSetColor(0,0,0);
        ofRectangle column;
        if( translations[index].raw.size() > 0) {
            
            // ofLogNotice("hey") << translations[index].raw;
            column = fontStash.drawMultiLineColumn(    translations[index].raw,            /*string*/
                                                   translationFontSize * 1.5,            /*size*/
                                                   x, y,        /*where*/
                                                   300, /*column width*/
                                                   numLines,    /*get back the number of lines*/
                                                   false,        /* if true, we wont draw (just get bbox back) */
                                                   9,            /* max number of lines to draw, crop after that */
                                                   true,        /*get the final text formatting (by adding \n's) in the supplied string;
                                                                 BE ARWARE that using TRUE in here will modify your supplied string! */
                                                   &wordsWereCropped, /* this bool will b set to true if the box was to small to fit all text*/
                                                   false        /*centered*/
                                                   );
            
            y += column.height + 10;
        }
        
        if(translations[index].trans.size() > 0) {
            
            ofSetColor(200);
            column = fontStash.drawMultiLineColumn(   translations[index].trans,            /*string*/
                                                   translationFontSize * 1.5,            /*size*/
                                                   x, y,        /*where*/
                                                   300, /*column width*/
                                                   numLines,    /*get back the number of lines*/
                                                   false,        /* if true, we wont draw (just get bbox back) */
                                                   9,            /* max number of lines to draw, crop after that */
                                                   true,        /*get the final text formatting (by adding \n's) in the supplied string;
                                                                 BE ARWARE that using TRUE in here will modify your supplied string! */
                                                   &wordsWereCropped, /* this bool will b set to true if the box was to small to fit all text*/
                                                   false        /*centered*/
                                                   );
            y += column.height + 20;
        }
        
    }
    ofPopMatrix();
    
    ofEnableAlphaBlending();
    //translationsFbo.draw(0.0,0.0);
    


    //ofPopMatrix();
    //translationsFbo.getTexture().mi
    
    // delete messages if too much
    if (messages.size() > NUM_MESSAGES) messages.erase( messages.begin() );

}


#ifdef DATASETMODE
void ofApp::ndJsonTest() {
    
    label = "nose";
    vector<string> jsons = ofSplitString(ofBufferFromFile("quickdraw/"+label+".ndjson").getText(), "\n");
    ofxJSONElement result;
    
    for (int l=0; l< jsons.size(); l++) {
        
        quickDrawShape shape;
        
        bool parsingSuccessful = result.parse(jsons[l]);
        
        if (parsingSuccessful) {
            
            if(result["recognized"].asBool()) {
            Json::Value& shapes = result["drawing"];
            
            for (Json::ArrayIndex i = 0; i < shapes.size(); ++i) {
                
                Json::Value& xValues = shapes[i][0];
                Json::Value& yValues = shapes[i][1];
                
                ofPolyline polyLine;
                for (Json::ArrayIndex j = 0; j < xValues.size(); ++j) {
                    
                    int x = xValues[j].asInt();
                    int y = yValues[j].asInt();
                    
                    polyLine.addVertex(x, y);
                    
                }
                
                shape.polylines.push_back(polyLine);
                
                }
            }
        }
        
        shapes.push_back(shape);
    }

}


void ofApp::saveImages() {
    
    if(currentIndex > shapes.size() - 1)
        exit();
    
    ofBackground(0, 0, 0);
    if(shapes.size() > 0 ) {
        
        
        int max = shapes.size();
        if(max > 10000)
            max = 10000;
        
        // ofTranslate(ofGetWidth() * .5 , ofGetHeight() * .5 );
        for(int j=0; j<max;j++ ) {
            ofBackground(255);
            ofSetColor(0);
            
            int max = shapes.size();
            if(max > 10000)
                max = 10000;
            
            ofLogNotice("draw number ") << j << " / " + ofToString(max);
            for(int i=0; i<shapes[j].polylines.size(); i++ ) {
                
                shapes[j].polylines[i].draw();
                screen.grabScreen(0,0, 256, 256);
                screen.save(label + "/" + ofToString(j) + ".jpg");
                
            }
        }
        
        
    }
    
    
}
#endif

void ofApp::exit() {
    
    //gui->exit();
    
}

//--------------------------------------------------------------
// Web socket events
//--------------------------------------------------------------
void ofApp::onBroadcast( ofxLibwebsockets::Event& args ){
    cout<<"got broadcast "<<args.message<<endl;
}

void ofApp::mousePressed(int x, int y, int button) {
    
    string url = "http";
    if ( server.usingSSL() ){
        url += "s";
    }
    url += "://localhost:" + ofToString( server.getPort() );
    ofLaunchBrowser(url);
}

void ofApp::keyPressed(int key) {
    
    
    
}

void ofApp::sendScenariosToSocket() {
    
    gui->labels.clear();
    gui->labels.push_back("rien");
    for(int i=0; i<sceneManager.scenes.size(); i++) {
        
        gui->labels.push_back(sceneManager.scenes[i]->label);
        string message = ofToString(i) + "|" + "SCENARIO_DATA" + "|" + sceneManager.scenes[i]->label + "|NULL";
        ofLogNotice("message:") << message;
        server.send(message);
    }
    
}



//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 'f')
       ofToggleFullscreen();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}



//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


//--------------------------------------------------------------
void ofApp::onConnect( ofxLibwebsockets::Event& args ){
    cout<<"on connected"<<endl;
}

//--------------------------------------------------------------
void ofApp::onOpen( ofxLibwebsockets::Event& args ){
    cout<<"new connection open"<<endl;
    sendScenariosToSocket();
   // messages.push_back("New connection from " + args.conn.getClientIP() + ", " + args.conn.getClientName() );
}

//--------------------------------------------------------------
void ofApp::onClose( ofxLibwebsockets::Event& args ){
    cout<<"on close"<<endl;
    //messages.push_back("Connection closed");
}

//--------------------------------------------------------------
void ofApp::onIdle( ofxLibwebsockets::Event& args ){
    cout<<"on idle"<<endl;
}

//--------------------------------------------------------------
void ofApp::onMessage( ofxLibwebsockets::Event& args ){
    
    parseTranslation(args);
   
}

void ofApp::parseTranslation( ofxLibwebsockets::Event& args) {
    
    // trace out string messages or JSON messages!
    if ( !args.json.isNull() ){
        messages.push_back(args.json.toStyledString()  );
    } else {
        messages.push_back( args.message );
    }
    
    // split result
    vector<string> splitted = ofSplitString(args.message, "|");
    string id               = splitted[0];
    string type             = splitted[1];
    string mesg             = splitted[2];
    
    
    if( type == "SCENARIO_CHANGE") {
        
        int _id = ofToInt(id);
        if(_id == 0)
            bEnableDetection = true;
        else
            bEnableDetection = false;
        
        sceneManager.setCurrentLabel(_id);
        
        
    }
    
    if( type == "SCENARIO_OVERWRITE") {
        
        bEnableDetection = ofToBool(id);
        return;
        
    }
    
    // check if already exist
    translated  * trans = getTranslatedForID(id);
    
    if(trans) {
        
        // update
        trans->uniqueID = id;
        if(type == "RAW")
            trans->raw = mesg;
        if(type == "TRANS")
            trans->trans = mesg;
        
    } else {
        
        // create & store
        translated trans;
        trans.uniqueID = id;
        if(type == "RAW")
            trans.raw = mesg;
        if(type == "TRANS")
            trans.trans = mesg;
        
        translations.push_back(trans);
    }
    
    // if type is not a translation we send for translation
    //if(type == "RAW")
        // send back for traduction
    
    ofLogNotice("send back to server") << args.message;
    server.send( args.message );
    
    
    // is that necessary?
    args.conn.send( args.message );
    
}


translated * ofApp::getTranslatedForID(string uniqueID) {
    
    for( int i=0; i<translations.size(); i++) {
        
        if(uniqueID == translations[i].uniqueID)
            return &translations[i];
    
    }
    
    return NULL;
}



