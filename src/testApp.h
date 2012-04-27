#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofxAssimpModelLoader.h"
#include "ofVboMesh.h"
#include "ofMain.h"

class SkeletonDrawer : public ofxAssimpModelLoader  
{  
public:  
	void drawSkeleton();  
	void rotateBone(aiBone *bone, float x, float y, float z);
	void translateBone(aiBone *bone, float x, float y, float z);
	void updateMeshes();
	int cualBone;
	string *trafoMsg;
	void randomPose();
private:  
	std::map<aiNode*, aiMatrix4x4> nodeMatrices;  
	void collectNodes(const struct aiScene *sc, const struct aiNode *nd);  
	
}; 

class testApp : public ofBaseApp{
private:
    void listBoneNames();
    void buildBoneNameList();
    
public:
    
	void setup();
	void update();
	void draw();
    void exit();
    const vector<string>& getBoneNames();
    const aiScene* getAiScene(ofxAssimpModelLoader model);
    aiBone* getBoneNamed(aiMesh *mesh, string boneName);
    
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
    
    SkeletonDrawer model;
    //ofxAssimpModelLoader arm;
    
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
    
    /*
     *  List of bone names  string( mBones[ j ]->mName.data )
     */
    vector<string>                              mBoneNames;
    vector<string> something;
    /*
     *  Bool to check if bone name lists are ready or not.
     */
    bool                                        mBoneListCreated;
    
    bool mSetup;
    
    int pMouseY, pMouseX, pMouseZ;
    int pHeadY, pHeadX, pHeadZ;
    int pArmY, pArmX, pArmZ;
    
};

#endif
