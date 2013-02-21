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
#include <stdlib.h>

#include <ctime> // For time()
#include <cstdlib> // For srand() and rand()

//-------------------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
}
//-------------------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//-------------------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
    // create your scene here :)
   // Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
 
    // Create an Entity
    ball = mSceneMgr->createEntity("Ball", "sphere.mesh");
    paddle = mSceneMgr->createEntity("Paddle", "cube.mesh");
    shadow = mSceneMgr->createEntity("Shadow", "sphere.mesh");
 
    // Create a SceneNode and attach the Entity to it
    ballNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BallNode");
    ballNode->attachObject(ball);
    ballNode->scale(.4,.4,.4);

    //paddle
    //paddle->setMaterialName("Glass");
    paddleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("PaddleNode");
    paddleNode->attachObject(paddle);
    paddleNode->scale(1,1,.1);
    paddleNode->setPosition(0,0,1000);

    //shadow

    Ogre::ManualObject * circle = mSceneMgr->createManualObject("circle_name");
 
    float const radius = 40;
 
    // accuracy is the count of points (and lines).
    // Higher values make the circle smoother, but may slowdown the performance.
    // The performance also is related to the count of circles.
    float const accuracy = 35;
 
    circle->begin("Black", Ogre::RenderOperation::OT_TRIANGLE_FAN);
 
    unsigned point_index = 0;
    circle->colour(Ogre::ColourValue::Black);
    circle->position(0,0,0);
    circle->index(point_index++);
    for(float theta = 2 * Ogre::Math::PI; theta >= 0 ; theta -= Ogre::Math::PI / accuracy) {
	circle->colour(Ogre::ColourValue::Black);
        circle->position(radius * cos(theta), 0, radius * sin(theta));
        circle->index(point_index++);
    }
    circle->index(1); // Rejoins the last point to the first.
 
    circle->end();

 
    shadowNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("ShadowNode");
    shadowNode->attachObject(circle);
    shadowNode->setPosition(0,-499,0);    
 
    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(20.0f, 300.0f, 700.0f);

	Ogre::Plane bottom(Ogre::Vector3::UNIT_Y, -500);
	Ogre::Plane top(Ogre::Vector3::NEGATIVE_UNIT_Y, -500);
	Ogre::Plane left(Ogre::Vector3::UNIT_X, -500);
	Ogre::Plane right(Ogre::Vector3::NEGATIVE_UNIT_X, -500);
	//Ogre::Plane front(Ogre::Vector3::NEGATIVE_UNIT_Z, -500);
	Ogre::Plane back(Ogre::Vector3::UNIT_Z, -1000);

	Ogre::MeshManager& manager = Ogre::MeshManager::getSingleton();

	manager.createPlane("bottom", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	bottom, 1000, 2000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
	manager.createPlane("top", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	top, 1000, 2000, 20, 20, true, 1, 5, 5, Ogre::Vector3::NEGATIVE_UNIT_Z);
	manager.createPlane("left", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	left, 2000, 1000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
	manager.createPlane("right", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	right, 2000, 1000, 20, 20, true, 1, 5, 5, Ogre::Vector3::NEGATIVE_UNIT_Y);
	/*manager.createPlane("front", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	front, 1000, 1000, 20, 20, true, 1, 5, 5, Ogre::Vector3::NEGATIVE_UNIT_X);*/
	manager.createPlane("back", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	back, 1000, 1000, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_X);
	Ogre::Entity* entBottom = mSceneMgr->createEntity("CubeBottom", "bottom");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entBottom);
	Ogre::Entity* entTop = mSceneMgr->createEntity("CubeTop", "top");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entTop);
	Ogre::Entity* entLeft = mSceneMgr->createEntity("CubeLeft", "left");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entLeft);
	Ogre::Entity* entRight = mSceneMgr->createEntity("CubeRight", "right");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entRight);
	/*Ogre::Entity* entFront = mSceneMgr->createEntity("CubeFront", "front");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entFront);*/
	Ogre::Entity* entBack = mSceneMgr->createEntity("CubeBack", "back");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entBack);
}

void TutorialApplication::createFrameListener(void){
    // Set default values for variables
    srand(time(0)); // Initialize random number generator.
    float r1 = (rand() % 100);
    float r2 = (rand() % 100);
    float r3 = (rand() % 100);
    
    mDirection = Ogre::Vector3(r1, r2, r3);
    mDirection.normalise();
    mDirection /= 10.0; // to adjust speed
    
    mCurrPosition = Ogre::Vector3::ZERO;
    mNextPosition = mDirection;
    
    //mBallSpeed = 35.0f;
    
    BaseApplication::createFrameListener();
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent &evt){
    
    /*
Reflection vector formula:
L = light
R = reflection
R = 2(N dot L)N - L
*/
    
    
    // The ball hits the positive x plane
    if (mCurrPosition.x > 460 ){
        mTempVector = (-2.0 * (Ogre::Vector3::NEGATIVE_UNIT_X.dotProduct(mDirection)) * Ogre::Vector3::NEGATIVE_UNIT_X) + mDirection;
         mDirection = mTempVector;
         cout << "hit!" << endl;
    }
    
    // The ball hits the negative x plane
    else if (mCurrPosition.x < -460 ){
        mTempVector = (-2.0 * (Ogre::Vector3::UNIT_X.dotProduct(mDirection)) * Ogre::Vector3::UNIT_X) + mDirection;
         mDirection = mTempVector;
         cout << "hit!" << endl;
    }
    
    // The ball hits the positive y plane
    else if (mCurrPosition.y > 460 ){
        mTempVector = (-2.0 * (Ogre::Vector3::NEGATIVE_UNIT_Y.dotProduct(mDirection)) * Ogre::Vector3::NEGATIVE_UNIT_Y) + mDirection;
         mDirection = mTempVector;
         cout << "hit!" << endl;
    }
    
    // The ball hits the negative y plane
    else if (mCurrPosition.y < -460 ){
        mTempVector = (-2.0 * (Ogre::Vector3::UNIT_Y.dotProduct(mDirection)) * Ogre::Vector3::UNIT_Y) + mDirection;
         mDirection = mTempVector;
         cout << "hit!" << endl;
    }
    
    // The ball hits the positive z plane
    else if (mCurrPosition.z > 960 ){
        mTempVector = (-2.0 * (Ogre::Vector3::NEGATIVE_UNIT_Z.dotProduct(mDirection)) * Ogre::Vector3::NEGATIVE_UNIT_Z) + mDirection;
         mDirection = mTempVector;
         cout << "hit!" << endl;
    }
    
    // The ball hits the negative z plane
    else if (mCurrPosition.z < -960 ){
        mTempVector = (-2.0 * (Ogre::Vector3::UNIT_Z.dotProduct(mDirection)) * Ogre::Vector3::UNIT_Z) + mDirection;
         mDirection = mTempVector;
         cout << "hit!" << endl;
    }
    
    mSceneMgr->getSceneNode("BallNode")->translate(mDirection);
    mCurrPosition += mDirection;
    
    
return BaseApplication::frameRenderingQueued(evt);
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

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif

