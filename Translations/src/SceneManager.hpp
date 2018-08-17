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

class SceneManager {
  
public:
    
    void setup();
    void update();
    void draw();
    void loadAssets();
    void setCurrentLabel(string label);
    void nextLabel();
    
    void onSceneChanged();
    void onSceneOutHandler();

    
    vector<Scene*> scenes;
    bool on;
    bool onPause;
    
private:
    
    int currentSceneIndex;
    
};

#endif /* SceneManager_hpp */
