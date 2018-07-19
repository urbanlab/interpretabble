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
    
    if(currentSceneIndex < 0 || currentSceneIndex >= scenes.size()) {
        
    } else {
        
        // draw
        scenes[currentSceneIndex]->update();
        
    }
    
}

void SceneManager::draw() {
    
    // idle mode
    if(currentSceneIndex < 0 || currentSceneIndex >= scenes.size()) {
        
    } else {
        
        // draw
        scenes[currentSceneIndex]->draw();
        ofLogNotice("Drawing ") << scenes[currentSceneIndex]->label;

    }
    
}

void SceneManager::loadAssets() {
    
    // go through each folder and load assets
    string path = "scenes";
    ofDirectory dir(path);
    dir.listDir();
    
    ofLogNotice("Loading assets");
    
    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        
        if(dir.getFile(i).isDirectory()) {
            
            ofDirectory subdir(dir.getPath(i));
            subdir.listDir();
            
            Scene * scene = new Scene();
            scene->label = dir.getFile(i).getBaseName();
            
            ofLogNotice("Parsing ") << scene->label;

            for(int j = 0; j < subdir.size(); j++){
                
                ofFile file = subdir.getFile(j);
                
                
                if ( file.getBaseName() == "tools" ) {
                    
                    scene->toolsImage.load(file.getAbsolutePath());
                    
                } else {
                    
                    string ext = file.getExtension();
                    Asset asset;
                    asset.addAsset(file.getAbsolutePath(), file.getExtension());
                    scene->assets.push_back(asset);

                }
                
                ofLogNotice("Adding ") << file.getBaseName();
                
                
            }
            scenes.push_back(scene);
            
        }
    }
    
    ofLogNotice("Parsing done with ") << scenes.size() << " scenes";

    
}

void SceneManager::setCurrentLabel(string label) {
    
    // need to put a timer on this
    
    ofLogNotice("set current label: ") << label;
    
    bool bhasFound = false;
    for(int i=0; i<scenes.size(); i++) {
        if(scenes[i]->label == label ) {
            
            if(currentSceneIndex != i) {
                currentSceneIndex = i;
                onSceneChanged();
                bhasFound = true;
                return;
            }
        }
    }
    
    if(label == "rien") {
        currentSceneIndex = -1;
        ofLogNotice("Nothing");
    }
    
}

void SceneManager::onSceneChanged() {
    
    ofLogNotice("Changing scene to") << currentSceneIndex << " with " << scenes.size() << " scenes";

    
    if(currentSceneIndex < 0 || currentSceneIndex >= scenes.size()) {
    } else {
        scenes[currentSceneIndex]->onStart();
        
    }
    
}

void SceneManager::onSceneOutHandler() {
    

}

void SceneManager::nextLabel() {
    
    currentSceneIndex++;
    if(currentSceneIndex >= scenes.size()) {
        currentSceneIndex = -1;
    }
    
    onSceneChanged();
    
}
