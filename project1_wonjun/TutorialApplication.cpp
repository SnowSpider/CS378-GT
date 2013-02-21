/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#include "TutorialApplication.h"

#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

#include <iostream>
#include <math.h>

TutorialApplication::TutorialApplication(void)
{
    deltaX = 0;
    deltaY = 0;
    xi = 0;
    yi = 0;
    exitCmd = false; 
    ready = true;
}
 
TutorialApplication::~TutorialApplication(void)
{
}
 
//-------------------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
    mCamera->setPosition(0,0,400); 
    
    // Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    
    // Create an Entity
    mBall = mSceneMgr->createEntity("mBall", "sphere.mesh");
    mBall->setMaterialName("Examples/Chrome");
    mBall->setCastShadows(true);
    
    // Create a SceneNode and attach the Entity to it
    Ogre::SceneNode* mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("mBall");
    mNode->attachObject(mBall);
    mNode->scale( 0.1, 0.1, 0.1 );
    
    
    //GameObject myBall(mSceneMgr, Ogre::Vector3(0,0,0), "sphere.mesh", "Examples/Chrome");
    myPaddle = GameObject(mSceneMgr, Ogre::Vector3(0,0,100), "cube.mesh", "Examples/BumpyMetal");
    myPaddle.scale(2);
    
    
    /*
    // Create an Entity
    Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "ogrehead.mesh");
 
    // Create a SceneNode and attach the Entity to it
    Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HeadNode");
    headNode->attachObject(ogreHead);
    
    headNode->scale( .5, 1, 2 ); 
    
    Ogre::Entity* ogreHead2 = mSceneMgr->createEntity( "Head2", "ogrehead.mesh" );
    //Ogre::SceneNode* headNode2 = mSceneMgr->getRootSceneNode()->createChildSceneNode( "HeadNode2", Ogre::Vector3( 100, 0, 0 ) );
    Ogre::SceneNode* headNode2 = headNode->createChildSceneNode( "HeadNode2", Ogre::Vector3( 100, 0, 0 ) );
    headNode2->attachObject( ogreHead2 );
    
    headNode2->scale( 1, 2, 1 );
    
    headNode2->translate( Ogre::Vector3( 10, 0, 10 ) );
    headNode->translate( Ogre::Vector3( 25, 0, 0 ) );
    
    */
    
    Ogre::Plane plane_x_pos_in(Ogre::Vector3::NEGATIVE_UNIT_X, -100);
    Ogre::Plane plane_x_neg_in(Ogre::Vector3::UNIT_X, -100);
    Ogre::Plane plane_y_pos_in(Ogre::Vector3::NEGATIVE_UNIT_Y, -100);
    Ogre::Plane plane_y_neg_in(Ogre::Vector3::UNIT_Y, -100);
    Ogre::Plane plane_z_pos_in(Ogre::Vector3::NEGATIVE_UNIT_Z, -100);
    Ogre::Plane plane_z_neg_in(Ogre::Vector3::UNIT_Z, -100);
    
    // plane_x_pos_in
	Ogre::MeshManager::getSingleton().createPlane("plane_x_pos_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_x_pos_in, 200.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* plane1 = mSceneMgr->createEntity("ent_plane_x_pos_in", "plane_x_pos_in");
	plane1->setMaterialName("Examples/Rockwall");
	plane1->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(plane1);
    
    // plane_x_neg_in
	Ogre::MeshManager::getSingleton().createPlane("plane_x_neg_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_x_neg_in, 200.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* plane2 = mSceneMgr->createEntity("ent_plane_x_neg_in", "plane_x_neg_in");
	plane2->setMaterialName("Examples/Rockwall");
	plane2->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(plane2);
    
    // plane_y_pos_in
	Ogre::MeshManager::getSingleton().createPlane("plane_y_pos_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_y_pos_in, 200.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_X);
	Ogre::Entity* plane3 = mSceneMgr->createEntity("ent_plane_y_pos_in", "plane_y_pos_in");
	plane3->setMaterialName("Examples/Rockwall");
	plane3->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(plane3);
    
    // plane_y_neg_in
	Ogre::MeshManager::getSingleton().createPlane("plane_y_neg_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_y_neg_in, 200.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_X);
	Ogre::Entity* plane4 = mSceneMgr->createEntity("ent_plane_y_neg_in", "plane_y_neg_in");
	plane4->setMaterialName("Examples/Rockwall");
	plane4->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(plane4);
    
    // plane_z_pos_in
	Ogre::MeshManager::getSingleton().createPlane("plane_z_pos_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_z_pos_in, 200.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* plane5 = mSceneMgr->createEntity("ent_plane_z_pos_in", "plane_z_pos_in");
	plane5->setMaterialName("Examples/Rockwall");
	plane5->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(plane5);
    
    // plane_z_neg_in
	Ogre::MeshManager::getSingleton().createPlane("plane_z_neg_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_z_neg_in, 200.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* plane6 = mSceneMgr->createEntity("ent_plane_z_neg_in", "plane_z_neg_in");
	plane6->setMaterialName("Examples/Rockwall");
	plane6->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(plane6);
	
    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(75.0f, 75.0f, 75.0f);
    light->setDiffuseColour(1,0,0);
}


void TutorialApplication::createFrameListener(void){
    srand(time(0));  // Initialize random number generator.
    // Set default values for variables
    float r1 = (rand() % 100);
    float r2 = (rand() % 100);
    float r3 = (rand() % 100);
    
    //mDirection = Ogre::Vector3(r1, r2, r3);
    mDirection = Ogre::Vector3(r1, r2, -1000);
    
    mDirection.normalise();
    mDirection /= 5.0; // to adjust speed
    
    mCurrPosition = Ogre::Vector3::ZERO;
    mNextPosition = mDirection;
    
    //mBallSpeed = 35.0f;
    
    BaseApplication::createFrameListener();
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent &evt){
    
    // Move the paddle
    myPaddle.setDirection(Ogre::Vector3(deltaX, -deltaY, 0));
    myPaddle.setSpeed(sqrt(deltaX*deltaX + deltaY*deltaY));
    myPaddle.move(evt);
    
    //cout << myPaddle.direction << endl;
    //cout << myPaddle.speed << endl;
    
    /*
    Reflection vector formula:
    
    L = light
    R = reflection
    
    R = 2(N dot L)N - L
    
    */
    
    
    // The ball hits the positive x plane
    if (mCurrPosition.x > 90 ){
        mTempVector = (-2.0 * (Ogre::Vector3::NEGATIVE_UNIT_X.dotProduct(mDirection)) * Ogre::Vector3::NEGATIVE_UNIT_X) + mDirection;
         mDirection = mTempVector;
         //cout << "hit!" << endl;
    }
    
    // The ball hits the negative x plane
    else if (mCurrPosition.x < -90 ){
        mTempVector = (-2.0 * (Ogre::Vector3::UNIT_X.dotProduct(mDirection)) * Ogre::Vector3::UNIT_X) + mDirection;
         mDirection = mTempVector;
         //cout << "hit!" << endl;
    }
    
    // The ball hits the positive y plane
    else if (mCurrPosition.y > 90 ){
        mTempVector = (-2.0 * (Ogre::Vector3::NEGATIVE_UNIT_Y.dotProduct(mDirection)) * Ogre::Vector3::NEGATIVE_UNIT_Y) + mDirection;
         mDirection = mTempVector;
         //cout << "hit!" << endl;
    }
    
    // The ball hits the negative y plane
    else if (mCurrPosition.y < -90 ){
        mTempVector = (-2.0 * (Ogre::Vector3::UNIT_Y.dotProduct(mDirection)) * Ogre::Vector3::UNIT_Y) + mDirection;
         mDirection = mTempVector;
         //cout << "hit!" << endl;
    }
    
    // The ball hits the positive z plane
    else if (mCurrPosition.z > 90 ){
        mTempVector = (-2.0 * (Ogre::Vector3::NEGATIVE_UNIT_Z.dotProduct(mDirection)) * Ogre::Vector3::NEGATIVE_UNIT_Z) + mDirection;
         mDirection = mTempVector;
         if (abs(myPaddle.location.x - mCurrPosition.x) <= 20 && abs(myPaddle.location.y - mCurrPosition.y) <= 20){
            cout << "HIT" << endl;
         }
         else{
            cout << "MISS" << endl;
            ready = true;
            float r1 = (rand() % 100);
            float r2 = (rand() % 100);
            mDirection = Ogre::Vector3(r1, r2, -1000);
            mDirection.normalise();
            mDirection /= 5.0; // to adjust speed
            mSceneMgr->getSceneNode("mBall")->setPosition(0,0,0);
            mCurrPosition = Ogre::Vector3::ZERO;
         }
         //cout << "hit!" << endl;
    }
    
    // The ball hits the negative z plane
    else if (mCurrPosition.z < -90 ){
        mTempVector = (-2.0 * (Ogre::Vector3::UNIT_Z.dotProduct(mDirection)) * Ogre::Vector3::UNIT_Z) + mDirection;
         mDirection = mTempVector;
         //cout << "hit!" << endl;
    }
    
    if (!ready){
        mSceneMgr->getSceneNode("mBall")->translate(mDirection);
        mCurrPosition += mDirection;
    }
    
	return BaseApplication::frameRenderingQueued(evt);
}
/*
bool TutorialApplication::keyPressed( const OIS::KeyEvent &evt ){
    switch (evt.key){
        case OIS::KC_SYSRQ:
            mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
            break;
        case OIS::KC_ESCAPE:
            exitCmd = true;
            mShutDown = true;
            break;
        default:
            break;
    }
    return true;
}

bool TutorialApplication::keyReleased( const OIS::KeyEvent &evt ){
    return true;
}
*/
bool TutorialApplication::mouseMoved( const OIS::MouseEvent &evt ){ 
    deltaX = evt.state.X.rel;
    deltaY = evt.state.Y.rel;
    //cout << "move: (" << deltaX << "," << deltaY << ")" << endl;
    return true;
}

bool TutorialApplication::mousePressed( const OIS::MouseEvent &evt, OIS::MouseButtonID id ){
    switch (id) {
        case OIS::MB_Left:
            if (ready){
                cout << "Left Click: Start Game" << endl;
                ready = false;
            }
            break;
        case OIS::MB_Right: 
            cout << "Right Click" << endl;
            break;
        default:
            break;
    }
    return true;
}

bool TutorialApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id ){
    return true;
}



#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
 
#ifdef __cplusplus
extern "C" {
#endif
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;
 
        try 
        {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR| MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
#endif
        }
 
        return 0;
    }
 
#ifdef __cplusplus
}
#endif
