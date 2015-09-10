#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(ofColor(34, 107, 126));
    
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
    // model - sakura
    sakuraModel.loadModel("sakura/sakura.3ds", true);
    sakuraModel.setRotation(0, 90, 1, 0, 0);
    sakuraModel.setRotation(1, 270, 0, 0, 1);
    sakuraModel.setScale(0.9, 0.9, 0.9);
    sakuraModel.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    ofVec3f scale(.0009, .0009, .0009);
    
    // model - of
//    sakuraModel.loadModel("of/OFlogo.dae", 20);
//    sakuraModel.setRotation(0, 90, 1, 0, 0);
//    sakuraModel.setRotation(1, 270, 0, 0, 1);
//    sakuraModel.setScale(.0009, .0009, .0009);
//    sakuraModel.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
//    ofVec3f scale(.0009, .0009, .0009);
    
    // camera
    camera.setDistance( 14 );
    camera.setPosition(ofVec3f(0, -3.f, -40.f));
    camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));
    camera.enableMouseInput();
    
    // light
    light.setPosition( 0, -10, 0 );
    
    // physics
    world.setup();
    world.enableGrabbing();
    world.setCamera(&camera);
    world.setGravity( ofVec3f(0, 25., 0) );
    
    ground.create( world.world, ofVec3f(0., 5.5, 0.), 0., 50., 1.f, 50.f );
    ground.setProperties(.25, .95);
    ground.add();
    
    ofQuaternion startRot = ofQuaternion(1., 0., 0., PI);
    sakuraBulletShapes.resize(3);
    for (int i = 0; i < sakuraBulletShapes.size(); i++) {
        sakuraBulletShapes[i] = new ofxBulletCustomShape;
        if (i == 0) {
            for (int j = 0; j < sakuraModel.getNumMeshes(); j++) {
                sakuraBulletShapes[j]->addMesh(sakuraModel.getMesh(i), scale, true);
            }
        } else {
            sakuraBulletShapes[i]->init((btCompoundShape*)sakuraBulletShapes[0]->getCollisionShape(), sakuraBulletShapes[0]->getCentroid());
        }
        ofVec3f startLoc = ofVec3f( ofRandom(-5, 5), ofRandom(0, -10), ofRandom(-5, 5) );
        sakuraBulletShapes[i]->create(world.world, startLoc, startRot);
        sakuraBulletShapes[i]->add();
    }
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
    
    // 3d model
    sakuraModel.update();
    sakuraModel.setRotation(1, 270 + ofGetElapsedTimef() * 60, 0, 0, 1);
    
    // physics
    world.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    // 3d model
    ofEnableDepthTest();{
        camera.begin();{
            ofEnableLighting();{
                light.enable();{
                    // ground
                    ofSetColor(34, 107, 126);
                    ground.draw();
                    
                    //fake back wall
                    ofSetColor(20, 20, 20);
                    glBegin(GL_QUADS);{
                        glVertex3f(0.0, ofGetHeight(), -600);
                        glVertex3f(ofGetWidth(), ofGetHeight(), -600);
                        glVertex3f(ofGetWidth(), 0, -600);
                        glVertex3f(0, 0, -600);
                    }glEnd();
                    
                    //fake wall
                    ofSetColor(50, 50, 50);
                    glBegin(GL_QUADS);{
                        glVertex3f(0.0, ofGetHeight(), 0);
                        glVertex3f(ofGetWidth(), ofGetHeight(), 0);
                        glVertex3f(ofGetWidth(), ofGetHeight(), -600);
                        glVertex3f(0, ofGetHeight(), -600);
                    } glEnd();
                    
                    //lets tumble the world with the mouse
                    glPushMatrix();{
                        //draw in middle of the screen
                        glTranslatef(ofGetWidth()/2,ofGetHeight()/2,0);
                        //tumble according to mouse
                        glRotatef(-mouseY,1,0,0);
                        glRotatef(mouseX,0,1,0);
                        glTranslatef(-ofGetWidth()/2,-ofGetHeight()/2,0);
                        ofSetColor(255, 255, 255, 255);
                        sakuraModel.drawFaces();
                    } glPopMatrix();
                    
                    // draw sakura
                    ofxAssimpMeshHelper & meshHelper = sakuraModel.getMeshHelper(0);
                    ofMaterial & material = meshHelper.material;
                    
                    ofPoint scale = sakuraModel.getScale();
                    meshHelper.getTextureRef().bind();
                    material.begin();
                    for (int i = 0; i < sakuraBulletShapes.size(); i++) {
                        sakuraBulletShapes[i]->transformGL();{
                            ofScale(scale.x, scale.y, scale.z);
                            sakuraModel.getMesh(0).drawFaces();
                        } sakuraBulletShapes[i]->restoreTransformGL();
                    }
                    material.end();
                    meshHelper.getTextureRef().unbind();
                }light.disable();
            }ofDisableLighting();
        } camera.end();
    } ofDisableDepthTest();

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
