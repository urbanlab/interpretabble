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
        
        if(video.isLoaded()) {
            video.update();
        }
    }
    
    void draw() {
        
        if(image.isAllocated()) {
            image.draw(0.0, 0.0);
        }
        
        if(video.isLoaded()) {
            video.draw(0.0, 0.0);
        }
         
    }
    
    float getHeight() {
        
    }
    
    ofImage image;
    ofVideoPlayer video;
    
    
};

class Scene {
    
public:
    
    string label;
    vector<Asset> assets;
    
    void update();
    void draw();
    void onStart();
    void onEnd();
    
private:
    
    
};

#endif /* Scene_hpp */
