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
    //exitCmd = false; 
    ready = true;
    speed_default = 250;
    score = 0;
}
 
TutorialApplication::~TutorialApplication(void)
{
}
 
//-------------------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
    mCamera->setPosition(0,0,450); 
    
    // Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    
    // Create game objects
    myBall = GameObject(mSceneMgr, Ogre::Vector3(0,0,0), "sphere.mesh", "Examples/GreenSkin");
    myPaddle = GameObject(mSceneMgr, Ogre::Vector3(0,0,210), "cube.mesh", "Examples/BumpyMetal");
    myPaddle.scale(2);
    
    Ogre::Plane plane_x_pos_in(Ogre::Vector3::NEGATIVE_UNIT_X, -100);
    Ogre::Plane plane_x_neg_in(Ogre::Vector3::UNIT_X, -100);
    Ogre::Plane plane_y_pos_in(Ogre::Vector3::NEGATIVE_UNIT_Y, -100);
    Ogre::Plane plane_y_neg_in(Ogre::Vector3::UNIT_Y, -100);
    Ogre::Plane plane_z_pos_in(Ogre::Vector3::NEGATIVE_UNIT_Z, -200);
    Ogre::Plane plane_z_neg_in(Ogre::Vector3::UNIT_Z, -200);
    
    // plane_x_pos_in
	Ogre::MeshManager::getSingleton().createPlane("plane_x_pos_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_x_pos_in, 200.0f, 400.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* plane1 = mSceneMgr->createEntity("ent_plane_x_pos_in", "plane_x_pos_in");
	plane1->setMaterialName("Examples/Rockwall");
	plane1->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(plane1);
    
    // plane_x_neg_in
	Ogre::MeshManager::getSingleton().createPlane("plane_x_neg_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_x_neg_in, 200.0f, 400.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_Z);
	Ogre::Entity* plane2 = mSceneMgr->createEntity("ent_plane_x_neg_in", "plane_x_neg_in");
	plane2->setMaterialName("Examples/Rockwall");
	plane2->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(plane2);
    
    // plane_y_pos_in
	Ogre::MeshManager::getSingleton().createPlane("plane_y_pos_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_y_pos_in, 400.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_X);
	Ogre::Entity* plane3 = mSceneMgr->createEntity("ent_plane_y_pos_in", "plane_y_pos_in");
	plane3->setMaterialName("Examples/Rockwall");
	plane3->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(plane3);
    
    // plane_y_neg_in
	Ogre::MeshManager::getSingleton().createPlane("plane_y_neg_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_y_neg_in, 400.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_X);
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
    light->setDiffuseColour(1,1,1);
}


void TutorialApplication::createFrameListener(void){
    srand(time(0));  // Initialize random number generator.
    // Set default values for variables
    float r1 = (rand() % 100);
    float r2 = (rand() % 100);
    float r3 = (rand() % 100);
    
    myBall.direction = Ogre::Vector3(r1, r2, -500);
    myBall.direction.normalise();
    myBall.speed = speed_default;
    myBall.setPosition(Ogre::Vector3::ZERO);
    
    BaseApplication::createFrameListener();
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent &evt){
    
    // Move the ball
    if (!ready){
        myBall.move(evt);
    }
    
    // Move the paddle
    myPaddle.direction = Ogre::Vector3(deltaX, -deltaY, 0);
    myPaddle.speed = sqrt(deltaX*deltaX + deltaY*deltaY);
    myPaddle.move(evt);
    
    //cout << myPaddle.direction << endl;
    //cout << myPaddle.speed << endl;
    
    /*
    Reflection vector formula:
    
    L = light
    R = reflection
    
    R = 2(N dot L)N - L
    
    */
    
    //myBall.nextPosition.x >= 90-myBall.position.x
    
    // The ball hits the positive x plane
    if (myBall.position.x > 90 || myBall.nextPosition.x > 90){
        myBall.setPosition(Ogre::Vector3(90, myBall.position.y, myBall.position.z));
        myBall.direction += (-2.0 * (Ogre::Vector3::NEGATIVE_UNIT_X.dotProduct(myBall.direction)) * Ogre::Vector3::NEGATIVE_UNIT_X);
        //cout << "hit!" << endl;
    }
    
    // The ball hits the negative x plane
    else if (myBall.position.x < -90 || myBall.nextPosition.x < -90){
        myBall.setPosition(Ogre::Vector3(-90, myBall.position.y, myBall.position.z));
        myBall.direction += (-2.0 * (Ogre::Vector3::UNIT_X.dotProduct(myBall.direction)) * Ogre::Vector3::UNIT_X);
        //cout << "hit!" << endl;
    }
    
    // The ball hits the positive y plane
    else if (myBall.position.y > 90 || myBall.nextPosition.y > 90){
        myBall.setPosition(Ogre::Vector3(myBall.position.x, 90, myBall.position.z));
        myBall.direction += (-2.0 * (Ogre::Vector3::NEGATIVE_UNIT_Y.dotProduct(myBall.direction)) * Ogre::Vector3::NEGATIVE_UNIT_Y);
        //cout << "hit!" << endl;
    }
    
    // The ball hits the negative y plane
    else if (myBall.position.y < -90 || myBall.nextPosition.y < -90){
        myBall.setPosition(Ogre::Vector3(myBall.position.x, -90, myBall.position.z));
        myBall.direction += (-2.0 * (Ogre::Vector3::UNIT_Y.dotProduct(myBall.direction)) * Ogre::Vector3::UNIT_Y);
        //cout << "hit!" << endl;
    }
    
    // The ball hits the positive z plane
    else if (myBall.position.z > 190 || myBall.nextPosition.z > 190){
        myBall.setPosition(Ogre::Vector3(myBall.position.x, myBall.position.y, 190));
        myBall.direction += (-2.0 * (Ogre::Vector3::NEGATIVE_UNIT_Z.dotProduct(myBall.direction)) * Ogre::Vector3::NEGATIVE_UNIT_Z);
        if (abs(myPaddle.position.x - myBall.position.x) <= 25 && abs(myPaddle.position.y - myBall.position.y) <= 25){
            score += myBall.speed;
            myBall.speed += 10;
            //cout << "HIT" << endl;
        }
        else if (abs(myPaddle.nextPosition.x - myBall.nextPosition.x) <= 25 && abs(myPaddle.nextPosition.y - myBall.nextPosition.y) <= 25){
            score += myBall.speed;
            myBall.speed += 10;
            //cout << "HIT" << endl;
        }
        else{
            cout << "Score:" << score << "\nClick to restart" << endl;
            score = 0;
            ready = true;
            float r1 = (rand() % 100);
            float r2 = (rand() % 100);
            myBall.direction = Ogre::Vector3(r1, r2, -1000);
            myBall.direction.normalise();
            myBall.speed = speed_default;
            myBall.setPosition(Ogre::Vector3::ZERO);
        }
        //cout << "hit!" << endl;
    }
    
    // The ball hits the negative z plane
    else if (myBall.position.z < -190 || myBall.nextPosition.z < -190){
        myBall.setPosition(Ogre::Vector3(myBall.position.x, myBall.position.y, -190));
        myBall.direction += (-2.0 * (Ogre::Vector3::UNIT_Z.dotProduct(myBall.direction)) * Ogre::Vector3::UNIT_Z);
         //cout << "hit!" << endl;
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
            //cout << "Left Click" << endl;
            if (ready){
                ready = false;
            }
            break;
        case OIS::MB_Right: 
            //cout << "Right Click" << endl;
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
