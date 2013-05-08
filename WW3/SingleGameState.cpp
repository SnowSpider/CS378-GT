//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "SingleGameState.hpp"

#include <ctime>    // fortime()
#include <cstdlib>  // forsrand() and rand()

#include <iostream>
#include <sstream>
#include <math.h>
#include <X11/Xlib.h>

#include <btBulletCollisionCommon.h>

#include <string.h>

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
    myEnemy = Owner_RED;
    commandBaseMax = 5;
    commandBaseTotal = 0;
    myCommandBase = 0;
    enemyCommandBase = 5;
    money = 100000000;
    plutonium = 100000000;
    population = 0;
    onButton = false;
    showWaterLand = false;
    acceptNeighbors[6] = {0};
    unitBuilding = 0;
    unitMoving = 0;
    unitMoney = 0;
    unitPlutonium = 0; 
    unitTimer = 0;
    currentCell = NULL;
    lastCell = NULL;
    timerup = false;
    tutorial = 0;
    std::time(&currentTime);
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
    
    
    for(int i=0;i<earth.cells.size();i++){
        string s = "Cell_";
        stringstream ss;
        Ogre::String name;
        ss << i;
        name = s.append(ss.str());
        if(myOwner == Owner_BLUE){
            if(earth.cells[i].owner == Owner_BLUE){
                earth.own(m_pSceneMgr, earth.cells[i]);
            }
        }
        else if(myOwner == Owner_RED){
            if(earth.cells[i].owner == Owner_RED){
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
    std::string tempmoney;
    std::string tempplut;
    std::string tempmove;
    std::string tempbuild;
    std::stringstream out;
    out << money;
    tempmoney = out.str();
    out.str( std::string() );
    out.clear();
    out << plutonium;
    tempplut = out.str();
    out.str( std::string() );
    out.clear();
    counter->setText("        Resources\nMoney: $"+tempmoney+"\nPlutonium: "+tempplut+"kg");
    counter->setSize(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.10, 0)));
    counter->setHorizontalAlignment(CEGUI::HA_LEFT);
    counter->setProperty("FrameEnabled", "false");
    counter->setProperty("BackgroundEnabled", "false");
 
    sheet->addChildWindow(counter);

    counter = wmgr.createWindow("TaharezLook/StaticText", "CountDown");
    counter->setText("Time until build:\n          0");
    counter->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.15, 0)));
    counter->setHorizontalAlignment(CEGUI::HA_LEFT);
    counter->setVerticalAlignment(CEGUI::VA_BOTTOM);
    counter->setVisible( false );
    counter->setProperty("HorzFormatting", "WordWrapCentred");
 
    sheet->addChildWindow(counter);

    std::string commandMy;
    std::string commandEnemy;
    std::string commandTotal;
    std::string commandMax;
    out << commandBaseTotal;
    commandTotal = out.str();
    out.str( std::string() );
    out.clear();
    out << myCommandBase;
    commandMy = out.str();
    out.str( std::string() );
    out.clear();
    out << enemyCommandBase;
    commandEnemy = out.str();
    out.str( std::string() );
    out.clear();   
    out << commandBaseMax;
    commandMax = out.str();
    out.str( std::string() );
    out.clear(); 

    CEGUI::Window *resultWindow = wmgr.createWindow("TaharezLook/StaticText", "ProgressText1");
    resultWindow->setHorizontalAlignment(CEGUI::HA_CENTRE);
    resultWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.04f, 0)));
    if(myOwner == Owner_BLUE)
        resultWindow->setText("[colour='FF0000FF']Player Command Bases Remaining: "+commandMy+"/"+commandTotal+"");
    else
        resultWindow->setText("[colour='FFFF0000']Player Command Bases Remaining: "+commandMy+"/"+commandTotal+"");
    resultWindow->setProperty("FrameEnabled", "false");
    resultWindow->setProperty("BackgroundEnabled", "false");
    resultWindow->setProperty("HorzFormatting", "WordWrapCentred");
    sheet->addChildWindow(resultWindow);

    resultWindow = wmgr.createWindow("TaharezLook/StaticText", "ProgressText2");
    resultWindow->setHorizontalAlignment(CEGUI::HA_CENTRE);
    resultWindow->setYPosition(CEGUI::UDim(0.05f, 0));
    resultWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.04f, 0)));
    if(myOwner == Owner_BLUE)
        resultWindow->setText("[colour='FFFF0000']Enemy Command Bases Remaining: "+commandEnemy+"/"+commandMax+"");
    else
        resultWindow->setText("[colour='FF0000FF']Enemy Command Bases Remaining: "+commandEnemy+"/"+commandMax+"");
    resultWindow->setProperty("FrameEnabled", "false");
    resultWindow->setProperty("BackgroundEnabled", "false");
    resultWindow->setProperty("HorzFormatting", "WordWrapCentred");
    sheet->addChildWindow(resultWindow);

    resultWindow = wmgr.createWindow("TaharezLook/Button", "FriendlyReminder");
    resultWindow->setHorizontalAlignment(CEGUI::HA_LEFT);
    resultWindow->setVerticalAlignment(CEGUI::VA_CENTRE);
    resultWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.4f, 0)));
    resultWindow->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::FriendlyReminderButton,this));
    sheet->addChildWindow(resultWindow);

    resultWindow = wmgr.createWindow("TaharezLook/StaticText", "FriendlyReminderText1");
    resultWindow->setHorizontalAlignment(CEGUI::HA_LEFT);
    resultWindow->setVerticalAlignment(CEGUI::VA_CENTRE);
    resultWindow->setXPosition(CEGUI::UDim(0.05f, 0));
    resultWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.4f, 0)));
    resultWindow->setText("Welcome! To interact with the world, use ASWD to move the camera around, QE to zoom in/out, and the left and right clicks to select/deselect, respectively, a cell. Click to Continue.");
    resultWindow->setProperty("FrameEnabled", "false");
    resultWindow->setProperty("BackgroundEnabled", "false");
    resultWindow->setProperty("HorzFormatting", "WordWrapCentred");
    resultWindow->disable();

    sheet->addChildWindow(resultWindow);

    resultWindow = wmgr.createWindow("TaharezLook/StaticText", "FriendlyReminderText2");
    resultWindow->setHorizontalAlignment(CEGUI::HA_LEFT);
    resultWindow->setVerticalAlignment(CEGUI::VA_CENTRE);
    resultWindow->setXPosition(CEGUI::UDim(0.05f, 0));
    resultWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.4f, 0)));
    resultWindow->setText("In order to purge the world of Evil, the opposing views must be completely destroyed. Your representing color is seen at the top of the screen. Tiles on the screen are represented as Land (green outline), Water (blue outline), and WaterLand (yellow outline). The thicker outlines on the map mark your's and your opponent's territory. Click to Continue.");
    resultWindow->setProperty("FrameEnabled", "false");
    resultWindow->setProperty("BackgroundEnabled", "false");
    resultWindow->setProperty("HorzFormatting", "WordWrapCentred");
    resultWindow->disable();
    resultWindow->setVisible( false );

    sheet->addChildWindow(resultWindow);

    resultWindow = wmgr.createWindow("TaharezLook/StaticText", "FriendlyReminderText3");
    resultWindow->setHorizontalAlignment(CEGUI::HA_LEFT);
    resultWindow->setVerticalAlignment(CEGUI::VA_CENTRE);
    resultWindow->setXPosition(CEGUI::UDim(0.05f, 0));
    resultWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.4f, 0), CEGUI::UDim(0.4f, 0)));
    resultWindow->setText("Begin your reign of Justice by clicking any non-water tile within your boundries. From there, the CommandBase button (upper right cornor below Sound) will light up, allowing you to click and build. Begin by building "+commandMax+" CommandBases that will act as your foundation. Click this message to remove.");
    resultWindow->setProperty("FrameEnabled", "false");
    resultWindow->setProperty("BackgroundEnabled", "false");
    resultWindow->setProperty("HorzFormatting", "WordWrapCentred");
    resultWindow->disable();
    resultWindow->setVisible( false );

    sheet->addChildWindow(resultWindow);

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

    w = CEGUI::WindowManager::getSingleton().getWindow("CommandBaseText");
    out << Au_COMMANDBASE;
    tempmoney = out.str();
    out.str( std::string() );
    out.clear();
    out << BuildTime_COMMANDBASE;
    tempbuild = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Cost:$"+tempmoney+"\nBuild Time:"+tempbuild+"s");

    CEGUI::ImagesetManager::getSingleton().create( "ArmyBase.imageset" );
    w = wmgr.loadWindowLayout("ArmyBase.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.20f, 0));
    w->setVisible( true );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::ArmyBaseButton,this));
    w->disable();

    sheet->addChildWindow(w);

    w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBaseText");
    out << Au_ARMYBASE;
    tempmoney = out.str();
    out.str( std::string() );
    out.clear();
    out << BuildTime_ARMYBASE;
    tempbuild = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Cost:$"+tempmoney+"\nBuild Time:"+tempbuild+"s");

    CEGUI::ImagesetManager::getSingleton().create( "NavyBase.imageset" );
    w = wmgr.loadWindowLayout("NavyBase.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.35f, 0));
    w->setVisible( true );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::NavyBaseButton,this));
    w->disable();

    sheet->addChildWindow(w);

    w = CEGUI::WindowManager::getSingleton().getWindow("NavyBaseText");
    out << Au_NAVYBASE;
    tempmoney = out.str();
    out.str( std::string() );
    out.clear();
    out << BuildTime_NAVYBASE;
    tempbuild = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Cost:$"+tempmoney+"\nBuild Time:"+tempbuild+"s");

    CEGUI::ImagesetManager::getSingleton().create( "AirForceBase.imageset" );
    w = wmgr.loadWindowLayout("AirForceBase.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.50f, 0));
    w->setVisible( true );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::AirForceBaseButton,this));
    w->disable();

    sheet->addChildWindow(w);

    w = CEGUI::WindowManager::getSingleton().getWindow("AirForceBaseText");
    out << Au_AIRFORCEBASE;
    tempmoney = out.str();
    out.str( std::string() );
    out.clear();
    out << BuildTime_AIRFORCEBASE;
    tempbuild = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Cost:$"+tempmoney+"\nBuild Time:"+tempbuild+"s");
    
    CEGUI::ImagesetManager::getSingleton().create( "NuclearPlant.imageset" );
    w = wmgr.loadWindowLayout("NuclearPlant.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.65f, 0));
    w->setVisible( true );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::NuclearPlantButton,this));
    w->disable();

    sheet->addChildWindow(w);

    w = CEGUI::WindowManager::getSingleton().getWindow("NuclearPlantText");
    out << Au_NUCLEARPLANT;
    tempmoney = out.str();
    out.str( std::string() );
    out.clear();
    out << BuildTime_NUCLEARPLANT;
    tempbuild = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Cost:$"+tempmoney+"\nBuild Time:"+tempbuild+"s");

    CEGUI::ImagesetManager::getSingleton().create( "ICBMSilo.imageset" );
    w = wmgr.loadWindowLayout("ICBMSilo.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.80f, 0));
    w->setVisible( true );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::ICBMSiloButton,this));
    w->disable();

    sheet->addChildWindow(w);

    w = CEGUI::WindowManager::getSingleton().getWindow("ICBMSiloText");
    out << Au_ICBMSILO;
    tempmoney = out.str();
    out.str( std::string() );
    out.clear();
    out << BuildTime_ICBMSILO;
    tempbuild = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Cost:$"+tempmoney+"\nBuild Time:"+tempbuild+"s");
    
    CEGUI::ImagesetManager::getSingleton().create( "Infantry.imageset" );
    w = wmgr.loadWindowLayout("Infantry.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::InfantryButton,this));
    w->disable();

    sheet->addChildWindow(w);

    w = CEGUI::WindowManager::getSingleton().getWindow("InfantryText");
    out << Au_INFANTRY;
    tempmoney = out.str();
    out.str( std::string() );
    out.clear();
    out << BuildTime_INFANTRY;
    tempbuild = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Cost:$"+tempmoney+"\nBuild Time:"+tempbuild+"s");
    
    CEGUI::ImagesetManager::getSingleton().create( "Submarine.imageset" );
    w = wmgr.loadWindowLayout("Submarine.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::SubmarineButton,this));
    w->disable();

    sheet->addChildWindow(w);    

    w = CEGUI::WindowManager::getSingleton().getWindow("SubmarineText");
    out << Au_SUBMARINE;
    tempmoney = out.str();
    out.str( std::string() );
    out.clear();
    out << BuildTime_SUBMARINE;
    tempbuild = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Cost:$"+tempmoney+"\nBuild Time:"+tempbuild+"s");
    
    CEGUI::ImagesetManager::getSingleton().create( "Destroyer.imageset" );
    w = wmgr.loadWindowLayout("Destroyer.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.20f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::DestroyerButton,this));
    w->disable();

    sheet->addChildWindow(w);

    w = CEGUI::WindowManager::getSingleton().getWindow("DestroyerText");
    out << Au_DESTROYER;
    tempmoney = out.str();
    out.str( std::string() );
    out.clear();
    out << BuildTime_DESTROYER;
    tempbuild = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Cost:$"+tempmoney+"\nBuild Time:"+tempbuild+"s");
    
    CEGUI::ImagesetManager::getSingleton().create( "Tank.imageset" );
    w = wmgr.loadWindowLayout("Tank.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::TankButton,this));
    w->disable();

    sheet->addChildWindow(w);

    w = CEGUI::WindowManager::getSingleton().getWindow("TankText");
    out << Au_TANK;
    tempmoney = out.str();
    out.str( std::string() );
    out.clear();
    out << BuildTime_TANK;
    tempbuild = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Cost:$"+tempmoney+"\nBuild Time:"+tempbuild+"s");
    
    CEGUI::ImagesetManager::getSingleton().create( "Scud.imageset" );
    w = wmgr.loadWindowLayout("Scud.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.20f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::ScudButton,this));
    w->disable();

    sheet->addChildWindow(w);

    w = CEGUI::WindowManager::getSingleton().getWindow("ScudText");
    out << Au_SCUD;
    tempmoney = out.str();
    out.str( std::string() );
    out.clear();
    out << BuildTime_SCUD;
    tempbuild = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Cost:$"+tempmoney+"\nBuild Time:"+tempbuild+"s");

    CEGUI::ImagesetManager::getSingleton().create( "Bomber.imageset" );
    w = wmgr.loadWindowLayout("Bomber.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::BomberButton,this));
    w->disable();

    sheet->addChildWindow(w);

    w = CEGUI::WindowManager::getSingleton().getWindow("BomberText");
    out << Au_BOMBER;
    tempmoney = out.str();
    out.str( std::string() );
    out.clear();
    out << BuildTime_BOMBER;
    tempbuild = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Cost:$"+tempmoney+"\nBuild Time:"+tempbuild+"s");
    
    CEGUI::ImagesetManager::getSingleton().create( "Fighter.imageset" );
    w = wmgr.loadWindowLayout("Fighter.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.20f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::FighterButton,this));
    w->disable();

    sheet->addChildWindow(w);

    w = CEGUI::WindowManager::getSingleton().getWindow("FighterText");
    out << Au_FIGHTER;
    tempmoney = out.str();
    out.str( std::string() );
    out.clear();
    out << BuildTime_FIGHTER;
    tempbuild = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Cost:$"+tempmoney+"\nBuild Time:"+tempbuild+"s");

    if(myOwner == Owner_BLUE)
    {
        CEGUI::ImagesetManager::getSingleton().create( "BCapture.imageset" );
        w = wmgr.loadWindowLayout("BCapture.layout");
    }
    else
    {
        CEGUI::ImagesetManager::getSingleton().create( "RCapture.imageset" );
        w = wmgr.loadWindowLayout("RCapture.layout");
    }
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::CaptureButton,this));
    w->disable();

    sheet->addChildWindow(w);

    CEGUI::ImagesetManager::getSingleton().create( "NuclearPlantInterface.imageset" );
    w = wmgr.loadWindowLayout("NuclearPlantInterface.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->disable();

    sheet->addChildWindow(w);

    CEGUI::ImagesetManager::getSingleton().create( "BomberInterface.imageset" );
    w = wmgr.loadWindowLayout("BomberInterface.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->disable();

    sheet->addChildWindow(w);

    w = CEGUI::WindowManager::getSingleton().getWindow("BomberInterfaceText");
    out << MoveTime_BOMBER;
    tempmove = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Move Time:"+tempmove+"s");

    CEGUI::ImagesetManager::getSingleton().create( "DestroyerInterface.imageset" );
    w = wmgr.loadWindowLayout("DestroyerInterface.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->disable();

    sheet->addChildWindow(w);

    w = CEGUI::WindowManager::getSingleton().getWindow("DestroyerInterfaceText");
    out << MoveTime_DESTROYER;
    tempmove = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Move Time:"+tempmove+"s");

    CEGUI::ImagesetManager::getSingleton().create( "FighterInterface.imageset" );
    w = wmgr.loadWindowLayout("FighterInterface.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->disable();

    sheet->addChildWindow(w);

    CEGUI::ImagesetManager::getSingleton().create( "InfantryInterface.imageset" );
    w = wmgr.loadWindowLayout("InfantryInterface.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.20f, 0));
    w->setVisible( false );
    w->disable();

    sheet->addChildWindow(w);

    w = CEGUI::WindowManager::getSingleton().getWindow("InfantryInterfaceText");
    out << MoveTime_INFANTRY;
    tempmove = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Move Time:"+tempmove+"s");

    CEGUI::ImagesetManager::getSingleton().create( "ScudInterface.imageset" );
    w = wmgr.loadWindowLayout("ScudInterface.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->disable();

    sheet->addChildWindow(w);

    w = CEGUI::WindowManager::getSingleton().getWindow("ScudInterfaceText");
    out << MoveTime_SCUD;
    tempmove = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Move Time:"+tempmove+"s");

    CEGUI::ImagesetManager::getSingleton().create( "SubmarineInterface.imageset" );
    w = wmgr.loadWindowLayout("SubmarineInterface.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->disable();

    sheet->addChildWindow(w);

    w = CEGUI::WindowManager::getSingleton().getWindow("SubmarineInterfaceText");
    out << MoveTime_SUBMARINE;
    tempmove = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Move Time:"+tempmove+"s");

    CEGUI::ImagesetManager::getSingleton().create( "TankInterface.imageset" );
    w = wmgr.loadWindowLayout("TankInterface.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->disable();

    sheet->addChildWindow(w);

    w = CEGUI::WindowManager::getSingleton().getWindow("TankInterfaceText");
    out << MoveTime_TANK;
    tempmove = out.str();
    out.str( std::string() );
    out.clear();
    w->setText("Move Time:"+tempmove+"s");

    CEGUI::ImagesetManager::getSingleton().create( "Launch.imageset" );
    w = wmgr.loadWindowLayout("Launch.layout");
    w->setHorizontalAlignment(CEGUI::HA_RIGHT);
    w->setYPosition(CEGUI::UDim(0.05f, 0));
    w->setVisible( false );
    w->subscribeEvent(CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&SingleGameState::LaunchButton,this));
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
    // Set default values forvariables
    float r1 = (rand() % 100);
    float r2 = (rand() % 100);
    float r3 = (rand() % 100);
    
}

bool SingleGameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    CEGUI::System &sys = CEGUI::System::getSingleton();
    sys.injectKeyDown(keyEventRef.key);
    sys.injectChar(keyEventRef.text);
    if(keyEventRef.key == OIS::KC_ESCAPE)
    {
        pushAppState(findByName("PauseState"));
        return true;
    }
    if(keyEventRef.key == OIS::KC_R) //retire the unit on the current cell
    {
        if(currentCell->myUnit != Unit_EMPTY && currentCell->occupier == myOwner){
            currentCell->myUnit = 0;
            currentCell->myUnitId = -1;
            currentCell->occupier == Owner_NEUTRAL;
            units[currentCell->myUnitId].destroy(m_pSceneMgr);
            BuildButtons();
        }
        return true;
    }
    if(keyEventRef.key == OIS::KC_X)
    {
        SingleGameState::soundSwitch();
    }
    if(keyEventRef.key == OIS::KC_Z)
    {
        if(showWaterLand) {
            showWaterLand = false;
            for(int i=0;i<earth.cells.size();i++){
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
            for(int i=0;i<earth.cells.size();i++){
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
    if(xi > 90) xi = 90;
    if(xi < -90) xi = -90;
    if(yi > 90) yi = 90;
    if(yi < -90) yi = -90;
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
            if(ready){
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

void SingleGameState::deselect(){
    unitBuilding = 0;
    unitMoving = 0;
    if(m_pCurrentObject){
        m_pCurrentObject->showBoundingBox(false);
    }
    if(lastCell != NULL){
        earth.changeMaterial(m_pSceneMgr, *lastCell, lastCell->baseMaterial);
        for(int i=0; i<lastCell->neighbors.size(); i++){
            PlanetCell& temp = earth.cells[lastCell->neighbors[i]];
            earth.changeMaterial(m_pSceneMgr, temp, temp.baseMaterial);
        }
    }
    m_pCurrentObject = NULL;
    m_pCurrentEntity = NULL;
    currentCell = NULL;
    lastCell = NULL;
    BuildButtons();
}

void SingleGameState::onLeftPressed(const OIS::MouseEvent &evt)
{
    printf("%d THE WIDTH",evt.state.width);
    printf("%d THE HEIGHT",evt.state.height);
    
    if(m_pCurrentObject){
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
            string name = itr->movable->getName();
            string idType = name.substr(0, 5);
            
            if(idType == "Cell_"){ // Select a cell.
                
                if(lastCell != NULL){
                    earth.changeMaterial(m_pSceneMgr, *lastCell, lastCell->baseMaterial);
                    for(int i=0; i<lastCell->neighbors.size(); i++){
                        PlanetCell& temp = earth.cells[lastCell->neighbors[i]];
                        earth.changeMaterial(m_pSceneMgr, temp, temp.baseMaterial);
                    }
                }
                
                string idNumber = name.substr(5, name.length()-1); 
                istringstream(idNumber) >> intId; 
                currentCell = &(earth.cells[intId]); 
                
                cout << "Cell id = " << intId << endl; 
                
                if(unitBuilding){
                    if(lastCell != NULL && currentCell->myUnit == Unit_EMPTY && currentCell->building == false){
                        issueProduceOrder(*lastCell, *currentCell);
                    }   
                    else{
                        unitBuilding = false;
                    }             
                }
                else if(unitMoving){
                    if(lastCell != NULL && (currentCell->myUnit == Unit_EMPTY || currentCell->occupier != myOwner) && currentCell->building == false){
                        issueMoveOrder(units[lastCell->myUnitId], *lastCell, *currentCell);
                    }
                    else{
                        unitMoving = false;
                    }
                }
                
                BuildButtons();
                if(currentCell->myUnit >= 1 && currentCell->myUnit <= 5){ //land unit
                    for(int i=0;i<currentCell->neighbors.size();i++){
                        PlanetCell& temp = earth.cells[currentCell->neighbors[i]];
                        if(temp.terrain != Terrain_WATER && temp.myUnit == Unit_EMPTY){
                            acceptNeighbors[i] = currentCell->neighbors[i];
                            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
                        }
                        else{
                            acceptNeighbors[i] = -1;
                            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
                        }
                    }
                    unitMoving = currentCell->myUnit;
                }
                else if(currentCell->myUnit >= 6 && currentCell->myUnit <= 9){ //naval unit
                    for(int i=0;i<currentCell->neighbors.size();i++){
                        PlanetCell& temp = earth.cells[currentCell->neighbors[i]];
                        if(temp.terrain != Terrain_LAND && temp.myUnit == Unit_EMPTY){
                            acceptNeighbors[i] = currentCell->neighbors[i];
                            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
                        }
                        else{
                            acceptNeighbors[i] = -1;
                            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
                        }
                    }
                    unitMoving = currentCell->myUnit;
                }
                else if(currentCell->myUnit >= 10 && currentCell->myUnit <= 12){ //air unit
                    for(int i=0;i<currentCell->neighbors.size();i++){
                        PlanetCell& temp = earth.cells[currentCell->neighbors[i]];
                        if(temp.myUnit == Unit_EMPTY){
                            acceptNeighbors[i] = currentCell->neighbors[i];
                            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
                        }
                        else{
                            acceptNeighbors[i] = -1;
                            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
                        }
                    }
                    unitMoving = currentCell->myUnit;
                }
                
                m_pCurrentObject = m_pSceneMgr->getEntity(name)->getParentSceneNode();
                m_pCurrentObject->showBoundingBox(true);
                m_pCurrentEntity = m_pSceneMgr->getEntity(name);
                
                lastCell = currentCell;
                
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
    if(m_pCurrentObject){
        m_pCurrentObject->showBoundingBox(true); 
    }
}

void SingleGameState::onRightPressed(const OIS::MouseEvent &evt){
    deselect();
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
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_Q)){
	    if(distance >= 11000){
	    	m_pCamera->moveRelative(m_TranslateVector/10);
	    }
    }
    else if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_E)){
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

	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_E)){
	    if(distance < 20000){
		m_TranslateVector.z = m_MoveScale;
	    }
	}
	 
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_Q)){

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
    
    processTimer();
    processEvents();
    
    
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
    currentCell->myUnit_pending = Unit_COMMANDBASE;
    currentCell->timer = BuildTime_COMMANDBASE;
    currentCell->building = true;
    processResources(-Au_COMMANDBASE,-Pt_COMMANDBASE);
    
    Unit newUnit(myOwner, Unit_COMMANDBASE);
    commandBaseChange(true, false);
    newUnit.createManualObject(m_pSceneMgr);
    newUnit.relocate(earth.vertices[currentCell->id]);
    newUnit.translate(0,150,0);
    units.push_back(newUnit);
    currentCell->myUnitId = newUnit.id;

    std::time(&currentTime);
    currentCell->timeNeeded = currentTime+currentCell->timer;
    Times temp(currentTime+currentCell->timer , currentCell->id);
    eventQueue.push(temp);

    onButton = true;
    return true;
}
bool SingleGameState::ArmyBaseButton(const CEGUI::EventArgs &e)
{
    currentCell->myUnit_pending = Unit_ARMYBASE;
    currentCell->timer = BuildTime_ARMYBASE;
    currentCell->building = true;
    processResources(-Au_ARMYBASE,-Pt_ARMYBASE);

    Unit newUnit(myOwner, Unit_ARMYBASE);
    newUnit.createManualObject(m_pSceneMgr);
    newUnit.relocate(earth.vertices[currentCell->id]); 
    newUnit.translate(0,150,0);
    units.push_back(newUnit);
    currentCell->myUnitId = newUnit.id;

    std::time(&currentTime);
    currentCell->timeNeeded = currentTime+currentCell->timer;
    Times temp(currentTime+currentCell->timer , currentCell->id);
    eventQueue.push(temp);

    onButton = true;
    return true;
}
bool SingleGameState::NavyBaseButton(const CEGUI::EventArgs &e)
{
    currentCell->myUnit_pending = Unit_NAVYBASE;
    currentCell->timer = BuildTime_NAVYBASE;
    currentCell->building = true;
    processResources(-Au_NAVYBASE,-Pt_NAVYBASE);
    
    Unit newUnit(myOwner, Unit_NAVYBASE);
    newUnit.createManualObject(m_pSceneMgr);
    newUnit.relocate(earth.vertices[currentCell->id]);
    newUnit.translate(0,150,0);
    units.push_back(newUnit);
    currentCell->myUnitId = newUnit.id;

    std::time(&currentTime);
    currentCell->timeNeeded = currentTime+currentCell->timer;
    Times temp(currentTime+currentCell->timer , currentCell->id);
    eventQueue.push(temp);

    onButton = true;
    return true;
}
bool SingleGameState::AirForceBaseButton(const CEGUI::EventArgs &e)
{
    currentCell->myUnit_pending = Unit_AIRFORCEBASE;
    currentCell->timer = BuildTime_AIRFORCEBASE;
    currentCell->building = true;
    processResources(-Au_AIRFORCEBASE,-Pt_AIRFORCEBASE);
    
    Unit newUnit(myOwner, Unit_AIRFORCEBASE);
    newUnit.createManualObject(m_pSceneMgr);
    newUnit.relocate(earth.vertices[currentCell->id]); 
    newUnit.translate(0,150,0);
    units.push_back(newUnit);
    currentCell->myUnitId = newUnit.id;

    std::time(&currentTime);
    currentCell->timeNeeded = currentTime+currentCell->timer;
    Times temp(currentTime+currentCell->timer , currentCell->id);
    eventQueue.push(temp);

    onButton = true;
    return true;
}
bool SingleGameState::NuclearPlantButton(const CEGUI::EventArgs &e)
{
    currentCell->myUnit_pending = Unit_NUCLEARPLANT;
    currentCell->timer = BuildTime_NUCLEARPLANT;
    currentCell->building = true;
    processResources(-Au_NUCLEARPLANT,-Pt_NUCLEARPLANT);
    
    Unit newUnit(myOwner, Unit_NUCLEARPLANT);
    newUnit.createManualObject(m_pSceneMgr);
    newUnit.relocate(earth.vertices[currentCell->id]); 
    newUnit.translate(0,150,0);
    units.push_back(newUnit);
    currentCell->myUnitId = newUnit.id;

    std::time(&currentTime);
    currentCell->timeNeeded = currentTime+currentCell->timer;
    Times temp(currentTime+currentCell->timer , currentCell->id);
    eventQueue.push(temp);

    onButton = true;
    return true;
}
bool SingleGameState::ICBMSiloButton(const CEGUI::EventArgs &e)
{
    currentCell->myUnit_pending = Unit_ICBMSILO;
    currentCell->timer = BuildTime_ICBMSILO;
    currentCell->building = true;
    processResources(-Au_ICBMSILO,-Pt_ICBMSILO);
    
    Unit newUnit(myOwner, Unit_ICBMSILO);
    newUnit.createManualObject(m_pSceneMgr);
    newUnit.relocate(earth.vertices[currentCell->id]);
    newUnit.translate(0,150,0);
    units.push_back(newUnit);
    currentCell->myUnitId = newUnit.id;

    std::time(&currentTime);
    currentCell->timeNeeded = currentTime+currentCell->timer;
    Times temp(currentTime+currentCell->timer , currentCell->id);
    eventQueue.push(temp);

    onButton = true;
    return true;
}
bool SingleGameState::InfantryButton(const CEGUI::EventArgs &e)
{
    for(int i=0;i<currentCell->neighbors.size();i++){
        PlanetCell& temp = earth.cells[currentCell->neighbors[i]];
        if(temp.terrain != Terrain_WATER && temp.myUnit == Unit_EMPTY)
        {
            acceptNeighbors[i] = currentCell->neighbors[i];
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
        }
        else
        {
            acceptNeighbors[i] = -1;
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
        }
    }
    unitBuilding = Unit_INFANTRY;
    unitMoney = Au_INFANTRY;
    unitPlutonium = Pt_INFANTRY;
    unitTimer = BuildTime_INFANTRY;
    onButton = true;
    return true;
}
bool SingleGameState::TankButton(const CEGUI::EventArgs &e)
{
    for(int i=0;i<currentCell->neighbors.size();i++){
        PlanetCell& temp = earth.cells[currentCell->neighbors[i]];
        if(temp.terrain != Terrain_WATER && temp.myUnit == Unit_EMPTY)
        {
            acceptNeighbors[i] = currentCell->neighbors[i];
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
        }
        else
        {
            acceptNeighbors[i] = -1;
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
        }
    }
    unitBuilding = Unit_TANK;
    unitMoney = Au_TANK;
    unitPlutonium = Pt_TANK;
    unitTimer = BuildTime_INFANTRY;
    onButton = true;
    return true;
}
bool SingleGameState::ScudButton(const CEGUI::EventArgs &e)
{
    for(int i=0;i<currentCell->neighbors.size();i++){
        PlanetCell& temp = earth.cells[currentCell->neighbors[i]];
        if(temp.terrain != Terrain_WATER && temp.myUnit == Unit_EMPTY)
        {
            acceptNeighbors[i] = currentCell->neighbors[i];
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
        }
        else
        {
            acceptNeighbors[i] = -1;
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
        }
    }
    unitBuilding = Unit_SCUD;
    unitMoney = Au_SCUD;
    unitPlutonium = Pt_SCUD;
    unitTimer = BuildTime_INFANTRY;
    onButton = true;
    return true;
}
bool SingleGameState::SubmarineButton(const CEGUI::EventArgs &e)
{
    for(int i=0;i<currentCell->neighbors.size();i++){
        PlanetCell& temp = earth.cells[currentCell->neighbors[i]];
        if(temp.terrain != Terrain_LAND && temp.myUnit == Unit_EMPTY)
        {
            acceptNeighbors[i] = currentCell->neighbors[i];
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
        }
        else
        {
            acceptNeighbors[i] = -1;
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
        }
    }
    unitBuilding = Unit_SUBMARINE;
    unitMoney = Au_SUBMARINE;
    unitPlutonium = Pt_SUBMARINE;
    unitTimer = BuildTime_INFANTRY;
    onButton = true;
    return true;
}
bool SingleGameState::DestroyerButton(const CEGUI::EventArgs &e)
{
    for(int i=0;i<currentCell->neighbors.size();i++){
        PlanetCell& temp = earth.cells[currentCell->neighbors[i]];
        if(temp.terrain != Terrain_LAND && temp.myUnit == Unit_EMPTY)
        {
            acceptNeighbors[i] = currentCell->neighbors[i];
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
        }
        else
        {
            acceptNeighbors[i] = -1;
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
        }
    }
    unitBuilding = Unit_DESTROYER;
    unitMoney = Au_DESTROYER;
    unitPlutonium = Pt_DESTROYER;
    unitTimer = BuildTime_INFANTRY;
    onButton = true;
    return true;
}
bool SingleGameState::BomberButton(const CEGUI::EventArgs &e)
{
    for(int i=0;i<currentCell->neighbors.size();i++){
        PlanetCell& temp = earth.cells[currentCell->neighbors[i]];
        if(temp.myUnit == Unit_EMPTY)
        {
            acceptNeighbors[i] = currentCell->neighbors[i];
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
        }
        else
        {
            acceptNeighbors[i] = -1;
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
        }
    }
    unitBuilding = Unit_BOMBER;
    unitMoney = Au_BOMBER;
    unitPlutonium = Pt_BOMBER;
    unitTimer = BuildTime_INFANTRY;
    onButton = true;
    return true;
}
bool SingleGameState::FighterButton(const CEGUI::EventArgs &e)
{
    for(int i=0;i<currentCell->neighbors.size();i++){
        PlanetCell& temp = earth.cells[currentCell->neighbors[i]];
        if(temp.myUnit == Unit_EMPTY)
        {
            acceptNeighbors[i] = currentCell->neighbors[i];
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_green");
        }
        else
        {
            acceptNeighbors[i] = -1;
            earth.changeMaterial(m_pSceneMgr, temp, "MyMaterials/earth_day_red");
        }
    }
    unitBuilding = Unit_FIGHTER;
    unitMoney = Au_FIGHTER;
    unitPlutonium = Pt_FIGHTER;
    unitTimer = BuildTime_INFANTRY;
    onButton = true;
    return true;
}

bool SingleGameState::CaptureButton(const CEGUI::EventArgs &e)
{

    return true;
}

bool SingleGameState::LaunchButton(const CEGUI::EventArgs &e)
{
    return true;
}

bool SingleGameState::FriendlyReminderButton(const CEGUI::EventArgs &e)
{
    if(tutorial == 0){
        CEGUI::Window *resultWindow = CEGUI::WindowManager::getSingleton().getWindow("FriendlyReminderText1");
        resultWindow->setVisible( false );
        resultWindow = CEGUI::WindowManager::getSingleton().getWindow("FriendlyReminderText2");
        resultWindow->setVisible( true );
        resultWindow->activate();
        tutorial++;
    }
    else if(tutorial == 1){
        CEGUI::Window *resultWindow = CEGUI::WindowManager::getSingleton().getWindow("FriendlyReminderText2");
        resultWindow->setVisible( false );
        resultWindow = CEGUI::WindowManager::getSingleton().getWindow("FriendlyReminderText3");
        resultWindow->setVisible( true );
        resultWindow->activate();
        tutorial++;
    }
    else if(tutorial == 2){
        CEGUI::Window *resultWindow = CEGUI::WindowManager::getSingleton().getWindow("FriendlyReminder");
        resultWindow->setVisible( false );
        resultWindow = CEGUI::WindowManager::getSingleton().getWindow("FriendlyReminderText3");
        resultWindow->setVisible( false );
        tutorial++;
    }
    return true;
}

void SingleGameState::BuildingImages0()
{
    CEGUI::Window* w;
    if(buildingImages != 1)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("CommandBase");
        w->setVisible( true );
        w->disable();
        w = CEGUI::WindowManager::getSingleton().getWindow("ArmyBase");
        w->setVisible( true );
        w->disable();
        w = CEGUI::WindowManager::getSingleton().getWindow("NavyBase");
        w->setVisible( true );
        w->disable();
        w = CEGUI::WindowManager::getSingleton().getWindow("AirForceBase");
        w->setVisible( true );
        w->disable();
        w = CEGUI::WindowManager::getSingleton().getWindow("NuclearPlant");
        w->setVisible( true );
        w->disable();
        w = CEGUI::WindowManager::getSingleton().getWindow("ICBMSilo");
        w->setVisible( true );
        w->disable();
        SingleGameState::DeButton();
        buildingImages = 1;
    }
    else
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
        SingleGameState::DeButton();
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

    if(cell.owner != myOwner || cell.terrain == Terrain_WATER || cell.myUnit != 0 || cell.building)
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
    if(commandBaseTotal < commandBaseMax)
    {
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
        w = CEGUI::WindowManager::getSingleton().getWindow("CommandBase");
        w->disable();
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
        SingleGameState::DeButton();
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
    if(commandBaseTotal < commandBaseMax)
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
        SingleGameState::DeButton();
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
        SingleGameState::DeButton();
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
        SingleGameState::DeButton();
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

void SingleGameState::BuildingImagesNP6(PlanetCell &cell)
{
    CEGUI::Window* w;
    if(buildingImages != 6)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("NuclearPlantInterface");
        w->setVisible( true );    
        w->enable();
        SingleGameState::DeButton();
        buildingImages = 6;
    }
    else
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("NuclearPlantInterface");
        w->enable();
    }
}
void SingleGameState::BuildingImagesICBMS7(PlanetCell &cell)
{
    CEGUI::Window* w;
    if(buildingImages != 14)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Launch");
        w->setVisible( true );    
        w->enable();
        SingleGameState::DeButton();
        buildingImages = 14;
    }
    else
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Launch");
        w->enable();
    }
    if(money < Au_LAUNCH || plutonium < Pt_LAUNCH)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Launch");
        w->disable();
    }
}
void SingleGameState::UnitImagesI1(PlanetCell &cell)
{
    CEGUI::Window* w;
    if(buildingImages != 7)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Capture");
        w->setVisible( true );    
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("InfantryInterface");
        w->setVisible( true );    
        w->enable();
        SingleGameState::DeButton();
        buildingImages = 7;
    }
    else
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Capture");
        w->enable();
        w = CEGUI::WindowManager::getSingleton().getWindow("InfantryInterface");
        w->enable();
    }
    if(money < Au_CAPTURE || plutonium < Pt_CAPTURE || cell.owner == myOwner || cell.terrain == Terrain_WATER)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Capture");
        w->disable();
    }
}
void SingleGameState::UnitImagesT2(PlanetCell &cell)
{
    CEGUI::Window* w;
    if(buildingImages != 8)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("TankInterface");
        w->setVisible( true );    
        w->enable();
        SingleGameState::DeButton();
        buildingImages = 8;
    }
    else
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("TankInterface");
        w->enable();
    }
}
void SingleGameState::UnitImagesS3(PlanetCell &cell)
{
    CEGUI::Window* w;
    if(buildingImages != 9)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("ScudInterface");
        w->setVisible( true );    
        w->enable();
        SingleGameState::DeButton();
        buildingImages = 9;
    }
    else
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("ScudInterface");
        w->enable();
    }
}
void SingleGameState::UnitImagesSM4(PlanetCell &cell)
{
    CEGUI::Window* w;
    if(buildingImages != 10)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("SubmarineInterface");
        w->setVisible( true );    
        w->enable();
        SingleGameState::DeButton();
        buildingImages = 10;
    }
    else
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("SubmarineInterface");
        w->enable();
    }
}
void SingleGameState::UnitImagesD5(PlanetCell &cell)
{
    CEGUI::Window* w;
    if(buildingImages != 11)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("DestroyerInterface");
        w->setVisible( true );    
        w->enable();
        SingleGameState::DeButton();
        buildingImages = 11;
    }
    else
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("DestroyerInterface");
        w->enable();
    }
}
void SingleGameState::UnitImagesB6(PlanetCell &cell)
{
    CEGUI::Window* w;
    if(buildingImages != 12)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("BomberInterface");
        w->setVisible( true );    
        w->enable();
        SingleGameState::DeButton();
        buildingImages = 12;
    }
    else
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("BomberInterface");
        w->enable();
    }
}
void SingleGameState::UnitImagesF7(PlanetCell &cell)
{
    CEGUI::Window* w;
    if(buildingImages != 13)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("FighterInterface");
        w->setVisible( true );    
        w->enable();
        SingleGameState::DeButton();
        buildingImages = 13;
    }
    else
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("FighterInterface");  
        w->enable();
    }
}

void SingleGameState::BuildButtons(void)
{
    if(currentCell == NULL)
        BuildingImages0();
    else if(currentCell->myUnit == Unit_EMPTY)
        BuildingImages1(*currentCell);
    else if(currentCell->myUnit == Unit_COMMANDBASE)
        BuildingImagesCB2(*currentCell);
    else if(currentCell->myUnit == Unit_ARMYBASE)
        BuildingImagesA3(*currentCell);
    else if(currentCell->myUnit == Unit_NAVYBASE)
        BuildingImagesN4(*currentCell);
    else if(currentCell->myUnit == Unit_AIRFORCEBASE)
        BuildingImagesAF5(*currentCell);
    else if(currentCell->myUnit == Unit_NUCLEARPLANT)
        BuildingImagesNP6(*currentCell);
    else if(currentCell->myUnit == Unit_ICBMSILO)
        BuildingImagesICBMS7(*currentCell);
    else if(currentCell->myUnit == Unit_INFANTRY)
        UnitImagesI1(*currentCell);
    else if(currentCell->myUnit == Unit_TANK)
        UnitImagesT2(*currentCell);
    else if(currentCell->myUnit == Unit_SCUD)
        UnitImagesS3(*currentCell);
    else if(currentCell->myUnit == Unit_SUBMARINE)
        UnitImagesSM4(*currentCell);
    else if(currentCell->myUnit == Unit_DESTROYER)
        UnitImagesD5(*currentCell);
    else if(currentCell->myUnit == Unit_BOMBER)
        UnitImagesB6(*currentCell);
    else if(currentCell->myUnit == Unit_FIGHTER)
        UnitImagesF7(*currentCell);
    else
        BuildingImages1(*currentCell);
}

bool SingleGameState::issueProduceOrder(PlanetCell& origin, PlanetCell& goal){
    for(int i=0;i<lastCell->neighbors.size();i++){
        if(goal.id == acceptNeighbors[i]){
            /*
            Arrow& tempArrow = units[origin.myUnitId].myArrow;
            tempArrow = Arrow(myOwner, earth.vertices[origin.id], earth.vertices[goal.id]);
            tempArrow.createObject(m_pSceneMgr);
            tempArrow.setVisibility(true);
            */
            goal.myUnit_pending = unitBuilding;
            if(unitBuilding == Unit_INFANTRY) origin.timer = BuildTime_INFANTRY;
            else if(unitBuilding == Unit_TANK) origin.timer = BuildTime_TANK;
            else if(unitBuilding == Unit_ARTILLERY) origin.timer = BuildTime_ARTILLERY;
            else if(unitBuilding == Unit_SCUD) origin.timer = BuildTime_SCUD;
            else if(unitBuilding == Unit_ENGINEER) origin.timer = BuildTime_ENGINEER;
            else if(unitBuilding == Unit_SUBMARINE) origin.timer = BuildTime_SUBMARINE;
            else if(unitBuilding == Unit_DESTROYER) origin.timer = BuildTime_DESTROYER;
            else if(unitBuilding == Unit_CARRIER) origin.timer = BuildTime_CARRIER;
            else if(unitBuilding == Unit_CRUISER) origin.timer = BuildTime_CRUISER;
            else if(unitBuilding == Unit_BOMBER) origin.timer = BuildTime_BOMBER;
            else if(unitBuilding == Unit_FIGHTER) origin.timer = BuildTime_FIGHTER;
            else if(unitBuilding == Unit_SPYPLANE) origin.timer = BuildTime_SPYPLANE;
            processResources(-unitMoney,-unitPlutonium);
            btVector3 a = earth.vertices[origin.id];
            btVector3 g = earth.vertices[goal.id];
            btVector3 b = g - a;
            btVector3 c = a.cross(b);
            btVector3 d = c.cross(a);
            goal.myUnitDirection = d;
            unitBuilding = 0;
            origin.goalId = goal.id;
            illuminate(origin);
            origin.unitbuilding = true;

            std::time(&currentTime);
            origin.timeNeeded = currentTime+origin.timer;
            Times temp(currentTime+origin.timer , origin.id);
            eventQueue.push(temp);
            return true;
        }
    }
    origin.unitbuilding = false;
    origin.goalId = -1;
    unitBuilding = 0;
    illuminate(origin);
    return true;
}

bool SingleGameState::createUnit(PlanetCell& goal){
    if(goal.myUnit == 0)
    {
        goal.myUnit = goal.myUnit_pending;
        goal.occupier = myOwner;
        goal.myUnit_pending = 0;
        Unit newUnit(myOwner, goal.myUnit);
        if(goal.myUnit == Unit_SCUD){
            newUnit.createSymbolObject(m_pSceneMgr);
            //newUnit.createObject(m_pSceneMgr, "missile.mesh", "MyMaterials/Blue");
        }
        else if(goal.myUnit == Unit_FIGHTER){
            newUnit.createObject(m_pSceneMgr, "fighter.mesh", "MyMaterials/Blue");
        }
        else if(goal.myUnit == Unit_TANK){
            newUnit.createSymbolObject(m_pSceneMgr);
            //newUnit.createObject(m_pSceneMgr, "tank.mesh", "MyMaterials/Blue");
        }
        else if(goal.myUnit == Unit_INFANTRY){
            newUnit.createSymbolObject(m_pSceneMgr);
            //newUnit.createObject(m_pSceneMgr, "soldier.mesh", "MyMaterials/Blue");
        }
        else if(goal.myUnit == Unit_DESTROYER){
            newUnit.createObject(m_pSceneMgr, "destroyer.mesh", "MyMaterials/Blue");
        }
        else if(goal.myUnit == Unit_BOMBER){
            newUnit.createObject(m_pSceneMgr, "bomber.mesh", "MyMaterials/Blue");
        }
        else if(goal.myUnit == Unit_SUBMARINE){
            newUnit.createObject(m_pSceneMgr, "sub.mesh", "MyMaterials/Blue");
        }
        else{
            newUnit.createManualObject(m_pSceneMgr);
        }
        newUnit.relocate(earth.vertices[goal.id]);
        //newUnit.setDirection(goal.myUnitDirection);
        units.push_back(newUnit);
        goal.myUnitId = newUnit.id;
        illuminate(goal);
        attack(goal);
        BuildButtons();
    }
}

bool SingleGameState::issueMoveOrder(Unit& unit, PlanetCell& origin, PlanetCell& goal){
    for(int i=0;i<origin.neighbors.size();i++){
        if(goal.id == acceptNeighbors[i]){
            if(origin.myUnit == Unit_INFANTRY) origin.timer = MoveTime_INFANTRY;
            else if(origin.myUnit == Unit_TANK) origin.timer = MoveTime_TANK;
            else if(origin.myUnit == Unit_ARTILLERY) origin.timer = MoveTime_ARTILLERY;
            else if(origin.myUnit == Unit_SCUD) origin.timer = MoveTime_SCUD;
            else if(origin.myUnit == Unit_ENGINEER) origin.timer = MoveTime_ENGINEER;
            else if(origin.myUnit == Unit_SUBMARINE) origin.timer = MoveTime_SUBMARINE;
            else if(origin.myUnit == Unit_DESTROYER) origin.timer = MoveTime_DESTROYER;
            else if(origin.myUnit == Unit_CARRIER) origin.timer = MoveTime_CARRIER;
            else if(origin.myUnit == Unit_CRUISER) origin.timer = MoveTime_CRUISER;
            else if(origin.myUnit == Unit_BOMBER) origin.timer = MoveTime_BOMBER;
            else if(origin.myUnit == Unit_FIGHTER) origin.timer = MoveTime_FIGHTER;
            else if(origin.myUnit == Unit_SPYPLANE) origin.timer = MoveTime_SPYPLANE;
            btVector3 a = earth.vertices[origin.id];
            btVector3 g = earth.vertices[goal.id];
            btVector3 b = g - a;
            btVector3 c = a.cross(b);
            btVector3 d = c.cross(a);
            goal.myUnitDirection = d;
            
            origin.moving = true;
            origin.goalId = goal.id;
            illuminate(origin);
            unitMoving = 0;

            std::time(&currentTime);
            origin.timeNeeded = currentTime+origin.timer;
            Times temp(currentTime+origin.timer , origin.id);
            eventQueue.push(temp);

            return true;
        }
    }
    origin.moving = false;
    origin.goalId = -1;
    illuminate(origin);
    unitMoving = 0;
    return false;
}

bool SingleGameState::moveUnit(Unit& unit, PlanetCell& origin, PlanetCell& goal){
    origin.moving = false;
    origin.goalId = -1;
    if(goal.myUnit == Unit_EMPTY){
        unit.relocate(earth.vertices[goal.id]);
        //unit.setDirection(goal.myUnitDirection);
        origin.myUnit = 0;
        origin.occupier = Owner_NEUTRAL;
        origin.myUnitId = -1;
        goal.myUnit = unit.myType;
        origin.occupier = myOwner;
        goal.myUnitId = unit.id;
        deluminate(origin);
        illuminate(goal);
        attack(goal);
        BuildButtons();
        return true;
    }
    /*
    else if(goal.occupier != Owner_NEUTRAL && goal.occupier != myOwner){ // bumping into an enemy unit
        if(unit.myType == goal.myUnit){ // Likes kill likes.
            nuke(goal);
            unit.relocate(earth.vertices[goal.id]);
            origin.myUnit = 0;
            origin.occupier = Owner_NEUTRAL;
            origin.myUnitId = -1;
            goal.myUnit = unit.myType;
            origin.occupier = myOwner;
            goal.myUnitId = unit.id;
            deluminate(origin);
            illuminate(goal);
            attack(goal);
            BuildButtons();
            return true;
        }
        //else return false;
        else{
            nuke(goal);
            unit.relocate(earth.vertices[goal.id]);
            origin.myUnit = 0;
            origin.occupier = Owner_NEUTRAL;
            origin.myUnitId = -1;
            goal.myUnit = unit.myType;
            origin.occupier = myOwner;
            goal.myUnitId = unit.id;
            deluminate(origin);
            illuminate(goal);
            attack(goal);
            BuildButtons();
            return true;
        }
    }
    */

    return false;
}

void SingleGameState::illuminate(PlanetCell& goal){
    goal.baseMaterial = "MyMaterials/earth_day";
    earth.changeMaterial(m_pSceneMgr, goal, "MyMaterials/earth_day");
    for(int i=0;i<goal.neighbors.size();i++){
        earth.cells[goal.neighbors[i]].baseMaterial = "MyMaterials/earth_day";
        earth.changeMaterial(m_pSceneMgr, earth.cells[goal.neighbors[i]], "MyMaterials/earth_day");
    }
}

void SingleGameState::deluminate(PlanetCell& goal){
    bool unseen_neighbor = true;
    bool unseen_self = true;
    for(int i=0;i<goal.neighbors.size();i++){
        PlanetCell& temp1 = earth.cells[goal.neighbors[i]];
        if(temp1.owner == myOwner || temp1.myUnit != 0){
            unseen_self = false;
        }
        for(int j=0;j<temp1.neighbors.size();j++){
            PlanetCell& temp2 = earth.cells[temp1.neighbors[j]];
            if(temp2.id != goal.id){
                if(temp2.owner == myOwner || temp2.myUnit != 0){
                    unseen_neighbor = false;
                }
            }
        }
        if(unseen_neighbor){
            temp1.baseMaterial = "MyMaterials/earth_day_bw";
            earth.changeMaterial(m_pSceneMgr, temp1, "MyMaterials/earth_day_bw");
        }
        else{
            temp1.baseMaterial = "MyMaterials/earth_day";
            earth.changeMaterial(m_pSceneMgr, temp1, "MyMaterials/earth_day");
        }
        unseen_neighbor = true;
    }
    if(unseen_self){
        goal.baseMaterial = "MyMaterials/earth_day_bw";
        earth.changeMaterial(m_pSceneMgr, goal, "MyMaterials/earth_day_bw");
    }
    else{
        goal.baseMaterial = "MyMaterials/earth_day";
        earth.changeMaterial(m_pSceneMgr, goal, "MyMaterials/earth_day");
    }
}

float y(float x, float v, float theta){
    float k = v * cos(theta);
    return x * tan(theta) - ( (9.8*x*x)/(2*k*k) );
}

void SingleGameState::fireMissile(PlanetCell& origin, PlanetCell& goal){
    Missile newMissile(myOwner, earth.vertices[origin.id], earth.vertices[goal.id]);
    newMissile.createObject(m_pSceneMgr);
    newMissile.relocate(earth.vertices[origin.id]);
    float theta = newMissile.start.angle(newMissile.end);
    float distance = theta * 6371;
    float v = 10;
    float angle_of_reach = 0.5 * asin(9.8 * distance / v);
    float time = distance / (v*cos(theta));
    
    missiles.push_back(newMissile);
}

void SingleGameState::retireUnit(PlanetCell& targetCell){
    // No refund! No shit!
    if(targetCell.myUnit == Unit_COMMANDBASE)
        commandBaseChange(false, false);
    units[targetCell.myUnitId].destroy(m_pSceneMgr);
    deluminate(targetCell);
    targetCell.myUnit = 0;
    targetCell.myUnitId = -1;
    targetCell.occupier = Owner_NEUTRAL;
    BuildButtons();
}

void SingleGameState::killEnemyUnit(PlanetCell& targetCell){
    if(targetCell.myUnit == Unit_COMMANDBASE)
        commandBaseChange(false, true);
    units[targetCell.myUnitId].destroy(m_pSceneMgr);
    targetCell.myUnit = 0;
    targetCell.myUnitId = -1;
    targetCell.occupier = Owner_NEUTRAL;
    BuildButtons();
}

void SingleGameState::nuke(PlanetCell& targetCell){
    units[targetCell.myUnitId].destroy(m_pSceneMgr);
    deluminate(targetCell);
    targetCell.myUnit = 0;
    targetCell.myUnitId = -1;
    targetCell.occupier = Owner_NEUTRAL;
    BuildButtons();
}

void SingleGameState::processEvents(void){
    
    std::time(&currentTime);
    int stay = 1;
    while(stay)
    {
        stay = 0;
        if(!eventQueue.empty())
        {
            cout << "Oh Shit I'm In" << endl;
            Times top = eventQueue.top();
            cout << "Wasn't Top!" << endl;
            if(top.time <= currentTime){
                cout << "Wasn't if" << endl;
                PlanetCell& tempCell = earth.cells[top.cellID];
                cout << "tempCell good" << endl;
                if(tempCell.moving){
                    moveUnit(units[tempCell.myUnitId], tempCell, earth.cells[tempCell.goalId]); 
                    cout << "NotMove" << endl;
                }
                else if(tempCell.myUnit_pending){
                    cout << "inHere?" << endl;
                    if(tempCell.myUnit_pending >= 13 && tempCell.myUnit_pending <= 19 && tempCell.myUnitId != -1){
                        tempCell.myUnit = tempCell.myUnit_pending;
                        tempCell.myUnit_pending = 0;
                    }
                }
                else if(tempCell.unitbuilding){
                    tempCell.unitbuilding = false;
                    createUnit(earth.cells[tempCell.goalId]);
                }
                cout << "Freedom!" << endl;
                tempCell.timeNeeded = 0;
                tempCell.growsCompleted = 0;
                eventQueue.pop();
                cout << "Hurray!!!!" << endl;
                BuildButtons();
                stay = 1;
            }
            else{
                cout << "It's in else!" << endl;
                std::priority_queue<Times, std::vector<Times>, CompareTimes> tempEventQueue;
                int eventSize = eventQueue.size();
                for(int i=0; i<eventSize; i++){
                    Times tempTime = eventQueue.top();
                    PlanetCell& tempCell = earth.cells[tempTime.cellID];
                    if(tempCell.myUnit_pending >= 13 && tempCell.myUnit_pending <= 19 && tempCell.myUnitId != -1){
                        cout << "Well I got to here..." << endl;
                        if((tempCell.timer-(tempTime.time-currentTime) >= tempCell.growsCompleted) &&
                        (tempCell.growsCompleted < tempCell.timer) ){
                            units[tempCell.myUnitId].grow();
                            tempCell.growsCompleted++;
                            cout << "I Build Something!" << endl;
                        }
                    }
                    eventQueue.pop();
                    tempEventQueue.push(tempTime);
                }
                eventQueue = tempEventQueue;
            }
        }
    }

}

void SingleGameState::DeButton(void)
{
    CEGUI::Window* w;
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
    else if(buildingImages == 5)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Bomber");
        w->setVisible( false );
        w = CEGUI::WindowManager::getSingleton().getWindow("Fighter");
        w->setVisible( false );
    }
    else if(buildingImages == 6)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("NuclearPlantInterface");
        w->setVisible( false );
    }
    else if(buildingImages == 7)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Capture");
        w->setVisible( false );
        w = CEGUI::WindowManager::getSingleton().getWindow("InfantryInterface");
        w->setVisible( false );
    }
    else if(buildingImages == 8)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("TankInterface");
        w->setVisible( false );
    }
    else if(buildingImages == 9)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("ScudInterface");
        w->setVisible( false );
    }
    else if(buildingImages == 10)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("SubmarineInterface");
        w->setVisible( false );
    }
    else if(buildingImages == 11)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("DestroyerInterface");
        w->setVisible( false );
    }
    else if(buildingImages == 12)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("BomberInterface");
        w->setVisible( false );
    }
    else if(buildingImages == 13)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("FighterInterface");
        w->setVisible( false );
    }
    else if(buildingImages == 14)
    {
        w = CEGUI::WindowManager::getSingleton().getWindow("Launch");
        w->setVisible( false );
    }
}

void SingleGameState::processResources(int inputmoney, int inputplut)
{
    money += inputmoney;
    plutonium += inputplut;
    CEGUI::Window* counter = CEGUI::WindowManager::getSingleton().getWindow("Counter");
    std::string tempmoney;
    std::string tempplut;
    std::stringstream out;
    out << money;
    tempmoney = out.str();
    out.str( std::string() );
    out.clear();
    out << plutonium;
    tempplut = out.str();
    out.str( std::string() );
    out.clear();
    counter->setText("        Resources\nMoney: $"+tempmoney+"\nPlutonium: "+tempplut+"kg");
    BuildButtons();
}

void SingleGameState::processTimer(void)
{
    std::time(&currentTime);
    if(currentCell!=NULL && currentCell->timeNeeded > 0)
    {
        CEGUI::Window* counter = CEGUI::WindowManager::getSingleton().getWindow("CountDown");
        time_t count = currentCell->timeNeeded - currentTime;
        std::string temptime;
        std::stringstream out;
        out << count;
        temptime = out.str();
        out.str( std::string() );
        out.clear();
        if(currentCell->moving)
            counter->setText("Time until move:\n"+temptime+"s");
        else
            counter->setText("Time until build:\n"+temptime+"s");
        if(!timerup)
        {
            timerup = true;
            counter->setVisible( true );
        }
        
    }
    else if(timerup)
    {
        CEGUI::Window* counter = CEGUI::WindowManager::getSingleton().getWindow("CountDown");
        counter->setVisible( false );
        timerup = false;
    }
}

void SingleGameState::capture(void)
{
    earth.cells[currentCell->id].owner = myOwner;
    earth.own(m_pSceneMgr, earth.cells[currentCell->id]);
    earth.updateBorderSegments(m_pSceneMgr);
}


void SingleGameState::attack(PlanetCell& targetCell)
{
    for (int i=0;i<targetCell.neighbors.size();i++){
        PlanetCell& temp = earth.cells[targetCell.neighbors[i]];
        if(temp.occupier == myEnemy && temp.myUnit > 0){
            combat(targetCell,temp);
            if(targetCell.occupier == Owner_NEUTRAL)
                break;
        }
    }
}

void SingleGameState::combat(PlanetCell& yourCell,PlanetCell& enemyCell)
{
    int yourUnit = yourCell.myUnit;
    int enemyUnit = enemyCell.myUnit;
    if(yourUnit == enemyUnit && yourUnit != Unit_BOMBER){
        //Do Vector Battle TODO
    }
    else if(yourUnit == Unit_INFANTRY){
        retireUnit(yourCell);
    }
    else if(enemyUnit == Unit_INFANTRY){
        killEnemyUnit(enemyCell);
    }
    else if(yourUnit == Unit_TANK){
        if(enemyUnit == Unit_SCUD || enemyUnit == Unit_DESTROYER)
            killEnemyUnit(enemyCell);
        else if(enemyUnit == Unit_BOMBER || enemyUnit == Unit_SUBMARINE)
            retireUnit(yourCell);
    }
    else if(yourUnit == Unit_SCUD){
        if((enemyUnit >= 13 && enemyUnit <= 18) || enemyUnit == Unit_DESTROYER|| enemyUnit == Unit_FIGHTER)
            killEnemyUnit(enemyCell);
        else if(enemyUnit == Unit_TANK || enemyUnit == Unit_BOMBER)
            retireUnit(yourCell);
    }
    else if(yourUnit == Unit_SUBMARINE){
        if(enemyUnit >= 13 && enemyUnit <= 18 || enemyUnit == Unit_TANK || enemyUnit == Unit_BOMBER)
            killEnemyUnit(enemyCell);
        else if(enemyUnit == Unit_DESTROYER || enemyUnit == Unit_FIGHTER)
            retireUnit(yourCell);
    }
    else if(yourUnit == Unit_DESTROYER){
        if(enemyUnit == Unit_FIGHTER || enemyUnit == Unit_SUBMARINE)
            killEnemyUnit(enemyCell);
        else if(enemyUnit == Unit_SCUD || enemyUnit == Unit_DESTROYER)
            retireUnit(yourCell);
    }
    else if(yourUnit == Unit_BOMBER){
        if((enemyUnit >= 13 && enemyUnit <= 18) || enemyUnit == Unit_TANK || enemyUnit == Unit_SCUD)
            killEnemyUnit(enemyCell);
        else if(enemyUnit == Unit_FIGHTER || enemyUnit == Unit_SUBMARINE)
            retireUnit(yourCell);
    }
    else if(yourUnit == Unit_FIGHTER){
        if(enemyUnit == Unit_BOMBER || enemyUnit == Unit_SUBMARINE)
            killEnemyUnit(enemyCell);
        else if(enemyUnit == Unit_DESTROYER || enemyUnit == Unit_SCUD)
            retireUnit(yourCell);
    }
    else if(yourUnit >= 13 && yourUnit <= 18){
        if(enemyUnit == Unit_SCUD || enemyUnit == Unit_BOMBER || enemyUnit == Unit_SUBMARINE)
            retireUnit(yourCell);
    }
}

void SingleGameState::commandBaseChange(bool built, bool enemy)
{
    if(built)
    {
        commandBaseTotal++;
        myCommandBase++;
    }
    else if(enemy)
    {
        enemyCommandBase--;
    }
    else{
        myCommandBase--;
    }
    std::string commandMy;
    std::string commandEnemy;
    std::string commandTotal;
    std::string commandMax;
    std::stringstream out;
    out << commandBaseTotal;
    commandTotal = out.str();
    out.str( std::string() );
    out.clear();
    out << myCommandBase;
    commandMy = out.str();
    out.str( std::string() );
    out.clear();
    out << enemyCommandBase;
    commandEnemy = out.str();
    out.str( std::string() );
    out.clear();   
    out << commandBaseMax;
    commandMax = out.str();
    out.str( std::string() );
    out.clear(); 
    CEGUI::Window *resultWindow = CEGUI::WindowManager::getSingleton().getWindow("ProgressText1");
    if(myOwner == Owner_BLUE)
        resultWindow->setText("[colour='FF0000FF']Player Command Bases Remaining: "+commandMy+"/"+commandTotal+"");
    else
        resultWindow->setText("[colour='FFFF0000']Player Command Bases Remaining: "+commandMy+"/"+commandTotal+"");

    resultWindow = CEGUI::WindowManager::getSingleton().getWindow("ProgressText2");

    if(myOwner == Owner_BLUE)
        resultWindow->setText("[colour='FFFF0000']Enemy Command Bases Remaining: "+commandEnemy+"/"+commandMax+"");
    else
        resultWindow->setText("[colour='FF0000FF']Enemy Command Bases Remaining: "+commandEnemy+"/"+commandMax+"");
    if(commandBaseTotal == commandBaseMax && enemyCommandBase == 0){
        //YouWin
    }
    if(commandBaseTotal == commandBaseMax && myCommandBase == 0){
        //YouLose
    }
}
