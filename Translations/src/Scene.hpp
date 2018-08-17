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
        
        if (ext == "mov" || ext == "mp4" || ext =="avi") {
            video.load(path);
            video.play();
        }
        
        if(ext == "dir") {
            ofDirectory dir;
            int nFiles = dir.listDir(path);
            if(nFiles) {
                
                for(int i=0; i<dir.numFiles()-1; i++) {
                    
                    // add the image to the vector
                    string filePath = dir.getPath(i);
                    ofLogNotice("AddAsset ") << filePath;
                    images.push_back(ofImage());
                    images.back().load(filePath);
                    
                }
                
            } else ofLog(OF_LOG_WARNING) << "Could not find folder";
            
            // this toggle will tell the sequence
            // to be indepent of the app fps
            bFrameIndependent = true;
            
            // this will set the speed to play
            // the animation back we set the
            // default to 24fps
            sequenceFPS = 24;
            
            // set the app fps
            appFPS = 60;
            ofSetFrameRate(appFPS);
            
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

        }
        
        if(video.isLoaded()) {
            video.draw(0.0, 0.0);
        }
        
        if(images.size()>0){
            ofLogNotice("Draw ") << images.size();
            uint64_t frameIndex = 0;
            
            if(bFrameIndependent) {
                // calculate the frame index based on the app time
                // and the desired sequence fps. then mod to wrap
                frameIndex = (int)(ofGetElapsedTimef() * sequenceFPS)% images.size();
            }
            else {
                // set the frame index based on the app frame
                // count. then mod to wrap.
                frameIndex = ofGetFrameNum() % images.size();
            }
            
            // draw the image sequence at the new frame count
            images[frameIndex].draw(0.0, 0.0);
            
            
            // draw where we are in the sequence
            float x = 0;
            for(int offset = 0; offset < 5; offset++) {
                int i = (frameIndex + offset) % images.size();
                images[i].draw(0.0+x, ofGetHeight()-40, 40, 40);
                x += 40;
            }
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
        
        if(images.size()>0){
            return images[0].getHeight();
        }
        
    }
    
    ofImage image;
    ofVideoPlayer video;
    ofxAnimatableFloat opacity;
    vector<ofImage> images;
    
    int   appFPS;
    float sequenceFPS;
    bool  bFrameIndependent;

    
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
    ofVideoPlayer accueil;

    
private:
    
    
    
};

#endif /* Scene_hpp */
