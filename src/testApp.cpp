#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    ofDisableArbTex();
    ofSetFrameRate(60);
    ofSetSmoothLighting(true);
	light.setAmbientColor(ofColor(.0, .0, .0));
	light.setDiffuseColor(ofColor(.0, .0, .0));
	light.setSpecularColor(ofColor(255, .1, .1));
    ofSetLogLevel(OF_LOG_NOTICE);
    
    if(model.loadModel("astroBoy_walk.dae",true)){
        model.calculateDimensions();
        material = model.getMaterialForMesh(0);
        mSetup = true;
        model.setPosition(ofGetWidth() / 2, ofGetHeight() * 0.75, 0);  
    }
    
    ofEnableSmoothing();
    glEnable(GL_DEPTH_TEST);  
	glShadeModel(GL_SMOOTH);
    ofEnableSeparateSpecularLight(); 
    pHeadY = 0;
    pHeadX = 0;
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
    
    int numUsers = openNIDevice.getNumTrackedUsers();
    if(numUsers > 0){
        ofxOpenNIUser & user = openNIDevice.getTrackedUser(0);
        
        _HEAD = user.getJoint(JOINT_HEAD).getProjectivePosition();
        _ARM = user.getJoint(JOINT_LEFT_FOOT).getProjectivePosition();
        
        model.translateBone(model.getAssimpScene()->mMeshes[0]->mBones[2], 0.0, 5*(pHeadY- int(_HEAD.y))/float(ofGetHeight()), -5*(pHeadX-int(_HEAD.x))/float(ofGetWidth()));
        
        pMouseX = mouseX;
        pMouseY = mouseY;
        
        pHeadY = _HEAD.y;
        pHeadX = _HEAD.x;
        //pMouseZ = mouseZ;
    }
		//	model.rotateBone(model.getAssimpScene()->mMeshes[cualMesh]->mBones[cualBone], 0.0, 5*(pMouseY- mouseY)/float(ofGetHeight()), 5*(pMouseX-mouseX)/float(ofGetWidth()));
		
	model.updateMeshes();
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

   /* int numUsers = openNIDevice.getNumTrackedUsers();
    if(numUsers > 0){
        ofxOpenNIUser & user = openNIDevice.getTrackedUser(0);
        
            _HEAD = user.getJoint(JOINT_HEAD).getProjectivePosition();
            _ARM = user.getJoint(JOINT_LEFT_FOOT).getProjectivePosition();
        
      */  
        //tranlation code for different limbs / models
       /* glPushMatrix();
            glTranslatef(_HEAD.x, _HEAD.y, -_HEAD.z);
            ofRotateY(90);
            material.begin();
                model.drawFaces();
            material.end();
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(_ARM.x,_ARM.y, -_ARM.z);
            ofRotateY(90);
           // arm.drawFaces();
        glPopMatrix();
           */
    
    //} 
    model.drawFaces();
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
        case 'y':
            cout << 5*(pMouseY- mouseY)/float(ofGetHeight())<<"mouse Y"<<endl;
            cout << 5*(pHeadY - _HEAD.y)/float(ofGetHeight())<<"head Y"<<endl;
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

void SkeletonDrawer::translateBone(aiBone *bone, float x, float y, float z){
	
	aiNode *node = scene->mRootNode->FindNode(bone->mName);
	
	aiMatrix4x4 mat;
	aiMatrix4x4::Translation(aiVector3D(x,y,z), mat);
	node->mTransformation*=mat;
	
}
void SkeletonDrawer::rotateBone(aiBone *bone, float x, float y, float z){
	
	aiNode *node = scene->mRootNode->FindNode(bone->mName);
	
	aiMatrix4x4 mat;
	aiMatrix4x4::RotationX(x, mat);
	node->mTransformation*=mat;
	
	aiMatrix4x4::RotationY(y, mat);
	node->mTransformation*=mat;
	
	aiMatrix4x4::RotationZ(z, mat);
	node->mTransformation*=mat;
} 

//The following is just a copy of the second part of ofxAssimpModelLoader::updateAnimation method.
//I just copyed it just for not modifiying the assimp addon and to lazy to extend the class.
void SkeletonDrawer::updateMeshes(){  
    // update mesh position for the animation
    for (unsigned int i = 0; i < modelMeshes.size(); ++i){
        
        // current mesh we are introspecting
        const aiMesh* mesh = modelMeshes[i].mesh;
        
        // calculate bone matrices
        std::vector<aiMatrix4x4> boneMatrices( mesh->mNumBones);
        for( size_t a = 0; a < mesh->mNumBones; ++a)
        {
            const aiBone* bone = mesh->mBones[a];
            
            // find the corresponding node by again looking recursively through the node hierarchy for the same name
            aiNode* node = scene->mRootNode->FindNode(bone->mName);
            
            // start with the mesh-to-bone matrix
            boneMatrices[a] = bone->mOffsetMatrix;
            // and now append all node transformations down the parent chain until we're back at mesh coordinates again
            const aiNode* tempNode = node;
            while( tempNode)
            {
                // check your matrix multiplication order here!!!
                boneMatrices[a] = tempNode->mTransformation * boneMatrices[a];
                // boneMatrices[a] = boneMatrices[a] * tempNode->mTransformation;
                tempNode = tempNode->mParent;
            }
            modelMeshes[i].hasChanged = true;
            modelMeshes[i].validCache = false;
        }
        
        modelMeshes[i].animatedPos.assign(modelMeshes[i].animatedPos.size(),0);
        if(mesh->HasNormals()){
            modelMeshes[i].animatedNorm.assign(modelMeshes[i].animatedNorm.size(),0);
        }
        // loop through all vertex weights of all bones
        for( size_t a = 0; a < mesh->mNumBones; ++a)
        {
            const aiBone* bone = mesh->mBones[a];
            const aiMatrix4x4& posTrafo = boneMatrices[a];
            
            
            for( size_t b = 0; b < bone->mNumWeights; ++b)
            {
                const aiVertexWeight& weight = bone->mWeights[b];
                
                size_t vertexId = weight.mVertexId;
                const aiVector3D& srcPos = mesh->mVertices[vertexId];
                
                modelMeshes[i].animatedPos[vertexId] += weight.mWeight * (posTrafo * srcPos);
            }
            if(mesh->HasNormals()){
                // 3x3 matrix, contains the bone matrix without the translation, only with rotation and possibly scaling
                aiMatrix3x3 normTrafo = aiMatrix3x3( posTrafo);
                for( size_t b = 0; b < bone->mNumWeights; ++b)
                {
                    const aiVertexWeight& weight = bone->mWeights[b];
                    size_t vertexId = weight.mVertexId;
                    
                    const aiVector3D& srcNorm = mesh->mNormals[vertexId];
                    modelMeshes[i].animatedNorm[vertexId] += weight.mWeight * (normTrafo * srcNorm);
                    
                }
            }
        }
    }
}
