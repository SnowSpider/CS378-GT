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
#include <sstream>
#include <math.h>

//#include <LinearMath/btVector3.h>
#include <btBulletCollisionCommon.h>

TutorialApplication::TutorialApplication(void)
: mRenderer(0)
{
    deltaX = 0;
    deltaY = 0;
    xi = 0;
    yi = 0;
    //exitCmd = false; 
    ready = true;
    speed_default = 200;
    increment = 25;
    score = 0;
    highScore = 0;
    soundIs = true;
    startUp = true;
    lost = false;
}
 
TutorialApplication::~TutorialApplication(void)
{
}
 
//-------------------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    mCamera->setPosition(0,0,450); 

    // Create SoundManager
    sound.open();
    sound.turnOn();
    
    // Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
    
    // Create game objects
    myBall = GameObject(mSceneMgr, btVector3(0,0,0), "sphere.mesh", "Examples/GreenSkin");
    myPaddle = GameObject(mSceneMgr, btVector3(0,0,210), "cube.mesh", "MyMaterials/wood1");
    myPaddle.scale(2, 2, 0.1);
    
    Ogre::Plane plane_x_pos_in(Ogre::Vector3::NEGATIVE_UNIT_X, -100);
    Ogre::Plane plane_x_neg_in(Ogre::Vector3::UNIT_X, -100);
    Ogre::Plane plane_y_pos_in(Ogre::Vector3::NEGATIVE_UNIT_Y, -100);
    Ogre::Plane plane_y_neg_in(Ogre::Vector3::UNIT_Y, -100);
    Ogre::Plane plane_z_pos_in(Ogre::Vector3::NEGATIVE_UNIT_Z, -200);
    Ogre::Plane plane_z_neg_in(Ogre::Vector3::UNIT_Z, -200);
    
    // plane_x_pos_in
	Ogre::MeshManager::getSingleton().createPlane("plane_x_pos_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_x_pos_in, 400.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* plane1 = mSceneMgr->createEntity("ent_plane_x_pos_in", "plane_x_pos_in");
	plane1->setMaterialName("Examples/Rockwall");
	plane1->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(plane1);
    
    // plane_x_neg_in
	Ogre::MeshManager::getSingleton().createPlane("plane_x_neg_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_x_neg_in, 400.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* plane2 = mSceneMgr->createEntity("ent_plane_x_neg_in", "plane_x_neg_in");
	plane2->setMaterialName("Examples/Rockwall");
	plane2->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(plane2);
    
    // plane_y_pos_in
	Ogre::MeshManager::getSingleton().createPlane("plane_y_pos_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_y_pos_in, 400.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_X);
	Ogre::Entity* plane3 = mSceneMgr->createEntity("ent_plane_y_pos_in", "plane_y_pos_in");
	plane3->setMaterialName("MyMaterials/wood2");
	plane3->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(plane3);
    
    // plane_y_neg_in
	Ogre::MeshManager::getSingleton().createPlane("plane_y_neg_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_y_neg_in, 400.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_X);
	Ogre::Entity* plane4 = mSceneMgr->createEntity("ent_plane_y_neg_in", "plane_y_neg_in");
	plane4->setMaterialName("MyMaterials/wood2");
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
	plane6->setMaterialName("MyMaterials/buddha1");
	plane6->setCastShadows(false);
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(plane6);
	
    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    light->setPosition(75.0f, 75.0f, 75.0f);
    light->setDiffuseColour(1,1,1);
    
    //CEGUI SHIT(Stuff Hard to Improve Technically)
    CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
 
    CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
 
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
 
    CEGUI::Window *counter = wmgr.createWindow("TaharezLook/Button", "Counter");
    counter->setText("Score:  ");
    counter->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
 
    sheet->addChildWindow(counter);

    CEGUI::Window *sounder = wmgr.createWindow("TaharezLook/Button", "SounderDontBreak");
    sounder->setText("Sound: On");
    sounder->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
    sounder->setHorizontalAlignment(CEGUI::HA_RIGHT);
 
    sheet->addChildWindow(sounder);

    CEGUI::Window *starter = wmgr.createWindow("TaharezLook/StaticText", "StartingWindow");
    starter->setText("Welcome, please click to begin.\nClick X to toggle sound.");
    starter->setSize(CEGUI::UVector2(CEGUI::UDim(0.30, 0), CEGUI::UDim(0.10, 0)));
    starter->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35f, 0), CEGUI::UDim(0.45f, 0 )));
 
    sheet->addChildWindow(starter);

    CEGUI::Window *gameOver = wmgr.createWindow("TaharezLook/StaticText", "GameOverWindow");
    gameOver->setText("");
    gameOver->setVisible(false);
    gameOver->setSize(CEGUI::UVector2(CEGUI::UDim(0.30, 0), CEGUI::UDim(0.15, 0)));
    gameOver->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35f, 0), CEGUI::UDim(0.425f, 0 )));
 
    sheet->addChildWindow(gameOver);
    CEGUI::System::getSingleton().setGUISheet(sheet);
}


void TutorialApplication::createFrameListener(void){
    srand(time(0));  // Initialize random number generator.
    // Set default values for variables
    float r1 = (rand() % 100);
    float r2 = (rand() % 100);
    float r3 = (rand() % 100);
    
    myBall.direction = btVector3(r1, r2, -500);
    myBall.direction.normalize();
    myBall.speed = speed_default;
    myBall.setPosition(btVector3(0,0,0));

    
    BaseApplication::createFrameListener();
}

bool TutorialApplication::frameRenderingQueued(const Ogre::FrameEvent &evt){
    
    // Move the ball
    if (!ready){
        myBall.move(evt);
    }
    
    // Move the paddle
    myPaddle.direction = btVector3(deltaX, deltaY, 0);
    myPaddle.speed = sqrt(deltaX*deltaX + deltaY*deltaY);
    myPaddle.setPosition(btVector3(xi, yi, 210)); 
    //myPaddle.move(evt);
    
    if (myPaddle.position.x() > 90){
        myPaddle.setPosition(btVector3(90, myPaddle.position.y(), myPaddle.position.z()));
    }
    if (myPaddle.position.x() < -90){
        myPaddle.setPosition(btVector3(-90, myPaddle.position.y(), myPaddle.position.z()));
    }
    if (myPaddle.position.y() > 90){
        myPaddle.setPosition(btVector3(myPaddle.position.x(), 90, myPaddle.position.z()));
    }
    if (myPaddle.position.y() < -90){
        myPaddle.setPosition(btVector3(myPaddle.position.x(), -90, myPaddle.position.z()));
    }
    
    
    //cout << myPaddle.direction << endl;
    //cout << myPaddle.speed << endl;
    
    /*
    Reflection vector formula:
    
    L = light
    R = reflection
    
    R = 2(N dot L)N - L
    
    */
    
    //myBall.nextPosition.x() >= 90-myBall.position.x()
    
    // The ball hits the positive x plane
    if (myBall.position.x() > 90 || myBall.nextPosition.x() > 90){
        myBall.setPosition(btVector3(90, myBall.position.y(), myBall.position.z()));
        myBall.direction += (-2.0 * (btVector3(-1,0,0).dot(myBall.direction)) * btVector3(-1,0,0));
        //cout << "hit!" << endl;
	sound.PlaySound("boing.wav");
    }
    
    // The ball hits the negative x plane
    else if (myBall.position.x() < -90 || myBall.nextPosition.x() < -90){
        myBall.setPosition(btVector3(-90, myBall.position.y(), myBall.position.z()));
        myBall.direction += (-2.0 * (btVector3(1,0,0).dot(myBall.direction)) * btVector3(1,0,0));
        //cout << "hit!" << endl;
	sound.PlaySound("boing.wav");
    }
    
    // The ball hits the positive y plane
    else if (myBall.position.y() > 90 || myBall.nextPosition.y() > 90){
        myBall.setPosition(btVector3(myBall.position.x(), 90, myBall.position.z()));
        myBall.direction += (-2.0 * (btVector3(0,-1,0).dot(myBall.direction)) * btVector3(0,-1,0));
        //cout << "hit!" << endl;
	sound.PlaySound("boing.wav");
    }
    
    // The ball hits the negative y plane
    else if (myBall.position.y() < -90 || myBall.nextPosition.y() < -90){
        myBall.setPosition(btVector3(myBall.position.x(), -90, myBall.position.z()));
        myBall.direction += (-2.0 * (btVector3(0,1,0).dot(myBall.direction)) * btVector3(0,1,0));
        //cout << "hit!" << endl;
	sound.PlaySound("boing.wav");
    }
    
    // The ball hits the positive z plane
    else if (myBall.position.z() > 190 || myBall.nextPosition.z() > 190){
        myBall.setPosition(btVector3(myBall.position.x(), myBall.position.y(), 190));
        myBall.direction += (-2.0 * (btVector3(0,0,-1).dot(myBall.direction)) * btVector3(0,0,-1));
        if (abs(myPaddle.position.x() - myBall.position.x()) <= 25 && abs(myPaddle.position.y() - myBall.position.y()) <= 25){
            score += myBall.speed;
            myBall.speed += increment;
            CEGUI::Window* counter = CEGUI::WindowManager::getSingleton().getWindow("Counter");
            std::string shownScore;
            std::stringstream out;
            out << score;
            shownScore = out.str();
            counter->setText("Score: " + shownScore);
            //cout << "HIT" << endl;
        }
        else if (abs(myPaddle.nextPosition.x() - myBall.nextPosition.x()) <= 25 && abs(myPaddle.nextPosition.y() - myBall.nextPosition.y()) <= 25){
            score += myBall.speed;
            myBall.speed += increment;
            CEGUI::Window* counter = CEGUI::WindowManager::getSingleton().getWindow("Counter");
            std::string shownScore;
            std::stringstream out;
            out << score;
            shownScore = out.str();
            counter->setText("Score: " + shownScore);
            //cout << "HIT" << endl;
	sound.PlaySound("boing.wav");
        }
        else{
            //cout << "Score:" << score << "\nClick to restart" << endl;
            lost = true;
            if(score > highScore)
                highScore = score;
            CEGUI::Window* gameOver = CEGUI::WindowManager::getSingleton().getWindow("GameOverWindow");
            std::string shownScore;
            std::string shownHighScore;
            std::stringstream out;
            std::stringstream highOut;
            out << score;
            highOut << highScore;
            shownScore = out.str();
            shownHighScore = highOut.str();
            gameOver->setText("Game Over, Final Score: " + shownScore + "\nTop Score: " + shownHighScore + "\nClick To Restart");
            gameOver->setVisible(true);
            score = 0;
            ready = true;
            float r1 = (rand() % 100);
            float r2 = (rand() % 100);
            myBall.direction = btVector3(r1, r2, -1000);
            myBall.direction.normalize();
            myBall.speed = speed_default;
            myBall.setPosition(btVector3(0,0,0));
        }
        //cout << "hit!" << endl;
    }
    
    // The ball hits the negative z plane
    else if (myBall.position.z() < -190 || myBall.nextPosition.z() < -190){
        myBall.setPosition(btVector3(myBall.position.x(), myBall.position.y(), -190));
        myBall.direction += (-2.0 * (btVector3(0,0,1).dot(myBall.direction)) * btVector3(0,0,1));
         //cout << "hit!" << endl;
	sound.PlaySound("boing.wav");
    }

    if(mWindow->isClosed())
        return false;
 
    if(mShutDown)
        return false;
 
    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();
    
	return BaseApplication::frameRenderingQueued(evt);
}

bool TutorialApplication::keyPressed( const OIS::KeyEvent &evt ){
    CEGUI::System &sys = CEGUI::System::getSingleton();
    sys.injectKeyDown(evt.key);
    sys.injectChar(evt.text);
 
    if (evt.key == OIS::KC_X)
    {
        TutorialApplication::soundSwitch();
        CEGUI::Window* sounder = CEGUI::WindowManager::getSingleton().getWindow("SounderDontBreak");
        if (soundIs)
            sounder->setText("Sound: On");
        else
            sounder->setText("Sound: Off");
    }
    if (evt.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }
 
    mCameraMan->injectKeyDown(evt);
    return true;
}

bool TutorialApplication::keyReleased( const OIS::KeyEvent &evt ){
    if(CEGUI::System::getSingleton().injectKeyUp(evt.key)) return true;
    mCameraMan->injectKeyUp(evt);
    return true;
}

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;
        break;
 
    case OIS::MB_Right:
        return CEGUI::RightButton;
        break;
 
    case OIS::MB_Middle:
        return CEGUI::MiddleButton;
        break;
 
    default:
        return CEGUI::LeftButton;
        break;
    }
}

bool TutorialApplication::mouseMoved( const OIS::MouseEvent &evt ){ 
    deltaX = evt.state.X.rel;
    deltaY = -evt.state.Y.rel;
    //xi = evt.state.X.abs - (1024/2);
    //yi = -evt.state.Y.abs + (768/2); 
    xi += evt.state.X.rel;
    yi += -evt.state.Y.rel; 
    if (xi > 90) xi = 90;
    if (xi < -90) xi = -90;
    if (yi > 90) yi = 90;
    if (yi < -90) yi = -90;
    //cout << "move: (" << deltaX << "," << deltaY << ")" << endl;
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
    if(startUp)
    {
        startUp = false;
        CEGUI::Window* starting = CEGUI::WindowManager::getSingleton().getWindow("StartingWindow");
        starting->setVisible(false);
        CEGUI::Window* counter = CEGUI::WindowManager::getSingleton().getWindow("Counter");
        counter->setText("Score: 0");
    }
    if(lost)
    {
        lost = false;
        CEGUI::Window* gameOver = CEGUI::WindowManager::getSingleton().getWindow("GameOverWindow");
        gameOver->setVisible(false);
        CEGUI::Window* counter = CEGUI::WindowManager::getSingleton().getWindow("Counter");
        counter->setText("Score: 0");
    }
    if(CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id))) return true;
    mCameraMan->injectMouseDown(evt, id);
    return true;
}

bool TutorialApplication::mouseReleased( const OIS::MouseEvent &evt, OIS::MouseButtonID id ){
    if(CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id))) return true;
    mCameraMan->injectMouseUp(evt, id);
    return true;
}

bool TutorialApplication::soundSwitch(void)
{
    if(soundIs)
       sound.turnOff();
    else
        sound.turnOn();
    soundIs = !soundIs;
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
