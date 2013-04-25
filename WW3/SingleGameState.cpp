//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "SingleGameState.hpp"

#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

#include <iostream>
#include <sstream>
#include <math.h>
#include <X11/Xlib.h>

#include <btBulletCollisionCommon.h>

#include <string.h>
#include "Planet.h"

//|||||||||||||||||||||||||||||||||||||||||||||||
 
using namespace Ogre;
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
SingleGameState::SingleGameState()
{
    m_MoveSpeed        = 50.0f;
    m_RotateSpeed    = 0.3f;
 
    m_bLMouseDown       = false;
    m_bRMouseDown       = false;
    m_bQuit             = false;
    m_bSettingsMode     = false;
 
    m_pDetailsPanel    = 0;
    
    // stuff from Assignment 2
    deltaX = 0;
    deltaY = 0;
    xi = 0;
    yi = 0;
    //exitCmd = false; 
    ready = true;
    speed_default = 200;
    increment = 50;
    score = 0;
    highScore = 0;
    soundIs = true;
    startUp = true;
    lost = false;
    buildingImage = 1;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void SingleGameState::enter()
{
    camDistance = 20000;
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    Display* pdsp = NULL;
    Window wid = 0;
    pdsp = XOpenDisplay( NULL );
    wid = DefaultRootWindow( pdsp );
    XWindowAttributes xwAttr;
    Status ret = XGetWindowAttributes( pdsp, wid, &xwAttr );
    CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Size(xwAttr.width, xwAttr.height));
    XCloseDisplay( pdsp );

    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering SingleGameState...");
    OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "GameSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
    
    m_pRSQ = m_pSceneMgr->createRayQuery(Ray());
    
    m_pCamera = m_pSceneMgr->createCamera("GameCamera");
    m_pSceneMgr->setSkyBox(true, "Examples/NebulaSkyBox");
    m_pCamera->setPosition(0,0,20000); 
    m_pCamera->lookAt(Vector3(0, 0, 0));
    m_pCamera->setNearClipDistance(5);
    
    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));
 
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
    m_pCurrentObject = 0;
 
    buildGUI();
 
    createScene();
    m_pCamera->setAutoTracking(true, cameraNode);
}

bool SingleGameState::pause()
{
    CEGUI::MouseCursor::getSingleton().hide();
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Pausing SingleGameState...");
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void SingleGameState::resume()
{
    CEGUI::MouseCursor::getSingleton().show();
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Resuming SingleGameState...");
    OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();
    buildGUI();
 
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
    m_bQuit = false;
}

void SingleGameState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving SingleGameState...");
    
    sound.close();
    myfile.close();
    
    m_pSceneMgr->destroyCamera(m_pCamera);
    m_pSceneMgr->destroyQuery(m_pRSQ);
    if(m_pSceneMgr) OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);

    //CEGUI::System::getSingleton().destroy();cout << "6" << endl;
    mRenderer->destroySystem();cout << "7" << endl;
    //delete mRenderer;
}

void SingleGameState::createScene()
{
    cout << "sizeof(double) = " << sizeof(double) << endl;
    cout << "sizeof(int) = " << sizeof(int) << endl;
    cout << "sizeof(long) = " << sizeof(long) << endl;
    cout << "sizeof(float) = " << sizeof(float) << endl;
    cout << "sizeof(char) = " << sizeof(char) << endl;
    cout << "sizeof(btVector3) = " << sizeof(btVector3) << endl;
    
    //cout << "sizeof(btVector3*) = " << sizeof(btVector3*) << endl;
    
    myfile.open("waterland.txt");

    // Create SoundManager
    sound.open();
    sound.turnOn();
    
    GameObject tempObject;
    
    // Create game objects
    
    // Earth
    earth = Planet( btVector3(0,0,0), //center
                    btVector3(0,1,0), //axis
                    btVector3(0,0,1), //longitude_zero
                    6371,
                    3);
    earth.init();
    earth.mapTerrain();
    earth.createManualObjects(m_pSceneMgr);
    mRayScnQuery = m_pSceneMgr->createRayQuery(Ogre::Ray());
    
    cameraNode = m_pSceneMgr->getRootSceneNode();
    
    
    
    // Create a Light and set its position
    Ogre::Light* light = m_pSceneMgr->createLight("MainLight");
    light->setPosition(20000.0f, 20000.0f, 20000.0f);
    light->setDiffuseColour(1,1,1);
    
    //CEGUI SHIT(Stuff Hard to Improve Technically)
    CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
 
    CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
 
    CEGUI::MouseCursor::getSingleton().setImage("TaharezLook", "MouseArrow");
    try
    {
        CEGUI::System::getSingleton().setDefaultMouseCursor( "TaharezLook", "MouseArrow" );
    }
    catch (CEGUI::Exception& e)
    {
        // something went wrong - probably did not load the imageset first!
        //TODO: Handle error,
    }
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");
 
    CEGUI::Window *counter = wmgr.createWindow("TaharezLook/StaticText", "Counter");
    counter->setText("    Resources\nMoney: $0\nPlutonium: 0kg\nUranium: 0kg");
    counter->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.15, 0)));
    counter->setProperty("FrameEnabled", "false");
    counter->setProperty("BackgroundEnabled", "false");
 
    sheet->addChildWindow(counter);

    CEGUI::ProgressBar *progressBar = static_cast<CEGUI::ProgressBar*>(wmgr.createWindow("TaharezLook/ProgressBar", "ProgressBar1"));
    sheet->addChildWindow(progressBar);
    progressBar->setSize(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.04f, 0)));
    progressBar->setHorizontalAlignment(CEGUI::HA_CENTRE);
    CEGUI::Window *resultWindow = wmgr.createWindow("TaharezLook/StaticText", "ProgressText1");
    sheet->addChildWindow(resultWindow);
    resultWindow->setHorizontalAlignment(CEGUI::HA_CENTRE);
    resultWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.04f, 0)));
    resultWindow->setText("Player Command Bases Remaining: 10/10");
    resultWindow->setProperty("FrameEnabled", "false");
    resultWindow->setProperty("BackgroundEnabled", "false");
    resultWindow->setProperty("HorzFormatting", "WordWrapCentred");

    progressBar = static_cast<CEGUI::ProgressBar*>(wmgr.createWindow("TaharezLook/ProgressBar", "ProgressBar2"));
    sheet->addChildWindow(progressBar);
    progressBar->setSize(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.04f, 0)));
    progressBar->setHorizontalAlignment(CEGUI::HA_CENTRE);
    progressBar->setYPosition(CEGUI::UDim(0.05f, 0));
    progressBar->setProgress(progressBar->getProgress() + 0.05f);
    resultWindow = wmgr.createWindow("TaharezLook/StaticText", "ProgressText2");
    sheet->addChildWindow(resultWindow);
    resultWindow->setHorizontalAlignment(CEGUI::HA_CENTRE);
    resultWindow->setYPosition(CEGUI::UDim(0.05f, 0));
    resultWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.04f, 0)));
    resultWindow->setText("Enemy Command Bases Remaining: 9/10");
    resultWindow->setProperty("FrameEnabled", "false");
    resultWindow->setProperty("BackgroundEnabled", "false");
    resultWindow->setProperty("HorzFormatting", "WordWrapCentred");

    CEGUI::ImagesetManager::getSingleton().create( "BCommandBase.imageset" );
    CEGUI::Window *w = wmgr.loadWindowLayout("BCommandBase.layout", "CommandBase");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( true );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::CommandBaseButton,this));

    sheet->addChildWindow(w);

    CEGUI::ImagesetManager::getSingleton().create( "ArmyBase.imageset" );
    w = wmgr.loadWindowLayout("ArmyBase.layout", "ArmyBase");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.20f, 0));
    w->setVisible( true );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::ArmyBaseButton,this));

    sheet->addChildWindow(w);

    CEGUI::ImagesetManager::getSingleton().create( "NavyBase.imageset" );
    w = wmgr.loadWindowLayout("NavyBase.layout", "NavyBase");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.35f, 0));
    w->setVisible( true );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::NavyBaseButton,this));

    sheet->addChildWindow(w);

    CEGUI::ImagesetManager::getSingleton().create( "AirForceBase.imageset" );
    w = wmgr.loadWindowLayout("AirForceBase.layout", "AirForceBase");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.50f, 0));
    w->setVisible( true );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::AirForceBaseButton,this));

    sheet->addChildWindow(w);
    
    CEGUI::ImagesetManager::getSingleton().create( "Infantry.imageset" );
    w = wmgr.loadWindowLayout("Infantry.layout", "Infantry");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::InfantryButton,this));

    sheet->addChildWindow(w);
    
    CEGUI::ImagesetManager::getSingleton().create( "Submarine.imageset" );
    w = wmgr.loadWindowLayout("Submarine.layout", "Submarine");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::SubmarineButton,this));

    sheet->addChildWindow(w);    
    
    CEGUI::ImagesetManager::getSingleton().create( "Destroyer.imageset" );
    w = wmgr.loadWindowLayout("Destroyer.layout", "Destroyer");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.20f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::DestroyerButton,this));

    sheet->addChildWindow(w);
    
    CEGUI::ImagesetManager::getSingleton().create( "Tank.imageset" );
    w = wmgr.loadWindowLayout("Tank.layout", "Tank");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::TankButton,this));

    sheet->addChildWindow(w);
    
    CEGUI::ImagesetManager::getSingleton().create( "Scud.imageset" );
    w = wmgr.loadWindowLayout("Scud.layout", "Scud");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.20f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::ScudButton,this));

    sheet->addChildWindow(w);

    CEGUI::ImagesetManager::getSingleton().create( "Bomber.imageset" );
    w = wmgr.loadWindowLayout("Bomber.layout", "Bomber");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::BomberButton,this));

    sheet->addChildWindow(w);
    
    CEGUI::ImagesetManager::getSingleton().create( "Fighter.imageset" );
    w = wmgr.loadWindowLayout("Fighter.layout", "Fightere");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.20f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::FighterButton,this));

    sheet->addChildWindow(w);

    CEGUI::Window *sounder = wmgr.createWindow("TaharezLook/Button", "SounderDontBreak");
    sounder->setText("Sound: On");
    sounder->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));
    sounder->setHorizontalAlignment(CEGUI::HA_RIGHT);
    sounder->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::soundButton,this));
    sheet->addChildWindow(sounder);

    CEGUI::Window *gameOver = wmgr.createWindow("TaharezLook/StaticText", "GameOverWindow");
    gameOver->setText("");
    gameOver->setVisible(false);
    gameOver->setSize(CEGUI::UVector2(CEGUI::UDim(0.30, 0), CEGUI::UDim(0.15, 0)));
    gameOver->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35f, 0), CEGUI::UDim(0.425f, 0 )));
 
    sheet->addChildWindow(gameOver);
    CEGUI::System::getSingleton().setGUISheet(sheet);
    
    // stuff from createFrameListener
    
    // Backgroun Music
    //sound.PlaySound("buddhist_chant.wav");
    
    srand(time(0));  // Initialize random number generator.
    // Set default values for variables
    float r1 = (rand() % 100);
    float r2 = (rand() % 100);
    float r3 = (rand() % 100);
    
}

bool SingleGameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    CEGUI::System &sys = CEGUI::System::getSingleton();
    sys.injectKeyDown(keyEventRef.key);
    sys.injectChar(keyEventRef.text);
    if (keyEventRef.key == OIS::KC_ESCAPE)
    {
        pushAppState(findByName("PauseState"));
        return true;
    }
    if (keyEventRef.key == OIS::KC_X)
    {
        SingleGameState::soundSwitch();
    }
    
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool SingleGameState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}

bool SingleGameState::mouseMoved(const OIS::MouseEvent &evt)
{
    CEGUI::System::getSingleton().injectMouseMove(evt.state.X.rel, evt.state.Y.rel);
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
    CEGUI::Vector2 guiMouse = CEGUI::MouseCursor::getSingleton().getPosition();
    deltaX = evt.state.X.rel;
    deltaY = -evt.state.Y.rel;
    //xi = evt.state.X.abs - (1024/2);
    //yi = -evt.state.Y.abs + (768/2); 
    xi += guiMouse.d_x;
    yi += -guiMouse.d_y; 
    if (xi > 90) xi = 90;
    if (xi < -90) xi = -90;
    if (yi > 90) yi = 90;
    if (yi < -90) yi = -90;
    //cout << "move: (" << deltaX << "," << deltaY << ")" << endl;
    
    if(m_bRMouseDown)
    {
        m_pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
        m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
    }
    
    return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool SingleGameState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    CEGUI::System::getSingleton().injectMouseButtonDown(SingleGameState::convertButton(id));
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
    
    switch (id) {
        case OIS::MB_Left:
            onLeftPressed(evt);
            m_bLMouseDown = true;
            //cout << "Left Click" << endl;
            if (ready){
                ready = false;
            }
            //OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();
            break;
        case OIS::MB_Right: 
            m_bRMouseDown = true;
            //cout << "Right Click" << endl;
            break;
        default:
            break;
    }

    //mCameraMan->injectMouseDown(evt, id);
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool SingleGameState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
 
    if(id == OIS::MB_Left)
    {
        m_bLMouseDown = false;
    }
    else if(id == OIS::MB_Right)
    {
        m_bRMouseDown = false;
    }
 
    return true;
}

void SingleGameState::onLeftPressed(const OIS::MouseEvent &evt)
{
    printf("%d THE WIDTH",evt.state.width);
    printf("%d THE HEIGHT",evt.state.height);
    if(m_pCurrentObject)
    {
        m_pCurrentObject->showBoundingBox(false);
    }
    CEGUI::Vector2 guiMouse = CEGUI::MouseCursor::getSingleton().getPosition();
    Ogre::Ray mouseRay = m_pCamera->getCameraToViewportRay(guiMouse.d_x / float(evt.state.width),
        guiMouse.d_y / float(evt.state.height));
    m_pRSQ->setRay(mouseRay);
    m_pRSQ->setSortByDistance(true);
 
    Ogre::RaySceneQueryResult &result = m_pRSQ->execute();
    Ogre::RaySceneQueryResult::iterator itr;
    
    for(itr = result.begin(); itr != result.end(); itr++)
    {
        if(itr->movable)
        {
            //OgreFramework::getSingletonPtr()->m_pLog->logMessage("MovableName: " + itr->movable->getName());
            m_pCurrentObject = m_pSceneMgr->getMovableObject(itr->movable->getName(), "ManualObject")->getParentSceneNode();
            //cout << "gets here" << endl;
            m_pCurrentObject->showBoundingBox(true);
            
            
            string tempLandCellId = itr->movable->getName();
            string idNumber = tempLandCellId.substr(5, tempLandCellId.length()-1);
            
            /*
            if (myfile.is_open()){
                myfile << "waterland_"<< idNumber << endl;
            }
            else cout << "Unable to open file";
            */
            
            int intId;
            istringstream(idNumber) >> intId;
            cout << tempLandCellId << " = " << earth.cells[intId].terrain << endl;
            cout << "Average color = " << earth.cells[intId].avg_color << endl;
            cout << endl;
            
            break;
        }
    }
    
}

void SingleGameState::moveCamera()
{
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
        m_pCamera->moveRelative(m_TranslateVector);
    m_pCamera->moveRelative(m_TranslateVector / 10);
}

void SingleGameState::getInput()
{
    if(m_bSettingsMode == false)
    {
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A))
            m_TranslateVector.x = -m_MoveScale;
 
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D))
            m_TranslateVector.x = m_MoveScale;
 
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W))
	    if(m_pCamera->getPosition().z > 200 || m_pCamera->getPosition().z < -200)
            	m_TranslateVector.y = m_MoveScale;
 
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S))
	    if(m_pCamera->getPosition().z > 200 || m_pCamera->getPosition().z < -200)
            	m_TranslateVector.y = -m_MoveScale;

	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_F)){
	    if(camDistance < 20000){
            	m_TranslateVector.z = m_MoveScale;
		camDistance += 100;
	    }
	}
 
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_G)){
	    if(camDistance > 7000){
            	m_TranslateVector.z = -m_MoveScale;
		camDistance -= 100;
	    }
	}

 
        //reset roll
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_C))
            m_pCamera->roll(-(m_pCamera->getRealOrientation().getRoll()));
    }
}

void SingleGameState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame / 1000.0;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
 
    if(m_bQuit == true)
    {
        popAppState();
        return;
    }
 
 
    m_MoveScale = m_MoveSpeed   * timeSinceLastFrame;
    m_RotScale  = m_RotateSpeed * timeSinceLastFrame;
 
    m_TranslateVector = Vector3::ZERO;
 
    getInput();
    moveCamera();
    
    
    
    /*
    if(mWindow->isClosed())
        return false;
    
    if(mShutDown)
        return false;
    */
    
    //Need to capture/update each device
    OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
}

void SingleGameState::buildGUI()
{
    /*
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOP, "GameLbl", "Singleplayer Game mode", 250);
    //OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();
 
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("Mode");
    
    m_pDetailsPanel = OgreFramework::getSingletonPtr()->m_pTrayMgr->createParamsPanel(OgreBites::TL_TOPLEFT, "DetailsPanel", 200, items);
    m_pDetailsPanel->show();
    
    Ogre::String infoText = "[TAB] - Switch input mode\n\n[W] - Forward / Mode up\n[S] - Backwards/ Mode down\n[A] - Left\n";
    infoText.append("[D] - Right\n\nPress [SHIFT] to move faster\n\n[O] - Toggle FPS / logo\n");
    infoText.append("[Print] - Take screenshot\n\n[ESC] - Exit");
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createTextBox(OgreBites::TL_RIGHT, "InfoPanel", infoText, 300, 220);
    
    Ogre::StringVector chatModes;
    chatModes.push_back("Solid mode");
    chatModes.push_back("Wireframe mode");
    chatModes.push_back("Point mode");
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createLongSelectMenu(OgreBites::TL_TOPRIGHT, "ChatModeSelMenu", "ChatMode", 200, 3, chatModes);
    */
}

void SingleGameState::itemSelected(OgreBites::SelectMenu* menu)
{
    switch(menu->getSelectionIndex())
    {
    case 0:
        m_pCamera->setPolygonMode(Ogre::PM_SOLID);break;
    case 1:
        m_pCamera->setPolygonMode(Ogre::PM_WIREFRAME);break;
    case 2:
        m_pCamera->setPolygonMode(Ogre::PM_POINTS);break;
    }
}

CEGUI::MouseButton SingleGameState::convertButton(OIS::MouseButtonID buttonID)
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

bool SingleGameState::soundButton(const CEGUI::EventArgs &e)
{
    SingleGameState::soundSwitch();
    return true;
}


bool SingleGameState::soundSwitch(void)
{
    CEGUI::Window* sounder = CEGUI::WindowManager::getSingleton().getWindow("SounderDontBreak");
    if(soundIs)
    {
       sound.turnOff();
       sounder->setText("Sound: Off");
    }
    else
    {
        sound.turnOn();
        sounder->setText("Sound: On");
    }
    soundIs = !soundIs;
    return true;
} 

bool SingleGameState::CommandBaseButton(const CEGUI::EventArgs &e)
{
	return true;
}
bool SingleGameState::ArmyBaseButton(const CEGUI::EventArgs &e)
{
	return true;
}
bool SingleGameState::NavyBaseButton(const CEGUI::EventArgs &e)
{
	return true;
}
bool SingleGameState::AirForceBaseButton(const CEGUI::EventArgs &e)
{
	return true;
}
bool SingleGameState::InfantryButton(const CEGUI::EventArgs &e)
{
	return true;
}
bool SingleGameState::TankButton(const CEGUI::EventArgs &e)
{
	return true;
}
bool SingleGameState::ScudButton(const CEGUI::EventArgs &e)
{
	return true;
}
bool SingleGameState::SubmarineButton(const CEGUI::EventArgs &e)
{
	return true;
}
bool SingleGameState::DestroyerButton(const CEGUI::EventArgs &e)
{
	return true;
}
bool SingleGameState::BomberButton(const CEGUI::EventArgs &e)
{
	return true;
}
bool SingleGameState::FighterButton(const CEGUI::EventArgs &e)
{
	return true;
}

void BuildingImages1(void)
{
	CEGUI::Window* w = CEGUI::WindowManager::getSingleton().getWindow("CommandBase");
	w->setVisible( true );
	w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
	w->setVisible( true );
	w = CEGUI::WindowManager::getSingleton().getWindow("NavyBase");
	w->setVisible( true );
	w = CEGUI::WindowManager::getSingleton().getWindow("AirForceBase");
	w->setVisible( true );
	w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
	w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
	w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
	w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
	w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
	buildingImages = 1;
}
void BuildingImagesCB2()
{
	CEGUI::Window* w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
	buildingImages = 2;
}
void BuildingImagesA3()
{
	CEGUI::Window* w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
	w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
	buildingImages = 3;
}
void BuildingImagesN4()
{
	CEGUI::Window* w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
	w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
	buildingImages = 4;
}
void BuildingImagesAF5()
{
	CEGUI::Window* w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
	w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
	buildingImages = 5;
}
