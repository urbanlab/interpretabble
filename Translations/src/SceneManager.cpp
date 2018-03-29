//
//  SceneManager.cpp
//  Translations
//
//  Created by Martial Geoffre-Rouland on 29/03/2018.
//

#include "SceneManager.hpp"


void SceneManager::setup() {
    
    currentSceneIndex = -1;
    loadAssets();
    
}

void SceneManager::update() {
    
    if(currentSceneIndex < 0 || currentSceneIndex >= scenes.size() -1 ) {
        
    } else {
        
        // draw
        scenes[currentSceneIndex].update();
        
    }
    
}

void SceneManager::draw() {
    
    // idle mode
    if(currentSceneIndex < 0 || currentSceneIndex >= scenes.size() -1 ) {
        
    } else {
        
        // draw
        scenes[currentSceneIndex].draw();
        
    }
    
}

void SceneManager::loadAssets() {
    
    // go through each folder and load assets
    string path = "scenes";
    ofDirectory dir(path);
    dir.listDir();
    
    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        
        if(dir.getFile(i).isDirectory()) {
            
            ofDirectory subdir(dir.getPath(i));
            subdir.listDir();
            
            Scene scene;
            scene.label = dir.getFile(i).getBaseName();
            
            for(int j = 0; j < subdir.size(); j++){
                
                ofFile file = subdir.getFile(j);
                string ext = file.getExtension();
                Asset asset;
                asset.addAsset(file.getAbsolutePath(), file.getExtension());
                scene.assets.push_back(asset);
                
            }
            scenes.push_back(scene);
            
        }
    }
    
}

void SceneManager::setCurrentLabel(string label) {
    
    // need to put a timer on this
    
    for(int i=0; i<scenes.size(); i++) {
        if(scenes[i].label == label)
            currentSceneIndex = i;
    }
    
}

void SceneManager::onSceneChanged() {
    
    
}


void SceneManager::nextLabel() {
    
    currentSceneIndex++;
    if(currentSceneIndex >= scenes.size() - 1) {
        currentSceneIndex = -1;
    }
    
}
