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
    buildingImages = 1;
    myOwner = Owner_BLUE;
    money = 100000000;
    plutonium = 100000000;
    population = 0;
    onButton = false;
    showWaterLand = false;
    acceptNeighbors[6] = {0};
    unitBuilding = 0;
    unitMoney = 0;
    unitPlutonium = 0; 
    unitSelected = NULL;
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
                    4);
    earth.init();
    earth.mapTerrain();
    earth.mapFaction();
    earth.mapPopDensity();
    earth.createManualObjects(m_pSceneMgr);
    earth.createManualObjects2(m_pSceneMgr);
    
    
    for (int i=0;i<earth.cells.size();i++){
        string s = "Cell_";
        stringstream ss;
        Ogre::String name;
        ss << i;
        name = s.append(ss.str());
        if (myOwner == Owner_BLUE){
            if (earth.cells[i].owner == Owner_BLUE){
                earth.own(m_pSceneMgr, earth.cells[i]);
            }
        }
        else if (myOwner == Owner_RED){
            if (earth.cells[i].owner == Owner_RED){
                earth.own(m_pSceneMgr, earth.cells[i]);
            }
        }
    }
    
    earth.createBorderSegments(m_pSceneMgr, 50);
    earth.updateBorderSegments(m_pSceneMgr);

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
    progressBar->setProgress(progressBar->getProgress() + 1.0f);
    CEGUI::Window *resultWindow = wmgr.createWindow("TaharezLook/StaticText", "ProgressText1");
    sheet->addChildWindow(resultWindow);
    resultWindow->setHorizontalAlignment(CEGUI::HA_CENTRE);
    resultWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.04f, 0)));
    if(myOwner == Owner_BLUE)
        resultWindow->setText("[colour='FF0000FF']Player Command Bases Remaining: 10/10");
    else
        resultWindow->setText("[colour='FFFF0000']Player Command Bases Remaining: 10/10");
    resultWindow->setProperty("FrameEnabled", "false");
    resultWindow->setProperty("BackgroundEnabled", "false");
    resultWindow->setProperty("HorzFormatting", "WordWrapCentred");

    progressBar = static_cast<CEGUI::ProgressBar*>(wmgr.createWindow("TaharezLook/ProgressBar", "ProgressBar2"));
    sheet->addChildWindow(progressBar);
    progressBar->setSize(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.04f, 0)));
    progressBar->setHorizontalAlignment(CEGUI::HA_CENTRE);
    progressBar->setYPosition(CEGUI::UDim(0.05f, 0));
    progressBar->setProgress(progressBar->getProgress() + 1.0f);
    resultWindow = wmgr.createWindow("TaharezLook/StaticText", "ProgressText2");
    sheet->addChildWindow(resultWindow);
    resultWindow->setHorizontalAlignment(CEGUI::HA_CENTRE);
    resultWindow->setYPosition(CEGUI::UDim(0.05f, 0));
    resultWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.04f, 0)));
    if(myOwner == Owner_BLUE)
        resultWindow->setText("[colour='FFFF0000']Enemy Command Bases Remaining: 10/10");
    else
        resultWindow->setText("[colour='FF0000FF']Enemy Command Bases Remaining: 10/10");
    resultWindow->setProperty("FrameEnabled", "false");
    resultWindow->setProperty("BackgroundEnabled", "false");
    resultWindow->setProperty("HorzFormatting", "WordWrapCentred");

    CEGUI::Window *w;
    if(myOwner == Owner_BLUE)
    {
        CEGUI::ImagesetManager::getSingleton().create( "BCommandBase.imageset" );
        w = wmgr.loadWindowLayout("BCommandBase.layout");
    }
    else
    {
        CEGUI::ImagesetManager::getSingleton().create( "RCommandBase.imageset" );
        w = wmgr.loadWindowLayout("RCommandBase.layout");
    }
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( true );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::CommandBaseButton,this));
    w->disable();

    sheet->addChildWindow(w);

    CEGUI::ImagesetManager::getSingleton().create( "ArmyBase.imageset" );
    w = wmgr.loadWindowLayout("ArmyBase.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.20f, 0));
    w->setVisible( true );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::ArmyBaseButton,this));
    w->disable();

    sheet->addChildWindow(w);

    CEGUI::ImagesetManager::getSingleton().create( "NavyBase.imageset" );
    w = wmgr.loadWindowLayout("NavyBase.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.35f, 0));
    w->setVisible( true );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::NavyBaseButton,this));
    w->disable();

    sheet->addChildWindow(w);

    CEGUI::ImagesetManager::getSingleton().create( "AirForceBase.imageset" );
    w = wmgr.loadWindowLayout("AirForceBase.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.50f, 0));
    w->setVisible( true );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::AirForceBaseButton,this));
    w->disable();

    sheet->addChildWindow(w);
    
    CEGUI::ImagesetManager::getSingleton().create( "NuclearPlant.imageset" );
    w = wmgr.loadWindowLayout("NuclearPlant.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.65f, 0));
    w->setVisible( true );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::ICBMSiloButton,this));
    w->disable();

    sheet->addChildWindow(w);

    CEGUI::ImagesetManager::getSingleton().create( "ICBMSilo.imageset" );
    w = wmgr.loadWindowLayout("ICBMSilo.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.80f, 0));
    w->setVisible( true );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::ICBMSiloButton,this));
    w->disable();

    sheet->addChildWindow(w);
    
    CEGUI::ImagesetManager::getSingleton().create( "Infantry.imageset" );
    w = wmgr.loadWindowLayout("Infantry.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::InfantryButton,this));
    w->disable();

    sheet->addChildWindow(w);
    
    CEGUI::ImagesetManager::getSingleton().create( "Submarine.imageset" );
    w = wmgr.loadWindowLayout("Submarine.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::SubmarineButton,this));
    w->disable();

    sheet->addChildWindow(w);    
    
    CEGUI::ImagesetManager::getSingleton().create( "Destroyer.imageset" );
    w = wmgr.loadWindowLayout("Destroyer.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.20f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::DestroyerButton,this));
    w->disable();

    sheet->addChildWindow(w);
    
    CEGUI::ImagesetManager::getSingleton().create( "Tank.imageset" );
    w = wmgr.loadWindowLayout("Tank.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::TankButton,this));
    w->disable();

    sheet->addChildWindow(w);
    
    CEGUI::ImagesetManager::getSingleton().create( "Scud.imageset" );
    w = wmgr.loadWindowLayout("Scud.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.20f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::ScudButton,this));
    w->disable();

    sheet->addChildWindow(w);

    CEGUI::ImagesetManager::getSingleton().create( "Bomber.imageset" );
    w = wmgr.loadWindowLayout("Bomber.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::BomberButton,this));
    w->disable();

    sheet->addChildWindow(w);
    
    CEGUI::ImagesetManager::getSingleton().create( "Fighter.imageset" );
    w = wmgr.loadWindowLayout("Fighter.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.20f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::FighterButton,this));
    w->disable();

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
    if (keyEventRef.key == OIS::KC_Z)
    {
        if(showWaterLand) {
            showWaterLand = false;
            for (int i=0;i<earth.cells.size();i++){
                string s = "CellBoundary_";
                stringstream ss;
                Ogre::String name;
                ss << i;
                name = s.append(ss.str());
                m_pSceneMgr->getEntity(name)->setVisible(true);
            }
        }
        else {
            showWaterLand = true;
            for (int i=0;i<earth.cells.size();i++){
                string s = "CellBoundary_";
                stringstream ss;
                Ogre::String name;
                ss << i;
                name = s.append(ss.str());
                m_pSceneMgr->getEntity(name)->setVisible(false);
            }
        }
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
    if(onButton)
    {
        onButton = false;
        return true;
    }
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
    
    //mCameraMan->injectMouseDown(evt, id);
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool SingleGameState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
    if(onButton)
    {
        onButton = false;
        return true;
    }
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
 
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
            onRightPressed(evt);
            m_bRMouseDown = true;
            //cout << "Right Click" << endl;
            break;
        default:
            break;
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
        //m_pCurrentObject = NULL;
        //m_pCurrentEntity = NULL;
        cout << "deselect" << endl;
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
            string name = itr->movable->getName();
            string idType = name.substr(0, 5);
            
            if(idType == "Unit_"){ // Select a unit.
                if(unitSelected){
                    PlanetCell& start = earth.cells[unitSelected->currentCellId];
                    for (int i=0;i<start.neighbors.size();i++){
                        earth.changeMaterial(m_pSceneMgr, earth.cells[start.neighbors[i]], earth.cells[start.neighbors[i]].baseMaterial);
                    }
                }
                
                m_pCurrentObject = m_pSceneMgr->getEntity(name)->getParentSceneNode(); 
                m_pCurrentObject->showBoundingBox(true); 
                m_pCurrentEntity = m_pSceneMgr->getEntity(name); 
                
                string idNumber = name.substr(5, name.length()-1);
                istringstream(idNumber) >> intId;
                unitSelected = &(units[intId]);
                PlanetCell& currentCell = earth.cells[unitSelected->currentCellId]; 
                
                if(unitSelected->myType == Unit_COMMANDBASE)
                    BuildingImagesCB2(currentCell);
                else if(unitSelected->myType == Unit_ARMYBASE)
                    BuildingImagesA3(currentCell);
                else if(unitSelected->myType == Unit_NAVYBASE)
                    BuildingImagesN4(currentCell);
                else if(unitSelected->myType == Unit_AIRFORCEBASE)
                    BuildingImagesAF5(currentCell);
                else{
                    BuildingImages1(currentCell);
                    
                    if(unitSelected->myType >= 1 && unitSelected->myType <= 5){ //land unit
                        for (int i=0;i<currentCell.neighbors.size();i++){
                            PlanetCell& temp = earth.cells[currentCell.neighbors[i]];
                            if (temp.terrain != Terrain_WATER && temp.myUnit == Unit_EMPTY){
                                acceptNeighbors[i] = currentCell.neighbors[i];
                                earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
                            }
                            else{
                                acceptNeighbors[i] = 0;
                                earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
                            }
                        }
                    }
                    else if(unitSelected->myType >= 6 && unitSelected->myType <= 9){ //naval unit
                        for (int i=0;i<currentCell.neighbors.size();i++){
                            PlanetCell& temp = earth.cells[currentCell.neighbors[i]];
                            if (temp.terrain != Terrain_LAND && temp.myUnit == Unit_EMPTY){
                                acceptNeighbors[i] = currentCell.neighbors[i];
                                earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
                            }
                            else{
                                acceptNeighbors[i] = 0;
                                earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
                            }
                        }
                    }
                    else if(unitSelected->myType >= 10 && unitSelected->myType <= 12){ //air unit
                        for (int i=0;i<currentCell.neighbors.size();i++){
                            PlanetCell& temp = earth.cells[currentCell.neighbors[i]];
                            acceptNeighbors[i] = currentCell.neighbors[i];
                            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
                        }
                    }
                }
                
                
                break;
            }
            if(idType == "Cell_"){ // Select a cell.
                
                string idNumber = name.substr(5, name.length()-1); 
                istringstream(idNumber) >> intId; 
                PlanetCell& currentCell = earth.cells[intId]; 
                
                if(unitBuilding){
                    SingleGameState::unitCreate(currentCell);
                    unitBuilding = 0;
                }
                
                else if(unitSelected){
                    if(currentCell.myUnit == Unit_EMPTY && unitSelected->myType >= 1 && unitSelected->myType <= 12){
                        SingleGameState::unitMove(unitSelected, currentCell);
                    }
                }
                
                else{
                    m_pCurrentObject = m_pSceneMgr->getEntity(name)->getParentSceneNode();
                    m_pCurrentObject->showBoundingBox(true);
                    m_pCurrentEntity = m_pSceneMgr->getEntity(name);
                }
                
                
                
                if(currentCell.myUnit == Unit_EMPTY)
                    BuildingImages1(currentCell);
                /*
                else if(currentCell.myUnit == Unit_COMMANDBASE)
                    BuildingImagesCB2(currentCell);
                else if(currentCell.myUnit == Unit_ARMYBASE)
                    BuildingImagesA3(currentCell);
                else if(currentCell.myUnit == Unit_NAVYBASE)
                    BuildingImagesN4(currentCell);
                else if(currentCell.myUnit == Unit_AIRFORCEBASE)
                    BuildingImagesAF5(currentCell);
                else{
                    BuildingImages1(currentCell);
                }
                */
                /*
                cout << tempLandCellId << " = " << earth.cells[intId].terrain << endl;
                cout << "Average color = " << earth.cells[intId].avg_color << endl;
                cout << "Longitude = " << earth.cells[intId].longitude << ", latitude = " << earth.cells[intId].latitude << endl;
                cout << endl;
                */
                break;
            }
        }
    }
}

void SingleGameState::onRightPressed(const OIS::MouseEvent &evt){
    if(m_pCurrentObject){
        if(unitBuilding){
            string name = m_pCurrentEntity->getName();
            string idNumber = name.substr(5, name.length()-1);
            istringstream(idNumber) >> intId;
            earth.own(m_pSceneMgr, earth.cells[units[intId].currentCellId]);
            unitBuilding = 0;
        }
        m_pCurrentObject->showBoundingBox(false);
        m_pCurrentObject = NULL;
        m_pCurrentEntity = NULL;
        cout << "deselect" << endl;
        unitSelected = NULL;
        BuildingImages1(earth.cells[intId]);
    }
}

void SingleGameState::moveCamera()
{
    float z2 = m_pCamera->getPosition().z * m_pCamera->getPosition().z;
    float y2 = m_pCamera->getPosition().y * m_pCamera->getPosition().y;
    float x2 = m_pCamera->getPosition().x * m_pCamera->getPosition().x;
    float distance = sqrt(x2 + y2 + z2);
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_LSHIFT)) //4x speed
        m_pCamera->moveRelative(m_TranslateVector / 2.5);
    else m_pCamera->moveRelative(m_TranslateVector / 10);
    z2 = m_pCamera->getPosition().z * m_pCamera->getPosition().z;
    y2 = m_pCamera->getPosition().y * m_pCamera->getPosition().y;
    x2 = m_pCamera->getPosition().x * m_pCamera->getPosition().x;
    float newdistance = sqrt(x2 + y2 + z2);
    float shift = newdistance - distance;
    m_pCamera->lookAt(0,0,0);
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_F)){
	    if(distance >= 11000){
	    	m_pCamera->moveRelative(m_TranslateVector/10);
	    }
    }
    else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_G)){
	    if(distance <= 20000){
	    	m_pCamera->moveRelative(m_TranslateVector/10);
	    }
    }
    m_TranslateVector = Vector3::ZERO;
    m_TranslateVector.z = -shift;
    m_pCamera->moveRelative(m_TranslateVector);
}

void SingleGameState::getInput()
{
    if(m_bSettingsMode == false)
    {
	float z2 = m_pCamera->getPosition().z * m_pCamera->getPosition().z;
	float y2 = m_pCamera->getPosition().y * m_pCamera->getPosition().y;
	float x2 = m_pCamera->getPosition().x * m_pCamera->getPosition().x;
	float distance = sqrt(x2 + y2 + z2);
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A)){
            m_TranslateVector.x = -m_MoveScale;
	}
 
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D)){
            m_TranslateVector.x = m_MoveScale;
	}
 
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W)){
		if(distance - m_pCamera->getPosition().y > 1000)
                	m_TranslateVector.y = m_MoveScale;

	}
 
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S)){
		if(distance + m_pCamera->getPosition().y > 1000)
			m_TranslateVector.y = -m_MoveScale;
	}

	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_G)){
	    if(distance < 20000){
		m_TranslateVector.z = m_MoveScale;
	    }
	}
	 
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_F)){

            if(distance > 11000){
                m_TranslateVector.z = -m_MoveScale;
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
    onButton = true;
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
    string name = m_pCurrentEntity->getName();
    string idNumber = name.substr(5, name.length()-1);
    istringstream(idNumber) >> intId;
    PlanetCell& currentCell = earth.cells[intId]; 
    
    currentCell.myUnit = Unit_COMMANDBASE;
    money -= Au_COMMANDBASE;
    plutonium -= Pt_COMMANDBASE;
    Unit newUnit(Owner_BLUE, Unit_COMMANDBASE);
    newUnit.currentCellId = intId; //critical
    newUnit.createManualObject(m_pSceneMgr);
    newUnit.relocate(earth.vertices[intId]);
    units.push_back(newUnit);
    BuildingImages1(currentCell); 
    onButton = true;
    return true;
}
bool SingleGameState::ArmyBaseButton(const CEGUI::EventArgs &e)
{
    string name = m_pCurrentEntity->getName();
    string idNumber = name.substr(5, name.length()-1);
    istringstream(idNumber) >> intId;
    PlanetCell& currentCell = earth.cells[intId]; 
    
    currentCell.myUnit = Unit_ARMYBASE;
    money -= Au_ARMYBASE;
    plutonium -= Pt_ARMYBASE;
    Unit newUnit(Owner_BLUE, Unit_ARMYBASE);
    newUnit.currentCellId = intId; //critical
    newUnit.createManualObject(m_pSceneMgr);
    newUnit.relocate(earth.vertices[intId]); 
    units.push_back(newUnit);
    BuildingImages1(currentCell);
    onButton = true;
    return true;
}
bool SingleGameState::NavyBaseButton(const CEGUI::EventArgs &e)
{
    string name = m_pCurrentEntity->getName();
    string idNumber = name.substr(5, name.length()-1);
    istringstream(idNumber) >> intId;
    PlanetCell& currentCell = earth.cells[intId]; 
    
    currentCell.myUnit = Unit_NAVYBASE;
    money -= Au_NAVYBASE;
    plutonium -= Pt_NAVYBASE;
    Unit newUnit(Owner_BLUE, Unit_NAVYBASE);
    newUnit.currentCellId = intId; //critical
    newUnit.createManualObject(m_pSceneMgr);
    newUnit.relocate(earth.vertices[intId]);
    units.push_back(newUnit);
    BuildingImages1(currentCell);
    onButton = true;
    return true;
}
bool SingleGameState::AirForceBaseButton(const CEGUI::EventArgs &e)
{
    string name = m_pCurrentEntity->getName();
    string idNumber = name.substr(5, name.length()-1);
    istringstream(idNumber) >> intId;
    PlanetCell& currentCell = earth.cells[intId]; 
    
    currentCell.myUnit = Unit_AIRFORCEBASE;
    money -= Au_AIRFORCEBASE;
    plutonium -= Pt_AIRFORCEBASE;
    Unit newUnit(Owner_BLUE, Unit_AIRFORCEBASE);
    newUnit.currentCellId = intId; //critical
    newUnit.createManualObject(m_pSceneMgr);
    newUnit.relocate(earth.vertices[intId]); 
    units.push_back(newUnit);
    BuildingImages1(currentCell);
    onButton = true;
    return true;
}
bool SingleGameState::NuclearPlantButton(const CEGUI::EventArgs &e)
{
    string name = m_pCurrentEntity->getName();
    string idNumber = name.substr(5, name.length()-1);
    istringstream(idNumber) >> intId;
    PlanetCell& currentCell = earth.cells[intId]; 
    
    currentCell.myUnit = Unit_NUCLEARPLANT;
    money -= Au_NUCLEARPLANT;
    plutonium -= Pt_NUCLEARPLANT;
    Unit newUnit(Owner_BLUE, Unit_NUCLEARPLANT);
    newUnit.currentCellId = intId; //critical
    newUnit.createManualObject(m_pSceneMgr);
    newUnit.relocate(earth.vertices[intId]); 
    units.push_back(newUnit);
    BuildingImages1(currentCell); 
    onButton = true;
    return true;
}
bool SingleGameState::ICBMSiloButton(const CEGUI::EventArgs &e)
{
    string name = m_pCurrentEntity->getName();
    string idNumber = name.substr(5, name.length()-1);
    istringstream(idNumber) >> intId;
    PlanetCell& currentCell = earth.cells[intId]; 
    
    currentCell.myUnit = Unit_ICBMSILO;
    money -= Au_ICBMSILO;
    plutonium -= Pt_ICBMSILO;
    Unit newUnit(Owner_BLUE, Unit_ICBMSILO);
    newUnit.currentCellId = intId; //critical
    newUnit.createManualObject(m_pSceneMgr);
    newUnit.relocate(earth.vertices[intId]);
    units.push_back(newUnit);
    BuildingImages1(currentCell);
    onButton = true;
    return true;
}
bool SingleGameState::InfantryButton(const CEGUI::EventArgs &e)
{
    string name = m_pCurrentEntity->getName();
    string idNumber = name.substr(5, name.length()-1);
    istringstream(idNumber) >> intId;
    unitSelected = &(units[intId]);
    PlanetCell& currentCell = earth.cells[unitSelected->currentCellId]; 
    
    for (int i=0;i<currentCell.neighbors.size();i++){
        PlanetCell& temp = earth.cells[currentCell.neighbors[i]];
        if (temp.terrain != Terrain_WATER && temp.myUnit == Unit_EMPTY)
        {
            acceptNeighbors[i] = currentCell.neighbors[i];
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
        }
        else
        {
            acceptNeighbors[i] = 0;
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
        }
    }
    unitBuilding = Unit_INFANTRY;
    unitMoney = Au_INFANTRY;
    unitPlutonium = Pt_INFANTRY;
    onButton = true;
    return true;
}
bool SingleGameState::TankButton(const CEGUI::EventArgs &e)
{
    string name = m_pCurrentEntity->getName();
    string idNumber = name.substr(5, name.length()-1);
    istringstream(idNumber) >> intId;
    unitSelected = &(units[intId]);
    PlanetCell& currentCell = earth.cells[unitSelected->currentCellId]; 
    
    for (int i=0;i<currentCell.neighbors.size();i++){
        PlanetCell& temp = earth.cells[currentCell.neighbors[i]];
        if (temp.terrain != Terrain_WATER && temp.myUnit == Unit_EMPTY)
        {
            acceptNeighbors[i] = currentCell.neighbors[i];
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
        }
        else
        {
            acceptNeighbors[i] = 0;
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
        }
    }
    unitBuilding = Unit_TANK;
    unitMoney = Au_TANK;
    unitPlutonium = Pt_TANK;
    onButton = true;
    return true;
}
bool SingleGameState::ScudButton(const CEGUI::EventArgs &e)
{
    string name = m_pCurrentEntity->getName();
    string idNumber = name.substr(5, name.length()-1);
    istringstream(idNumber) >> intId;
    unitSelected = &(units[intId]);
    PlanetCell& currentCell = earth.cells[unitSelected->currentCellId]; 
    
    for (int i=0;i<currentCell.neighbors.size();i++){
        PlanetCell& temp = earth.cells[currentCell.neighbors[i]];
        if (temp.terrain != Terrain_WATER && temp.myUnit == Unit_EMPTY)
        {
            acceptNeighbors[i] = currentCell.neighbors[i];
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
        }
        else
        {
            acceptNeighbors[i] = 0;
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
        }
    }
    unitBuilding = Unit_SCUD;
    unitMoney = Au_SCUD;
    unitPlutonium = Pt_SCUD;
    onButton = true;
    return true;
}
bool SingleGameState::SubmarineButton(const CEGUI::EventArgs &e)
{
    string name = m_pCurrentEntity->getName();
    string idNumber = name.substr(5, name.length()-1);
    istringstream(idNumber) >> intId;
    PlanetCell& currentCell = earth.cells[units[intId].currentCellId];
    
    for (int i=0;i<currentCell.neighbors.size();i++){
        PlanetCell& temp = earth.cells[currentCell.neighbors[i]];
        if (temp.terrain != Terrain_LAND && temp.myUnit == Unit_EMPTY)
        {
            acceptNeighbors[i] = currentCell.neighbors[i];
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
        }
        else
        {
            acceptNeighbors[i] = 0;
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
        }
    }
    unitBuilding = Unit_SUBMARINE;
    unitMoney = Au_SUBMARINE;
    unitPlutonium = Pt_SUBMARINE;
    onButton = true;
    return true;
}
bool SingleGameState::DestroyerButton(const CEGUI::EventArgs &e)
{
    string name = m_pCurrentEntity->getName();
    string idNumber = name.substr(5, name.length()-1);
    istringstream(idNumber) >> intId;
    PlanetCell& currentCell = earth.cells[units[intId].currentCellId];
    
    for (int i=0;i<currentCell.neighbors.size();i++){
        PlanetCell& temp = earth.cells[currentCell.neighbors[i]];
        if (temp.terrain != Terrain_LAND && temp.myUnit == Unit_EMPTY)
        {
            acceptNeighbors[i] = currentCell.neighbors[i];
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
        }
        else
        {
            acceptNeighbors[i] = 0;
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
        }
    }
    unitBuilding = Unit_DESTROYER;
    unitMoney = Au_DESTROYER;
    unitPlutonium = Pt_DESTROYER;
    onButton = true;
    return true;
}
bool SingleGameState::BomberButton(const CEGUI::EventArgs &e)
{
    string name = m_pCurrentEntity->getName();
    string idNumber = name.substr(5, name.length()-1);
    istringstream(idNumber) >> intId;
    PlanetCell& currentCell = earth.cells[units[intId].currentCellId];
    
    for (int i=0;i<currentCell.neighbors.size();i++){
        PlanetCell& temp = earth.cells[currentCell.neighbors[i]];
        if (temp.myUnit == Unit_EMPTY)
        {
            acceptNeighbors[i] = currentCell.neighbors[i];
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
        }
        else
        {
            acceptNeighbors[i] = 0;
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
        }
    }
    unitBuilding = Unit_BOMBER;
    unitMoney = Au_BOMBER;
    unitPlutonium = Pt_BOMBER;
    onButton = true;
    return true;
}
bool SingleGameState::FighterButton(const CEGUI::EventArgs &e)
{
    string name = m_pCurrentEntity->getName();
    string idNumber = name.substr(5, name.length()-1);
    istringstream(idNumber) >> intId;
    PlanetCell& currentCell = earth.cells[units[intId].currentCellId];
    
    for (int i=0;i<currentCell.neighbors.size();i++){
        PlanetCell& temp = earth.cells[currentCell.neighbors[i]];
        if (temp.myUnit == Unit_EMPTY)
        {
            acceptNeighbors[i] = currentCell.neighbors[i];
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
        }
        else
        {
            acceptNeighbors[i] = 0;
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
        }
    }
    unitBuilding = Unit_FIGHTER;
    unitMoney = Au_FIGHTER;
    unitPlutonium = Pt_FIGHTER;
    onButton = true;
    return true;
}

void SingleGameState::BuildingImages1(PlanetCell &cell)
{
    CEGUI::Window* w;
    if(buildingImages != 1)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("CommandBase");
        w->setVisible( true );
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
        w->setVisible( true );
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("NavyBase");
        w->setVisible( true );
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("AirForceBase");
        w->setVisible( true );
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("NuclearPlant");
        w->setVisible( true );
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("ICBMSilo");
        w->setVisible( true );
        w->enable();
        if(buildingImages == 2)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("Infantry");
            w->setVisible( false );
        }
        else if(buildingImages == 3)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("Tank");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("Scud");
            w->setVisible( false );
        }
        else if(buildingImages == 4)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("Submarine");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("Destroyer");
            w->setVisible( false );
        }
        else if(buildingImages == 5)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("Bomber");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("Fighter");
            w->setVisible( false );
        }
        buildingImages = 1;
    }
    else
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("CommandBase");
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("NavyBase");
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("AirForceBase");
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("NuclearPlant");
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("ICBMSilo");
        w->enable();
    }

    if(cell.owner != myOwner || cell.terrain == Terrain_WATER || cell.myUnit != 0)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("CommandBase");
        w->disable();
        w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
        w->disable();
        w = CEGUI::WindowManager::getSingleton().getWindow("NavyBase");
        w->disable();
        w = CEGUI::WindowManager::getSingleton().getWindow("AirForceBase");
        w->disable();
        w = CEGUI::WindowManager::getSingleton().getWindow("NuclearPlant");
        w->disable();
        w = CEGUI::WindowManager::getSingleton().getWindow("ICBMSilo");
        w->disable();
    }
    else
    {

        if(money < Au_COMMANDBASE || plutonium < Pt_COMMANDBASE)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("CommandBase");
            w->disable();    
        }
        if(money < Au_ARMYBASE || plutonium < Pt_ARMYBASE)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
            w->disable();    
        }
        if(money < Au_AIRFORCEBASE || plutonium < Pt_AIRFORCEBASE)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("AirForceBase");
            w->disable();    
        }
        if(money < Au_NUCLEARPLANT || plutonium < Pt_NUCLEARPLANT)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("NuclearPlant");
            w->disable();    
        }
        if(money < Au_ICBMSILO || plutonium < Pt_ICBMSILO)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("ICBMSilo");
            w->disable();    
        }
        if(cell.terrain == Terrain_LAND || money < Au_NAVYBASE || plutonium < Pt_NAVYBASE)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("NavyBase");
            w->disable();    
        }
    }
}
void SingleGameState::BuildingImagesCB2(PlanetCell &cell)
{
    CEGUI::Window* w;
    if(buildingImages != 2)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Infantry");
        w->setVisible( true );    
        w->enable();
        if(buildingImages == 1)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("CommandBase");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("NavyBase");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("AirForceBase");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("NuclearPlant");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("ICBMSilo");
            w->setVisible( false );
        }
        else if(buildingImages == 3)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("Tank");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("Scud");
            w->setVisible( false );
        }
        else if(buildingImages == 4)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("Submarine");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("Destroyer");
            w->setVisible( false );
        }
        else if(buildingImages == 5)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("Bomber");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("Fighter");
            w->setVisible( false );
        }
        buildingImages = 2;
    }
    else
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Infantry");
        w->enable();
    }
    if(money < Au_INFANTRY || plutonium < Pt_INFANTRY)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Infantry");
        w->disable();
    }
}
void SingleGameState::BuildingImagesA3(PlanetCell &cell)
{
    CEGUI::Window* w;
    if(buildingImages != 3)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Tank");
        w->setVisible( true );
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("Scud");
        w->setVisible( true );    
        w->enable();
        if(buildingImages == 1)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("CommandBase");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("NavyBase");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("AirForceBase");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("NuclearPlant");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("ICBMSilo");
            w->setVisible( false );
        }
        else if(buildingImages == 2)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("Infantry");
            w->setVisible( false );
        }
        else if(buildingImages == 4)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("Submarine");
            w->setVisible( false );    
            w = CEGUI::WindowManager::getSingleton().getWindow("Destroyer");
            w->setVisible( false );
        }
        else if(buildingImages == 5)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("Bomber");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("Fighter");
            w->setVisible( false );
        }
        buildingImages = 3;
    }
    else
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Tank");
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("Scud");
        w->enable();
    }
    if(money < Au_TANK || plutonium < Pt_TANK)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Tank");
        w->disable();
    }
    if(money < Au_SCUD || plutonium < Pt_SCUD)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Scud");
        w->disable();
    }
}
void SingleGameState::BuildingImagesN4(PlanetCell &cell)
{
    CEGUI::Window* w;
    if(buildingImages != 4)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Submarine");
        w->setVisible( true );
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("Destroyer");
        w->setVisible( true );    
        w->enable();
        if(buildingImages == 1)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("CommandBase");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("NavyBase");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("AirForceBase");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("NuclearPlant");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("ICBMSilo");
            w->setVisible( false );
        }
        else if(buildingImages == 2)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("Infantry");
            w->setVisible( false );
        }
        else if(buildingImages == 3)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("Tank");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("Scud");
            w->setVisible( false );
        }
        else if(buildingImages == 5)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("Bomber");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("Fighter");
            w->setVisible( false );
        }
        buildingImages = 4;
    }
    else
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Submarine");
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("Destroyer");
        w->enable();
    }
    if(money < Au_SUBMARINE || plutonium < Pt_SUBMARINE)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Submarine");
        w->disable();
    }
    if(money < Au_DESTROYER || plutonium < Pt_DESTROYER)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Destroyer");
        w->disable();
    }
}
void SingleGameState::BuildingImagesAF5(PlanetCell &cell)
{
    CEGUI::Window* w;
    if(buildingImages != 5)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Bomber");
        w->setVisible( true );
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("Fighter");
        w->setVisible( true );
        w->enable();
        if(buildingImages == 1)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("CommandBase");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("NavyBase");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("AirForceBase");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("NuclearPlant");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("ICBMSilo");
            w->setVisible( false );
        }
        else if(buildingImages == 2)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("Infantry");
            w->setVisible( false );
        }
        else if(buildingImages == 3)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("Tank");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("Scud");
            w->setVisible( false );
        }
        else if(buildingImages == 4)
        {
            w = CEGUI::WindowManager::getSingleton().getWindow("Submarine");
            w->setVisible( false );
            w = CEGUI::WindowManager::getSingleton().getWindow("Destroyer");
            w->setVisible( false );
        }
        buildingImages = 5;
    }
    else
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Bomber");
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("Fighter");
        w->enable();
    }
    if(money < Au_BOMBER || plutonium < Pt_BOMBER)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Bomber");
        w->disable();
    }
    if(money < Au_FIGHTER || plutonium < Pt_FIGHTER)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Fighter");
        w->disable();
    }
}

void SingleGameState::unitCreate(PlanetCell &cell)
{
    string name = m_pCurrentEntity->getName();
    string idNumber = name.substr(5, name.length()-1);
    istringstream(idNumber) >> intId;
    PlanetCell& currentCell = earth.cells[units[intId].currentCellId];
    
    for (int i=0;i<currentCell.neighbors.size();i++){
        if (cell.id == acceptNeighbors[i]){
            cell.myUnit = unitBuilding;
            money -= unitMoney;
            plutonium -= unitPlutonium;
            Unit newUnit(Owner_BLUE, unitBuilding);
            if(unitBuilding == Unit_SCUD){
                newUnit.createObject(m_pSceneMgr, "missile.mesh", "SolidColour");
            }
            else if(unitBuilding == Unit_FIGHTER){
                newUnit.createObject(m_pSceneMgr, "fighter.mesh", "SolidColour");
            }
            else{
                newUnit.createManualObject(m_pSceneMgr);
            }
            newUnit.currentCellId = cell.id; //critical
            newUnit.relocate(earth.vertices[cell.id]);
            units.push_back(newUnit);
            break;
        }
    }
    earth.own(m_pSceneMgr, currentCell);
}

void SingleGameState::unitMove(Unit* unit, PlanetCell& targetCell){
    PlanetCell& start = earth.cells[unit->currentCellId];
    for (int i=0;i<start.neighbors.size();i++){
        if (targetCell.id == acceptNeighbors[i]){
            unit->relocate(earth.vertices[targetCell.id]);
            unit->currentCellId = targetCell.id; //critical
            start.myUnit = 0;
            targetCell.myUnit = unit->myType;
            for (int j=0;j<start.neighbors.size();j++){
                earth.changeMaterial(m_pSceneMgr, earth.cells[start.neighbors[j]], earth.cells[start.neighbors[j]].baseMaterial);
            }
            earth.changeMaterial(m_pSceneMgr, targetCell, "MyMaterials/earth_day");
            for (int j=0;j<targetCell.neighbors.size();j++){
                earth.changeMaterial(m_pSceneMgr, earth.cells[targetCell.neighbors[j]], "MyMaterials/earth_day");
            }
            break;
        }
    }
    
    //unitSelected = NULL;
}

