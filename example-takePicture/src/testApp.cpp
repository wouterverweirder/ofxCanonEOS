#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofAddListener(wrapper.onFileAdded, this, &testApp::onFileAdded);
    wrapper.start();
}

void testApp::onFileAdded(string &fileName){
    lastImage.loadImage(fileName);
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
    if(lastImage.isAllocated()){
        lastImage.draw(0, 0);
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if(' ' == key){
        if(!wrapper.isStarted()){
            wrapper.start(); //restart if camera was disconnected
        }
        wrapper.takePicture();
    }
}