//
//  Scene.cpp
//  Translations
//
//  Created by Martial Geoffre-Rouland on 29/03/2018.
//

#include "Scene.hpp"

void Scene::update() {
    
    
}
void Scene::draw() {
    
    ofEnableAlphaBlending();
    
    ofPushMatrix();
    float yPos = 0.0;
    for (int i=0; i<assets.size(); i++) {
        
        ofPushMatrix();
        ofTranslate(0.0, yPos);
        assets[i].draw();
        yPos +=  assets[i].getHeight();
        ofPopMatrix();
        
    }
    ofPopMatrix();
    ofDisableAlphaBlending();
    
}

void Scene::onStart() {
    
}

void Scene::onEnd() {
    
}
