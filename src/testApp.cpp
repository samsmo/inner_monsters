#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    ofSetFrameRate(60);
    ofSetSmoothLighting(true);
	light.setAmbientColor(ofColor(.0, .0, .0));
	light.setDiffuseColor(ofColor(.0, .0, .0));
	light.setSpecularColor(ofColor(255, .1, .1));
    ofDisableArbTex();
    ofSetLogLevel(OF_LOG_NOTICE);
    
    if(model.loadModel("testmonster.dae",true) && arm.loadModel("leg.dae",true)){
        model.calculateDimensions();
    }
    
    ofEnableSmoothing();
    openNIDevice.setup();
    //openNIDevice.setLogLevel(OF_LOG_VERBOSE);
    openNIDevice.addDepthGenerator();
    openNIDevice.addImageGenerator();
    openNIDevice.addUserGenerator();
    
    openNIDevice.setRegister(true); // register depth to image
    openNIDevice.setMirror(true);
    
    openNIDevice.setMaxNumUsers(1); // defualt is 4
    openNIDevice.setUseMaskTextureAllUsers(true);
    openNIDevice.setUsePointCloudsAllUsers(true);
    openNIDevice.setPointCloudDrawSizeAllUsers(3); // sets the size the points are drawn on screen (in pixels)
    openNIDevice.setPointCloudResolutionAllUsers(2); // sets the step between pixels and point cloud eg., 1 is every pixel, 2 is every second pixel etc

	openNIDevice.start();

}

//--------------------------------------------------------------
void testApp::update(){
    ofBackground(0, 0, 0);
    openNIDevice.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    ofEnableLighting();
	light.enable();
    light.setPosition(0, 0, 150);
    
    ofBackground(50, 50, 50, 0);
    ofSetColor(255, 255, 255, 255);
    openNIDevice.drawDepth(0, 0, 0, 0);
    openNIDevice.drawSkeletons(0,0,640*2,540*2);

    int numUsers = openNIDevice.getNumTrackedUsers();
    if(numUsers > 0){
        ofxOpenNIUser & user = openNIDevice.getTrackedUser(0);
        
            _HEAD = user.getJoint(JOINT_HEAD).getProjectivePosition();
            _ARM = user.getJoint(JOINT_LEFT_FOOT).getProjectivePosition();
        
        //tranlation code for different limbs / models
        glPushMatrix();
            glTranslatef(_HEAD.x, _HEAD.y, -_HEAD.z);
            ofRotateY(90);
            model.drawFaces();
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(_ARM.x,_ARM.y, -_ARM.z);
            ofRotateY(90);
            arm.drawFaces();
        glPopMatrix();
           
    
    } 

    light.disable();
	glDisable(GL_DEPTH_TEST);
	ofDisableLighting();
}

//--------------------------------------------------------------
void testApp::exit(){
    // this often does not work -> it's a known bug -> but calling it on a key press or 
    // anywhere that isnt std::aexit() works...press 'x' to shutdown cleanly...
    openNIDevice.stop();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case 'x':
            openNIDevice.stop();
            break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

