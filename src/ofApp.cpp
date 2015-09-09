#include "ofApp.h"

GLfloat lightOnePosition[] = {40.0, 40, 100.0, 0.0};
GLfloat lightOneColor[] = {0.99, 0.99, 0.99, 1.0};

GLfloat lightTwoPosition[] = {-40.0, 40, 100.0, 0.0};
GLfloat lightTwoColor[] = {0.99, 0.99, 0.99, 1.0};

//--------------------------------------------------------------
void ofApp::setup(){
    // gui
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
    gui.add(enableChannel4.set("Enable channel 4", false));
    gui.add(channel4.set("Channel 4", 127, 0, 255));
    gui.add(allChannelsOnButton.setup("All channels ON"));
    gui.add(allChannelsOffButton.setup("All channels OFF"));
    
    // dmx
    dmx.connect("tty.usbserial-ENY46L4I"); // use the name
    //dmx.connect(0); // or use a number
    
    // 3d model
    //load the squirrel model - the 3ds and the texture file need to be in the same folder
    sakuraModel.loadModel("sakura/sakura.3ds", 20);
    
    //you can create as many rotations as you want
    //choose which axis you want it to effect
    //you can update these rotations later on
    sakuraModel.setRotation(0, 90, 1, 0, 0);
    sakuraModel.setRotation(1, 270, 0, 0, 1);
    sakuraModel.setScale(0.9, 0.9, 0.9);
    sakuraModel.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);

    // light
    glShadeModel (GL_SMOOTH);
    
    /* initialize lighting */
    glLightfv (GL_LIGHT0, GL_POSITION, lightOnePosition);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, lightOneColor);
    glEnable (GL_LIGHT0);
    glLightfv (GL_LIGHT1, GL_POSITION, lightTwoPosition);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, lightTwoColor);
    glEnable (GL_LIGHT1);
    glEnable (GL_LIGHTING);
    glColorMaterial (GL_FRONT_AND_BACK, GL_DIFFUSE);
    glEnable (GL_COLOR_MATERIAL);
}

void ofApp::allChannelsOnButtonPressed(){
    enableChannel1.set(true);
    enableChannel2.set(true);
    enableChannel3.set(true);
    enableChannel4.set(true);
}

void ofApp::allChannelsOffButtonPressed(){
    enableChannel1.set(false);
    enableChannel2.set(false);
    enableChannel3.set(false);
    enableChannel4.set(false);
}

//--------------------------------------------------------------
void ofApp::update(){
    if (enableChannel1.get()) {
        dmx.setLevel(1, channel1);
    }else{
        dmx.setLevel(1, 0);
    }
    if (enableChannel2.get()) {
        dmx.setLevel(2, channel2);
    }else{
        dmx.setLevel(2, 0);
    }
    if (enableChannel3.get()) {
        dmx.setLevel(3, channel3);
    }else{
        dmx.setLevel(3, 0);
    }
    if (enableChannel4.get()) {
        dmx.setLevel(4, channel4);
    }else{
        dmx.setLevel(4, 0);
    }
    dmx.update();
    
    //3d model
    sakuraModel.setRotation(1, 270 + ofGetElapsedTimef() * 60, 0, 0, 1);
}

//--------------------------------------------------------------
void ofApp::draw(){
    // 3d model
    ofEnableDepthTest();
    
    //fake back wall
    ofSetColor(20, 20, 20);
    glBegin(GL_QUADS);
    glVertex3f(0.0, ofGetHeight(), -600);
    glVertex3f(ofGetWidth(), ofGetHeight(), -600);
    glVertex3f(ofGetWidth(), 0, -600);
    glVertex3f(0, 0, -600);
    glEnd();
    
    //fake wall
    ofSetColor(50, 50, 50);
    glBegin(GL_QUADS);
    glVertex3f(0.0, ofGetHeight(), 0);
    glVertex3f(ofGetWidth(), ofGetHeight(), 0);
    glVertex3f(ofGetWidth(), ofGetHeight(), -600);
    glVertex3f(0, ofGetHeight(), -600);
    glEnd();
    
    //lets tumble the world with the mouse
    glPushMatrix();
    
    //draw in middle of the screen
    glTranslatef(ofGetWidth()/2,ofGetHeight()/2,0);
    //tumble according to mouse
    glRotatef(-mouseY,1,0,0);
    glRotatef(mouseX,0,1,0);
    glTranslatef(-ofGetWidth()/2,-ofGetHeight()/2,0);
    
    ofSetColor(255, 255, 255, 255);
    sakuraModel.draw();
    
    glPopMatrix();
    ofDisableDepthTest();
    ofSetHexColor(0x000000);

    // gui
    if (showGui) gui.draw();

    // debug
    ofSetWindowTitle(ofToString(ofGetFrameRate(), 0));
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
