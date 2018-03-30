//
//  Scene.hpp
//  Translations
//
//  Created by Martial Geoffre-Rouland on 29/03/2018.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ofxAnimatableFloat.h"
#include "ImageSequencePlayer.hpp"

class Asset {
  
public:
    
    void addAsset(string path, string ext) {
        
        // get extension and load
        if (ext == "png" || ext == "jpg") {
            image.load(path);
        }
        
        if (ext == "mov" || ext == "mp4") {
            video.load(path);
            video.play();
        }
        
        if(ext == "folder" ) {
            
            ofLogNotice("yeah folder");
            sequence.setup();
            sequence.loadFolder(path);
            sequence.setFps(30);
            sequence.setLoop(true);
            sequence.play();
            
        }
        
    }
    
    void play () {
        if(sequence.bIsLoaded){
            sequence.play();
            
        }
    }
    
    void update() {
        
        float dt = 1.0f / 60.0f;
        opacity.update( dt );
        
        if(video.isLoaded()) {
            video.update();
        }
        
        if(sequence.bIsLoaded){
            sequence.update();

        }
    }
    
    void draw() {
        
        ofEnableAlphaBlending();
        ofSetColor(255, opacity.val());
        
        if(image.isAllocated()) {
            image.draw(0.0, 0.0);

        }
        
        if(video.isLoaded()) {
            video.draw(0.0, 0.0);
        }
        
        if(sequence.bIsLoaded) {
            sequence.draw(0.0,0.0, sequence.getWidth(), sequence.getHeight());
            ofLogNotice("drawing folder ") << sequence.getWidth() << " " << sequence.getHeight();

        }
        
        ofDisableAlphaBlending();
         
    }
    
    float getHeight() {
        
        if(image.isAllocated()) {
            return image.getHeight();
        }
        if(video.isLoaded()) {
            return video.getHeight();
        }
        
        if(sequence.bIsLoaded) {
            return sequence.getHeight();
        }
        
    }
    
    ofImage image;
    ofVideoPlayer video;
    ofxAnimatableFloat opacity;
    ImageSequencePlayer sequence;
    
};

class Scene {
    
public:
    
    string label;
    vector<Asset> assets;
    
    void update();
    void draw();
    void onStart();
    void onEnd();
    
    ofImage toolsImage;

    
private:
    
    
    
};

#endif /* Scene_hpp */
