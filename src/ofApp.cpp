#include "ofApp.h"

bool shouldRemoveRigidBody( const shared_ptr<ofxBulletRigidBody>& ab ) {
    return ab->getPosition().y > 15;
}

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(ofColor(34, 107, 126));
    
    TIME_SAMPLE_SET_FRAMERATE(60.0f); //specify a target framerate
    ofSetVerticalSync(false);
    ofSetFrameRate(0);
    
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
    ofVec3f scale(0.4, 0.4, 0.4);
    
    // model - sakura
    sakuraModel.loadModel("sakura/sakura.3ds", true);
    sakuraModel.setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);
    sakuraModel.setScale(scale.x, scale.y, scale.z);
    
    // camera
    camera.setDistance(14);
    camera.setPosition(ofVec3f(0, -30.f, -50.f));
    camera.lookAt(ofVec3f(0, 0, 0), ofVec3f(0, -1, 0));
    camera.enableMouseInput();
    
    // light
    light.setPosition( 0, -10, 0 );
    
    // physics
    world.setup();
    world.enableGrabbing();
    world.setCamera(&camera);
    world.setGravity( ofVec3f(0, 25., 0) );
    
    // pole
    float poleCylinderMass = 0; // set as static
    float poleCylinderRadius = 0.3;
    float poleCylinderHeight = 10;
    
    for (int i = 0; i < 4; i++) {
        shared_ptr<ofxBulletCylinder> cylinder(new ofxBulletCylinder);
        switch (i) {
            case 0:
                // - front left
                cylinder->create(world.world, ofVec3f(-20, -10, 20), poleCylinderMass, poleCylinderRadius, poleCylinderHeight);
                break;
            case 1:
                // - front right
                cylinder->create(world.world, ofVec3f(20, -10, 20), poleCylinderMass, poleCylinderRadius, poleCylinderHeight);
                break;
            case 2:
                // - rear left
                cylinder->create(world.world, ofVec3f(-20, -10, -20), poleCylinderMass, poleCylinderRadius, poleCylinderHeight);
                break;
            case 3:
                // - rear right
                cylinder->create(world.world, ofVec3f(20, -10, -20), poleCylinderMass, poleCylinderRadius, poleCylinderHeight);
                break;
            default:
                break;
        }

        cylinder->add();
        poleCylinders.push_back(cylinder);
    }
    
    // ground
    groundBox = new ofxBulletBox();
    groundBox->create( world.world, ofVec3f(0., 5.5, 0.), 0., 50., 1.f, 50.f );
    groundBox->setProperties(.25, .95);
    groundBox->add();
    
    const btScalar	s = 18;
    const btScalar	h = -2*poleCylinderHeight;
    const int		r = 64;
    
    // patches
    groundPatch = new ofxBulletPatch();
    groundPatch->create( &world, ofVec3f(-s,h,-s), ofVec3f(s,h,-s), ofVec3f(-s, h, s ), ofVec3f(s,h,s), r, r );
    groundPatch->add();
//    groundPatch->getSoftBody()->getCollisionShape()->setMargin(0.25);
//    groundPatch->getSoftBody()->generateBendingConstraints(1, groundPatch->getSoftBody()->m_materials[0]);
//    groundPatch->getSoftBody()->m_materials[0]->m_kLST		=	0.4;
    
    groundPatch->setMass(0.25, false);
    groundPatch->getSoftBody()->m_cfg.piterations = 20;
    groundPatch->getSoftBody()->m_cfg.citerations = 20;
    groundPatch->getSoftBody()->m_cfg.diterations = 20;
    
    groundPatch->attachRigidBodyAt(r*(r-1), poleCylinders[0]->getRigidBody());
    groundPatch->attachRigidBodyAt(r*r-1, poleCylinders[1]->getRigidBody());
    groundPatch->attachRigidBodyAt(r*0, poleCylinders[2]->getRigidBody());
    groundPatch->attachRigidBodyAt(r*1-1, poleCylinders[3]->getRigidBody());

    
    // sakura
    ofQuaternion startRot = ofQuaternion(1., 0., 0., PI);
    sakuraBulletShapes.resize(10);
    for (int i = 0; i < sakuraBulletShapes.size(); i++) {
        sakuraBulletShapes[i] = new ofxBulletCustomShape;
        if (i == 0) {
            for (int i = 0; i < sakuraModel.getNumMeshes(); i++) {
                sakuraBulletShapes[i]->addMesh(sakuraModel.getMesh(i), scale, true);
            }
        } else {
            sakuraBulletShapes[i]->init((btCompoundShape*)sakuraBulletShapes[0]->getCollisionShape(), sakuraBulletShapes[0]->getCentroid());
        }
        ofVec3f startLoc = ofVec3f( ofRandom(-5, 5), ofRandom(0, -10), ofRandom(-5, 5) );
        sakuraBulletShapes[i]->create(world.world, startLoc, startRot, 3.);
        sakuraBulletShapes[i]->add();
    }

    // debug
    enableDebugDraw = false;
    world.enableDebugDraw();
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
    // dmx
    TS_START("update dmx");{
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
    }TS_STOP("update dmx");
    
    // 3d model
    TS_START("update sakura");{
        sakuraModel.update();
    }TS_STOP("update sakura");
    // physics
    TS_START("update world");{
        world.update();
    }TS_STOP("update world");
    
    ofRemove( spheres, shouldRemoveRigidBody );
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    world.drawDebug();
    // 3d model
    ofEnableDepthTest();{
        camera.begin();{
            ofSetLineWidth(1.f);
            if(enableDebugDraw) world.drawDebug();
            
            ofEnableLighting();{
                light.enable();{
                    // ground
                    TS_START("ground");{
                        ofSetColor(34, 107, 126);
                        groundBox->draw();
                        ofSetColor(255);
                        groundPatch->draw();
                    }TS_STOP("ground");
                    
                    // pole
                    TS_START("pole");{
                        ofSetColor(ofColor::yellow);
                        for (int i = 0; i < poleCylinders.size(); i++) {
                            poleCylinders[i]->draw();
                        }
                        ofSetColor(255);
                    }TS_STOP("pole");
                    
                    // spheres
                    TS_START("sphere");{
                        ofSetHexColor( 0xC4EF02 );
                        for( int i = 0; i < spheres.size(); i++ ) {
                            spheres[i]->draw();
                        }
                        ofSetColor(255);
                    }TS_STOP("sphere");
                    
                    // draw sakura
                    TS_START("sakura");{
                        ofxAssimpMeshHelper & meshHelper = sakuraModel.getMeshHelper(0);
                        ofMaterial & material = meshHelper.material;
                        ofPoint scale = sakuraModel.getScale();
                        meshHelper.getTextureRef().bind();{
                            material.begin();{
                                for (int i = 0; i < sakuraBulletShapes.size(); i++) {
                                    sakuraBulletShapes[i]->transformGL();{
                                        ofScale(scale.x, scale.y, scale.z);
                                        sakuraModel.getCurrentAnimatedMesh(0).drawFaces();
                                    } sakuraBulletShapes[i]->restoreTransformGL();
                                }
                            }material.end();
                        }meshHelper.getTextureRef().unbind();
                    }TS_STOP("sakura");
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
        case 'd':
            // draw debug or disable to draw debug
            enableDebugDraw = !enableDebugDraw;
            break;
        case 'f':
            // full screen or disable full screnn
            ofToggleFullscreen();
            break;
        case 'h':
            // show or hide gui
            showGui = !showGui;
            break;
        case 'l':
            // load gui setting data
            gui.loadFromFile("settings.xml");
            break;
        case 'm':{
            // add sakura model
            ofxBulletCustomShape *sakuraShape;
            sakuraShape = new ofxBulletCustomShape;
            ofQuaternion startRot = ofQuaternion(1., 0., 0., PI);
            sakuraShape->init((btCompoundShape*)sakuraBulletShapes[0]->getCollisionShape(), sakuraBulletShapes[0]->getCentroid());
            sakuraShape->create(world.world, camera.getPosition(), startRot, 0.000005);
            sakuraShape->add();
            sakuraBulletShapes.push_back(sakuraShape);
        }
            break;
        case 's':
            // save gui setting
            gui.saveToFile("settings.xml");
            break;
        case ' ':{
            // add one sphere
            shared_ptr< ofxBulletSphere > ss( new ofxBulletSphere() );
            ss->create( world.world, camera.getPosition(), 0.000005, 1.0);
            ss->add();
            
            ofVec3f frc = -camera.getPosition();
            frc.normalize();
            ss->applyCentralForce( frc * 0.001 );
            
            spheres.push_back( ss );
        }
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
