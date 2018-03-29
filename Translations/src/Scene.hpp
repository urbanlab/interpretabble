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
        
    }
    
    void update() {
        
        float dt = 1.0f / 60.0f;
        opacity.update( dt );
        
        if(video.isLoaded()) {
            video.update();
        }
    }
    
    void draw() {
        
        ofEnableAlphaBlending();
        ofSetColor(255, opacity.val());
        
        if(image.isAllocated()) {
            image.draw(0.0, 0.0);
            ofLogNotice("drawing image") << opacity.val();

        }
        
        if(video.isLoaded()) {
            video.draw(0.0, 0.0);
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
        
    }
    
    ofImage image;
    ofVideoPlayer video;
    ofxAnimatableFloat opacity;

    
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
