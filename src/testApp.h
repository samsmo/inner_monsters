#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"
#include "ofMain.h"

class testApp : public ofBaseApp{

public:
    
	void setup();
	void update();
	void draw();
    void exit();
    
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	ofxOpenNI openNIDevice;
    
    ofTrueTypeFont verdana;
    
    ofPoint _HEAD;
    ofPoint _ARM;
    
    ofxAssimpModelLoader model;
    ofxAssimpModelLoader arm;
    
    ofVboMesh mesh;
    ofPoint position;
    float normScale;
    ofPoint scale;
    ofPoint sceneCenter;
    ofMaterial material;
    ofTexture tex;
    ofLight	light;
    float rotationTest;
    
    ofCamera					camera;
	float						camLat, camLon, camDist;
	ofVec3f						camTarget;
    
};

#endif
