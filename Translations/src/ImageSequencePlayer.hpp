//
//  ImageSequencePlayer.hpp
//  5GTunnel
//
//  Created by Martial Geoffre-Rouland on 24/11/2017.
//

#ifndef ImageSequencePlayer_hpp
#define ImageSequencePlayer_hpp

#include <stdio.h>
#include "ofMain.h"

class ImageSequencePlayer {
  
public:
    
    ImageSequencePlayer();
    void setup();
    void update();
    void draw(float x, float y, float w, float h);

    void loadFolder(string path);
    void setFps(int fps);
    
    void play();
    void stop();
    void goToFrame(int frameID);
    
    int     getNumFrames();
    float   getDurationInMillis();
    float   getPosition();
    bool    isPlaying();
    
    void    setLoop(bool bLoop);
    
    void    setCustomTimingMillisEvent(int millis);
    
    void    resetCustomEvent();
    
    void    nextFrame();
    ofEvent<int> videoCustomTimingEvent;
    ofEvent<int> videoCompleteEvent;
    
    float getWidth() {
        
        return width;
        
    }
    
    float getHeight() {
        
        return height;
        
    }
    
    bool bIsLoaded;
    
    
private:
    
    void updateFrame();
    void computeDuration();
    
    vector<ofImage> images;
    
    int             customEventMillis;
    
    bool            bLoop;
    int             currentFrame;
    int             fps;
    int             savedTime;
    float           durationMillis;
    bool            bIsPlaying;
    bool            bHasShotCustomEvent;
    
    float width, height;
    
};

#endif /* ImageSequencePlayer_hpp */
