#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    showGui = true;
    allChannelsOnButton.addListener(this, &ofApp::allChannelsOnButtonPressed);
    allChannelsOffButton.addListener(this, &ofApp::allChannelsOffButtonPressed);
    gui.setup();
    gui.add(enableChannel1.set("Enable channel 1", false));
    gui.add(channel1.set("Channel 1", 127, 0, 255));
    gui.add(enableChannel2.set("Enable channel 2", false));
    gui.add(channel2.set("Channel 2", 127, 0, 255));
    gui.add(enableChannel3.set("Enable channel 3", false));
    gui.add(channel3.set("Channel 3", 127, 0, 255));
    gui.add(allChannelsOnButton.setup("All channels ON"));
    gui.add(allChannelsOffButton.setup("All channels OFF"));
}

void ofApp::allChannelsOnButtonPressed(){
    enableChannel1.set(true);
    enableChannel2.set(true);
    enableChannel3.set(true);
}

void ofApp::allChannelsOffButtonPressed(){
    enableChannel1.set(false);
    enableChannel2.set(false);
    enableChannel3.set(false);
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (showGui) gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 'h':
            showGui = !showGui;
            break;
        case 'l':
            gui.loadFromFile("settings.xml");
            break;
        case 's':
            gui.saveToFile("settings.xml");
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
