//
//  Scene.cpp
//  Translations
//
//  Created by Martial Geoffre-Rouland on 29/03/2018.
//

#include "Scene.hpp"

void Scene::update() {
    
    for (int i=0; i<assets.size(); i++) {
       
        assets[i].update();
       
        
    }
    
}
void Scene::draw() {
    
    ofEnableAlphaBlending();
    
    ofPushMatrix();
    float yPos = 88.0;
    for (int i=0; i<assets.size(); i++) {
        
        ofPushMatrix();
        ofTranslate(1434, yPos);
        assets[i].draw();
        yPos +=  assets[i].getHeight();
        ofPopMatrix();
        
    }
    ofPopMatrix();
    
    ofEnableAlphaBlending();
    toolsImage.draw(1118, 270);
    
    ofDisableAlphaBlending();
    
}

void Scene::onStart() {
    
    for (int i=0; i<assets.size(); i++) {
        
        assets[i].opacity.reset(0.0);
        assets[i].opacity.setCurve(EASE_IN);
        assets[i].opacity.setDuration(0.55 + i * 0.1);
        assets[i].opacity.animateTo(255.0);
        assets[i].play();
        
    }
    
    
}

void Scene::onEnd() {
    
}
