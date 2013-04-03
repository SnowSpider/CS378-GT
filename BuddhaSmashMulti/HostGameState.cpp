//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "HostGameState.hpp"

#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

#include <iostream>
#include <sstream>
#include <math.h>

#include <btBulletCollisionCommon.h>

//|||||||||||||||||||||||||||||||||||||||||||||||
 
using namespace Ogre;
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
HostGameState::HostGameState()
{
    m_MoveSpeed		= 0.1f;
    m_RotateSpeed	= 0.3f;
 
    m_bLMouseDown       = false;
    m_bRMouseDown       = false;
    m_bQuit             = false;
    m_bSettingsMode     = false;
 
    m_pDetailsPanel	= 0;
    
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
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void HostGameState::enter()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering HostGameState...");
    
    // Connect : begin
    
    
    // Create a room
    SDL_Init(SDL_INIT_EVERYTHING);
    /*
    TCPsocket sd, csd; // Socket descriptor, Client socket descriptor 
    IPaddress ip, *remoteIP;
    int quit, quit2;
    char buffer[512];
    */
    
    if (SDLNet_Init() < 0)
    {
	    fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
	    //exit(EXIT_FAILURE);
    }

    // Resolving the host using NULL make network interface to listen 
    if (SDLNet_ResolveHost(&ip, NULL, 1234) < 0)
    {
	    fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
	    //exit(EXIT_FAILURE);
    }

    // Open a connection with the IP provided (listen on the host's port) 
    if (!(sd = SDLNet_TCP_Open(&ip)))
    {
	    fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
	    //exit(EXIT_FAILURE);
    }
    
    // Wait for a connection, send data and term 
    quit = 0;
    
    
    // This check the sd if there is a pending connection.
    // If there is one, accept that, and open a new socket for communicating 
    if ((csd = SDLNet_TCP_Accept(sd)))
    {
	    // Now we can communicate with the client using csd socket
	    // sd will remain opened waiting other connections 

	    // Get the remote address 
	    if ((remoteIP = SDLNet_TCP_GetPeerAddress(csd)))
		    // Print the address, converting in the host format 
		    printf("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
	    else
		    fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
    }
    
    // Connect : end
    
    
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "GameSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
 
    m_pCamera = m_pSceneMgr->createCamera("GameCamera");
    m_pCamera->setPosition(0,0,450); 
    m_pCamera->lookAt(Vector3(0, 0, 0));
    m_pCamera->setNearClipDistance(5);
 
    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));
 
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
    m_pCurrentObject = 0;
 
    buildGUI();
 
    createScene();
}

bool HostGameState::pause()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Pausing HostGameState...");
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void HostGameState::resume()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Resuming HostGameState...");
 
    buildGUI();
 
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
    m_bQuit = false;
}

void HostGameState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving HostGameState...");
 
    m_pSceneMgr->destroyCamera(m_pCamera);
    //m_pSceneMgr->destroyQuery(m_pRSQ);
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
    
    sound.close();
    
    SDLNet_TCP_Close(csd);
    SDLNet_TCP_Close(sd);
    SDLNet_Quit(); 
}

void HostGameState::createScene()
{
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    
    // Create SoundManager
    sound.open();
    sound.turnOn();
    
    GameObject tempObject;
    
    // Create game objects
    // Ball
    tempObject = GameObject(m_pSceneMgr, &simulator, btVector3(0,0,0), "sphere.mesh", "Examples/PlasmaShit"); 
    tempObject.rootNode->attachObject(m_pSceneMgr->createParticleSystem("Smoke", "Examples/Smoke"));
    tempObject.scale(0.1, 0.1, 0.1);
    tempObject.mass = 1.0f; 
    tempObject.shape = new btSphereShape(10.0f);
    tempObject.addToSimulator();
    m_objects.push_back(tempObject);
    
    // Host's paddle
    tempObject = GameObject(m_pSceneMgr, &simulator, btVector3(0,0,200), "cube.mesh", "MyMaterials/wood1");
    tempObject.scale(0.2, 0.2, 0.01);
    tempObject.mass = 1.0f;
    tempObject.shape = new btStaticPlaneShape(btVector3(0,0,-1), 0);
    tempObject.addToSimulator();
    m_objects.push_back(tempObject);
    
    // Client's paddle
    tempObject = GameObject(m_pSceneMgr, &simulator, btVector3(0,0,-200), "cube.mesh", "MyMaterials/wood1");
    tempObject.scale(0.2, 0.2, 0.01);
    tempObject.mass = 1.0f;
    tempObject.shape = new btStaticPlaneShape(btVector3(0,0,1), 0);
    tempObject.addToSimulator();
    m_objects.push_back(tempObject);
    
    Ogre::Plane plane_x_pos_in(Ogre::Vector3::NEGATIVE_UNIT_X, 0);
    Ogre::Plane plane_x_neg_in(Ogre::Vector3::UNIT_X, 0);
    Ogre::Plane plane_y_pos_in(Ogre::Vector3::NEGATIVE_UNIT_Y, 0);
    Ogre::Plane plane_y_neg_in(Ogre::Vector3::UNIT_Y, 0);
    Ogre::Plane plane_z_pos_in(Ogre::Vector3::NEGATIVE_UNIT_Z, 0);
    Ogre::Plane plane_z_neg_in(Ogre::Vector3::UNIT_Z, 0);
    
    // positive x plane
    Ogre::MeshManager::getSingleton().createPlane("plane_x_pos_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_x_pos_in, 400.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_Y);
    tempObject = GameObject(m_pSceneMgr, &simulator, btVector3(100,0,0), "plane_x_pos_in", "Examples/Rockwall");
    tempObject.shape = new btStaticPlaneShape(btVector3(-1,0,0), 0);
    tempObject.addToSimulator();
    m_objects.push_back(tempObject);
    
    // negative x plane
    Ogre::MeshManager::getSingleton().createPlane("plane_x_neg_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_x_neg_in, 400.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_Y);
    tempObject = GameObject(m_pSceneMgr, &simulator, btVector3(-100,0,0), "plane_x_neg_in", "Examples/Rockwall");
    tempObject.shape = new btStaticPlaneShape(btVector3(1,0,0), 0);
    tempObject.addToSimulator();
    m_objects.push_back(tempObject);
    
    // positive y plane
    Ogre::MeshManager::getSingleton().createPlane("plane_y_pos_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_y_pos_in, 400.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_X);
    tempObject = GameObject(m_pSceneMgr, &simulator, btVector3(0,100,0), "plane_y_pos_in", "MyMaterials/wood3");
    tempObject.shape = new btStaticPlaneShape(btVector3(0,-1,0), 0);
    tempObject.addToSimulator();
    m_objects.push_back(tempObject);
    
    // negative y plane
    Ogre::MeshManager::getSingleton().createPlane("plane_y_neg_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_y_neg_in, 400.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_X);
    tempObject = GameObject(m_pSceneMgr, &simulator, btVector3(0,-100,0), "plane_y_neg_in", "MyMaterials/wood3");
    tempObject.shape = new btStaticPlaneShape(btVector3(0,1,0), 0);
    tempObject.addToSimulator();
    m_objects.push_back(tempObject);
    
    // positive z plane
    Ogre::MeshManager::getSingleton().createPlane("plane_z_pos_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_z_pos_in, 200.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_Y);
    tempObject = GameObject(m_pSceneMgr, &simulator, btVector3(0,0,200), "plane_z_pos_in", "Examples/buddha1");
    tempObject.shape = new btStaticPlaneShape(btVector3(0,0,-1), 0);
    tempObject.addToSimulator();
    m_objects.push_back(tempObject);
    
    // negative z plane
    Ogre::MeshManager::getSingleton().createPlane("plane_z_neg_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_z_neg_in, 200.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_Y);
    tempObject = GameObject(m_pSceneMgr, &simulator, btVector3(0,0,-200), "plane_z_neg_in", "Examples/buddha1");
    tempObject.shape = new btStaticPlaneShape(btVector3(0,0,1), 0);
    tempObject.addToSimulator();
    m_objects.push_back(tempObject);
	
    // Create a Light and set its position
    Ogre::Light* light = m_pSceneMgr->createLight("MainLight");
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
    
    // stuff from createFrameListener
    
    // Backgroun Music
    sound.PlaySound("buddhist_chant.wav");
    
    srand(time(0));  // Initialize random number generator.
    // Set default values for variables
    float r1 = (rand() % 100);
    float r2 = (rand() % 100);
    float r3 = (rand() % 100);
    
    myBall = &m_objects[0];
    myBall->tr = btTransform(btMatrix3x3(1,0,0,0,1,0,0,0,1),btVector3(r1, r2, -500));
    myBall->updateTransform();
    
    myBall->direction = btVector3(r1, r2, -500);
    myBall->direction.normalize();
    myBall->speed = speed_default;
    myBall->setPosition(btVector3(0,0,0));
}

bool HostGameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(m_bSettingsMode == true)
    {
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S))
        {
            OgreBites::SelectMenu* pMenu = (OgreBites::SelectMenu*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("ChatModeSelMenu");
            if(pMenu->getSelectionIndex() + 1 < (int)pMenu->getNumItems())
                pMenu->selectItem(pMenu->getSelectionIndex() + 1);
        }
 
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W))
        {
            OgreBites::SelectMenu* pMenu = (OgreBites::SelectMenu*)OgreFramework::getSingletonPtr()->m_pTrayMgr->getWidget("ChatModeSelMenu");
            if(pMenu->getSelectionIndex() - 1 >= 0)
                pMenu->selectItem(pMenu->getSelectionIndex() - 1);
        }
    }
 
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        pushAppState(findByName("PauseState"));
        OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();
        return true;
    }
 
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_I))
    {
        if(m_pDetailsPanel->getTrayLocation() == OgreBites::TL_NONE)
        {
            OgreFramework::getSingletonPtr()->m_pTrayMgr->moveWidgetToTray(m_pDetailsPanel, OgreBites::TL_TOPLEFT, 0);
            m_pDetailsPanel->show();
        }
        else
        {
            OgreFramework::getSingletonPtr()->m_pTrayMgr->removeWidgetFromTray(m_pDetailsPanel);
            m_pDetailsPanel->hide();
        }
    }
 
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_TAB))
    {
        m_bSettingsMode = !m_bSettingsMode;
        return true;
    }
 
    if(m_bSettingsMode && OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_RETURN) ||
        OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_NUMPADENTER))
    {
    }
 
    if(!m_bSettingsMode || (m_bSettingsMode && !OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_O)))
        OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool HostGameState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}

bool HostGameState::mouseMoved(const OIS::MouseEvent &evt)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
    
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
    return true;
    
    /*
 
    if(m_bRMouseDown)
    {
        m_pCamera->yaw(Degree(evt.state.X.rel * -0.1f));
        m_pCamera->pitch(Degree(evt.state.Y.rel * -0.1f));
    }
 
    return true;
    */
}

//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool HostGameState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
    
    switch (id) {
        case OIS::MB_Left:
            onLeftPressed(evt);
            m_bLMouseDown = true;
            //cout << "Left Click" << endl;
            if (ready){
                ready = false;
            }
            OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();
            break;
        case OIS::MB_Right: 
            m_bRMouseDown = true;
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
    //mCameraMan->injectMouseDown(evt, id);
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool HostGameState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
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

void HostGameState::onLeftPressed(const OIS::MouseEvent &evt)
{
    if(m_pCurrentObject)
    {
        m_pCurrentObject->showBoundingBox(false);
        m_pCurrentEntity->getSubEntity(1)->setMaterial(m_pOgreHeadMat);
    }
}

void HostGameState::moveCamera()
{
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_LSHIFT))
        m_pCamera->moveRelative(m_TranslateVector);
    m_pCamera->moveRelative(m_TranslateVector / 10);
}

void HostGameState::getInput()
{
    if(m_bSettingsMode == false)
    {
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A))
            m_TranslateVector.x = -m_MoveScale;
 
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D))
            m_TranslateVector.x = m_MoveScale;
 
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W))
            m_TranslateVector.z = -m_MoveScale;
 
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S))
            m_TranslateVector.z = m_MoveScale;
 
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_Q))
            m_TranslateVector.y = -m_MoveScale;
 
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_E))
            m_TranslateVector.y = m_MoveScale;
 
        //camera roll
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_Z))
            m_pCamera->roll(Angle(-m_MoveScale));
 
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_X))
            m_pCamera->roll(Angle(m_MoveScale));
 
        //reset roll
        if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_C))
            m_pCamera->roll(-(m_pCamera->getRealOrientation().getRoll()));
    }
}

void HostGameState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame / 1000.0;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
    
    len = sizeof(btVector3);
    
    if (SDLNet_TCP_Recv(csd, buffer, len) > 0) // you can receive anything here
    {
        std::memcpy(clientPaddle->position, buffer, sizeof(btVector3));
    }
    
    
    if(m_bQuit == true)
    {
        popAppState();
        return;
    }
    /*
    if(!OgreFramework::getSingletonPtr()->m_pTrayMgr->isDialogVisible())
    {
        if(m_pDetailsPanel->isVisible())
        {
            m_pDetailsPanel->setParamValue(0, Ogre::StringConverter::toString(m_pCamera->getDerivedPosition().x));
            m_pDetailsPanel->setParamValue(1, Ogre::StringConverter::toString(m_pCamera->getDerivedPosition().y));
            m_pDetailsPanel->setParamValue(2, Ogre::StringConverter::toString(m_pCamera->getDerivedPosition().z));
            m_pDetailsPanel->setParamValue(3, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().w));
            m_pDetailsPanel->setParamValue(4, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().x));
            m_pDetailsPanel->setParamValue(5, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().y));
            m_pDetailsPanel->setParamValue(6, Ogre::StringConverter::toString(m_pCamera->getDerivedOrientation().z));
            if(m_bSettingsMode)
                m_pDetailsPanel->setParamValue(7, "Buffered Input");
            else
                m_pDetailsPanel->setParamValue(7, "Un-Buffered Input");
        }
    }
    */
    
    m_MoveScale = m_MoveSpeed   * timeSinceLastFrame;
    m_RotScale  = m_RotateSpeed * timeSinceLastFrame;
 
    m_TranslateVector = Vector3::ZERO;
 
    getInput();
    moveCamera();
    
    // stuff from Assignment 2
    myBall = &m_objects[0];
    hostPaddle = &m_objects[1];
    clientPaddle = &m_objects[2];
    GameObject* myWall_x_pos = &m_objects[3];
    GameObject* myWall_x_neg = &m_objects[4];
    GameObject* myWall_y_pos = &m_objects[5];
    GameObject* myWall_y_neg = &m_objects[6];
    GameObject* myWall_z_pos = &m_objects[7];
    GameObject* myWall_z_neg = &m_objects[8];
    
    // Apply gravity
    myBall->direction += btVector3(0, -(0.00001*GRAVITY), 0);
    myBall->direction.normalize();
    
    // Move the ball
    if (!ready){
        //myBall->simulator->stepSimulation(1);
        myBall->move(m_FrameEvent);
    }
    
    // Move the paddle
    hostPaddle->direction = btVector3(deltaX, deltaY, 0);
    hostPaddle->speed = sqrt(deltaX*deltaX + deltaY*deltaY);
    hostPaddle->setPosition(btVector3(xi, yi, 200));
    //hostPaddle->move(evt);
    
    if (hostPaddle->position.x() > 90){
        hostPaddle->setPosition(btVector3(90, hostPaddle->position.y(), hostPaddle->position.z()));
    }
    if (hostPaddle->position.x() < -90){
        hostPaddle->setPosition(btVector3(-90, hostPaddle->position.y(), hostPaddle->position.z()));
    }
    if (hostPaddle->position.y() > 90){
        hostPaddle->setPosition(btVector3(hostPaddle->position.x(), 90, hostPaddle->position.z()));
    }
    if (hostPaddle->position.y() < -90){
        hostPaddle->setPosition(btVector3(hostPaddle->position.x(), -90, hostPaddle->position.z()));
    }
    
    //cout << hostPaddle->direction << endl;
    //cout << hostPaddle->speed << endl;
    
    /*
    Reflection vector formula:
    
    L = light
    R = reflection
    
    R = 2(N dot L)N - L
    
    */
    
    //myBall->nextPosition.x() >= 90-myBall->position.x()
    
    // The ball hits the positive x plane
    if (myBall->position.x() > 90 || myBall->nextPosition.x() > 90){
        myBall->setPosition(btVector3(90, myBall->position.y(), myBall->position.z()));
        myBall->direction += (-2.0 * (btVector3(-1,0,0).dot(myBall->direction)) * btVector3(-1,0,0));
        //cout << "hit!" << endl;
	    sound.PlaySound("boink1_cx65377.wav");
    }
    
    // The ball hits the negative x plane
    else if (myBall->position.x() < -90 || myBall->nextPosition.x() < -90){
        myBall->setPosition(btVector3(-90, myBall->position.y(), myBall->position.z()));
        myBall->direction += (-2.0 * (btVector3(1,0,0).dot(myBall->direction)) * btVector3(1,0,0));
        //cout << "hit!" << endl;
	    sound.PlaySound("boink1_cx65377.wav");
    }
    
    // The ball hits the positive y plane
    else if (myBall->position.y() > 90 || myBall->nextPosition.y() > 90){
        myBall->setPosition(btVector3(myBall->position.x(), 90, myBall->position.z()));
        myBall->direction += (-2.0 * (btVector3(0,-1,0).dot(myBall->direction)) * btVector3(0,-1,0));
        //cout << "hit!" << endl;
	    sound.PlaySound("boink1_cx65377.wav");
    }
    
    // The ball hits the negative y plane
    else if (myBall->position.y() < -90 || myBall->nextPosition.y() < -90){
        myBall->setPosition(btVector3(myBall->position.x(), -90, myBall->position.z()));
        myBall->direction += (-2.0 * (btVector3(0,1,0).dot(myBall->direction)) * btVector3(0,1,0));
        //cout << "hit!" << endl;
	    sound.PlaySound("boink1_cx65377.wav");
    }
    
    // The ball hits the positive z plane
    else if (myBall->position.z() > 190 || myBall->nextPosition.z() > 190){
        myBall->setPosition(btVector3(myBall->position.x(), myBall->position.y(), 190));
        myBall->direction += (-2.0 * (btVector3(0,0,-1).dot(myBall->direction)) * btVector3(0,0,-1));
        if (abs(hostPaddle->position.x() - myBall->position.x()) <= 25 && abs(hostPaddle->position.y() - myBall->position.y()) <= 25){
            myBall->direction += (0.2 * hostPaddle->direction);
            myBall->direction.normalize();
            score += myBall->speed;
            myBall->speed += increment;
            CEGUI::Window* counter = CEGUI::WindowManager::getSingleton().getWindow("Counter");
            std::string shownScore;
            std::stringstream out;
            out << score;
            shownScore = out.str();
            counter->setText("Score: " + shownScore);
            //cout << "HIT" << endl;
            sound.PlaySound("boink1_cx65377.wav");
        }
        else if (abs(hostPaddle->nextPosition.x() - myBall->nextPosition.x()) <= 25 && abs(hostPaddle->nextPosition.y() - myBall->nextPosition.y()) <= 25){
            myBall->direction += (0.2 * hostPaddle->direction);
            myBall->direction.normalize();
            score += myBall->speed;
            myBall->speed += increment;
            CEGUI::Window* counter = CEGUI::WindowManager::getSingleton().getWindow("Counter");
            std::string shownScore;
            std::stringstream out;
            out << score;
            shownScore = out.str();
            counter->setText("Score: " + shownScore);
            //cout << "HIT" << endl;
	        sound.PlaySound("boink1_cx65377.wav");
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
            myBall->direction = btVector3(r1, r2, -1000);
            myBall->direction.normalize();
            myBall->speed = speed_default;
            myBall->setPosition(btVector3(0,0,0));
        }
        //cout << "hit!" << endl;
    }
    
    // The ball hits the negative z plane
    else if (myBall->position.z() < -190 || myBall->nextPosition.z() < -190){
        myBall->setPosition(btVector3(myBall->position.x(), myBall->position.y(), -190));
        myBall->direction += (-2.0 * (btVector3(0,0,1).dot(myBall->direction)) * btVector3(0,0,1));
        //cout << "hit!" << endl;
	    sound.PlaySound("boink1_cx65377.wav");
	    myBall->speed -= 10;
    }
    
    /*
    if(mWindow->isClosed())
        return false;
    
    if(mShutDown)
        return false;
    */
    
    
    // Send message
    
    len = sizeof(btVector3);
    
    buffer = &(hostPaddle->position);
    
    if (SDLNet_TCP_Send(sd, (void *)buffer, len) < len) // you can send anything here
    {
	    fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
	    //exit(EXIT_FAILURE);
    }
    
    
    //Need to capture/update each device
    OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
}

void HostGameState::buildGUI()
{
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOP, "GameLbl", "Host Game mode", 250);
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
    
    Ogre::StringVector chatModes;
    chatModes.push_back("Solid mode");
    chatModes.push_back("Wireframe mode");
    chatModes.push_back("Point mode");
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createLongSelectMenu(OgreBites::TL_TOPRIGHT, "ChatModeSelMenu", "ChatMode", 200, 3, chatModes);
}

void HostGameState::itemSelected(OgreBites::SelectMenu* menu)
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




