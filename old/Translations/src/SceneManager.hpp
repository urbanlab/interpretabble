//
//  SceneManager.hpp
//  Translations
//
//  Created by Martial Geoffre-Rouland on 29/03/2018.
//

#ifndef SceneManager_hpp
#define SceneManager_hpp

#include <stdio.h>
#include "ofMain.h"
#include "Scene.hpp"
//#include "ImageSequencePlayer.hpp"
class SceneManager {
  
public:
    
    void setup();
    void update();
    void draw();
    void loadAssets();
    void setCurrentLabel(string label);
    void setCurrentLabel(int id);

    void nextLabel();
    
    void onSceneChanged();
    void onSceneOutHandler();

    
    vector<Scene*> scenes;
    
    int nFramesDelaymax;
    bool bOnNothing;
    
private:
    
    //ImageSequencePlayer accueil;
    int currentSceneIndex;
    
    int nFramesIndexChangedDelay;
    bool bHasChanged;
    int tempIndex;
};

#endif /* SceneManager_hpp */
