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
    
    accueil.setup();
    accueil.loadFolder("accueil");
    accueil.setFps(30);
    accueil.setLoop(true);
    accueil.play();
    
    bHasChanged = false;
    tempIndex = currentSceneIndex;
    nFramesIndexChangedDelay = 0;
    nFramesDelaymax = 500;
}

void SceneManager::update() {
    
    if (tempIndex != currentSceneIndex ) {
        
        // man, we have potentially changed.
        // let's wait for a bit
        nFramesIndexChangedDelay++;
        
        ofLogNotice("waiting... ") << nFramesIndexChangedDelay;
        // man. Times up.
        
        if(nFramesIndexChangedDelay > nFramesDelaymax) {
            
            nFramesIndexChangedDelay = 0;
            currentSceneIndex = tempIndex;
            onSceneChanged();
            
        }
        
        
    }
    
    if(currentSceneIndex < 0 || currentSceneIndex >= scenes.size()) {
        
        accueil.update();
        
    } else {
        
        // draw
        scenes[currentSceneIndex]->update();
        
    }
    
}

void SceneManager::draw() {
    
    // idle mode
    if(currentSceneIndex < 0 || currentSceneIndex >= scenes.size()) {
        
        ofSetColor(255,255);
        accueil.draw(0.0,0.0, 1920, 1080);

        
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
                    
                    Asset asset;
                    if(file.isDirectory()) {
                        
                        ofLogNotice("check folder") << subdir.getPath(j);
                        asset.addAsset(subdir.getPath(j), "folder");

                    } else {
                    
                    string ext = file.getExtension();
                    
                    asset.addAsset(file.getAbsolutePath(), file.getExtension());
                        
                    }
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
                tempIndex = i;
                return;
            }
        }
    }
    
    if(label == "rien") {
        tempIndex = -1;
        //ofLogNotice("Nothing");
    }
    
}

void SceneManager::onSceneChanged() {
    
    ofLogNotice("Changing scene to") << currentSceneIndex << " with " << scenes.size() << " scenes";

    
    if(currentSceneIndex < 0 || currentSceneIndex >= scenes.size()) {
    } else {
        scenes[currentSceneIndex]->onStart();
        
    }
    
    tempIndex = currentSceneIndex;
    
}

void SceneManager::onSceneOutHandler() {
    

}

void SceneManager::setCurrentLabel(int id) {
    
    currentSceneIndex = id - 1;
    onSceneChanged();
    
}


void SceneManager::nextLabel() {
    
    currentSceneIndex++;
    if(currentSceneIndex >= scenes.size()) {
        currentSceneIndex = -1;
    }
    
    onSceneChanged();
    
}
