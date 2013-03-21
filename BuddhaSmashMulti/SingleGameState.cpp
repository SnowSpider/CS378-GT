//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "SingleGameState.hpp"

#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

#include <iostream>
#include <sstream>
#include <math.h>

#include <btBulletCollisionCommon.h>

//|||||||||||||||||||||||||||||||||||||||||||||||
 
using namespace Ogre;
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
SingleGameState::SingleGameState()
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
 
void SingleGameState::enter()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering SingleGameState...");
 
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

bool SingleGameState::pause()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Pausing SingleGameState...");
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void SingleGameState::resume()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Resuming SingleGameState...");
 
    buildGUI();
 
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
    m_bQuit = false;
}

void SingleGameState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving SingleGameState...");
 
    m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
}

void SingleGameState::createScene()
{
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    
    // Create SoundManager
    sound.open();
    sound.turnOn();
    
    GameObject tempObject;
    
    // Create game objects
    tempObject = GameObject(m_pSceneMgr, &simulator, btVector3(0,0,0), "sphere.mesh", "Examples/PlasmaShit"); 
    tempObject.rootNode->attachObject(m_pSceneMgr->createParticleSystem("Smoke", "Examples/Smoke"));
    tempObject.scale(0.1, 0.1, 0.1);
    tempObject.mass = 1.0f; 
    tempObject.shape = new btSphereShape(10.0f);
    tempObject.addToSimulator();
    m_objects.push_back(tempObject);
    
    tempObject = GameObject(m_pSceneMgr, &simulator, btVector3(0,0,210), "cube.mesh", "MyMaterials/wood1");
    tempObject.scale(0.2, 0.2, 0.01);
    tempObject.mass = 1.0f;
    tempObject.shape = new btStaticPlaneShape(btVector3(0,0,-1), 0);
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
    tempObject = GameObject(m_pSceneMgr, &simulator, btVector3(0,0,200), "plane_z_pos_in", "Examples/Rockwall");
    tempObject.shape = new btStaticPlaneShape(btVector3(0,0,-1), 0);
    tempObject.addToSimulator();
    m_objects.push_back(tempObject);
    
    // negative z plane
    Ogre::MeshManager::getSingleton().createPlane("plane_z_neg_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_z_neg_in, 200.0f, 200.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_Y);
    tempObject = GameObject(m_pSceneMgr, &simulator, btVector3(0,0,-200), "plane_z_neg_in", "MyMaterials/buddha1");
    tempObject.shape = new btStaticPlaneShape(btVector3(0,0,1), 0);
    tempObject.addToSimulator();
    m_objects.push_back(tempObject);
	
    // Create a Light and set its position
    Ogre::Light* light = m_pSceneMgr->createLight("MainLight");
    light->setPosition(75.0f, 75.0f, 75.0f);
    light->setDiffuseColour(1,1,1);
    
    //CEGUI S  HIT(Stuff Hard to Improve Technically)
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
    
    GameObject* myBall = &m_objects[0];
    myBall->tr = btTransform(btMatrix3x3(1,0,0,0,1,0,0,0,1),btVector3(r1, r2, -500));
    myBall->updateTransform();
    
    myBall->direction = btVector3(r1, r2, -500);
    myBall->direction.normalize();
    myBall->speed = speed_default;
    myBall->setPosition(btVector3(0,0,0));
}

bool SingleGameState::keyPressed(const OIS::KeyEvent &keyEventRef)
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
 
bool SingleGameState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}

bool SingleGameState::mouseMoved(const OIS::MouseEvent &evt)
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

//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool SingleGameState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
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
    if(CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id))) return true;
    //mCameraMan->injectMouseDown(evt, id);
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool SingleGameState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
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
    
    if(CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id))) return true;
 
    return true;
}

void SingleGameState::onLeftPressed(const OIS::MouseEvent &evt)
{
    if(m_pCurrentObject)
    {
        m_pCurrentObject->showBoundingBox(false);
        m_pCurrentEntity->getSubEntity(1)->setMaterial(m_pOgreHeadMat);
    }
    
    /*
    Ogre::Ray mouseRay = m_pCamera->getCameraToViewportRay(OgreFramework::getSingletonPtr()->m_pMouse->getMouseState().X.abs / float(evt.state.width),
        OgreFramework::getSingletonPtr()->m_pMouse->getMouseState().Y.abs / float(evt.state.height));
    m_pRSQ->setRay(mouseRay);
    m_pRSQ->setSortByDistance(true);
 
    Ogre::RaySceneQueryResult &result = m_pRSQ->execute();
    Ogre::RaySceneQueryResult::iterator itr;
    
    for(itr = result.begin(); itr != result.end(); itr++)
    {
        if(itr->movable)
        {
            OgreFramework::getSingletonPtr()->m_pLog->logMessage("MovableName: " + itr->movable->getName());
            m_pCurrentObject = m_pSceneMgr->getEntity(itr->movable->getName())->getParentSceneNode();
            OgreFramework::getSingletonPtr()->m_pLog->logMessage("ObjName " + m_pCurrentObject->getName());
            m_pCurrentObject->showBoundingBox(true);
            m_pCurrentEntity = m_pSceneMgr->getEntity(itr->movable->getName());
            m_pCurrentEntity->getSubEntity(1)->setMaterial(m_pOgreHeadMatHigh);
            break;
        }
    }
    */
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

void SingleGameState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame / 1000.0;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
 
    if(m_bQuit == true)
    {
        popAppState();
        return;
    }
 
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
 
    m_MoveScale = m_MoveSpeed   * timeSinceLastFrame;
    m_RotScale  = m_RotateSpeed * timeSinceLastFrame;
 
    m_TranslateVector = Vector3::ZERO;
 
    getInput();
    moveCamera();
    
    // stuff from Assignment 2
    GameObject* myBall = &m_objects[0];
    GameObject* myPaddle = &m_objects[1];
    GameObject* myWall_x_pos = &m_objects[2];
    GameObject* myWall_x_neg = &m_objects[3];
    GameObject* myWall_y_pos = &m_objects[4];
    GameObject* myWall_y_neg = &m_objects[5];
    GameObject* myWall_z_pos = &m_objects[6];
    GameObject* myWall_z_neg = &m_objects[7];
    
    // Apply gravity
    myBall->direction += btVector3(0, -(0.00001*GRAVITY), 0);
    myBall->direction.normalize();
    
    // Move the ball
    if (!ready){
        //myBall->simulator->stepSimulation(1);
        myBall->move(m_FrameEvent);
    }
    
    // Move the paddle
    myPaddle->direction = btVector3(deltaX, deltaY, 0);
    myPaddle->speed = sqrt(deltaX*deltaX + deltaY*deltaY);
    myPaddle->setPosition(btVector3(xi, yi, 210));
    //myPaddle->move(evt);
    
    if (myPaddle->position.x() > 90){
        myPaddle->setPosition(btVector3(90, myPaddle->position.y(), myPaddle->position.z()));
    }
    if (myPaddle->position.x() < -90){
        myPaddle->setPosition(btVector3(-90, myPaddle->position.y(), myPaddle->position.z()));
    }
    if (myPaddle->position.y() > 90){
        myPaddle->setPosition(btVector3(myPaddle->position.x(), 90, myPaddle->position.z()));
    }
    if (myPaddle->position.y() < -90){
        myPaddle->setPosition(btVector3(myPaddle->position.x(), -90, myPaddle->position.z()));
    }
    
    //cout << myPaddle->direction << endl;
    //cout << myPaddle->speed << endl;
    
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
        if (abs(myPaddle->position.x() - myBall->position.x()) <= 25 && abs(myPaddle->position.y() - myBall->position.y()) <= 25){
            myBall->direction += (0.2 * myPaddle->direction);
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
        else if (abs(myPaddle->nextPosition.x() - myBall->nextPosition.x()) <= 25 && abs(myPaddle->nextPosition.y() - myBall->nextPosition.y()) <= 25){
            myBall->direction += (0.2 * myPaddle->direction);
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
    
    //Need to capture/update each device
    OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
    OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
}

void SingleGameState::buildGUI()
{
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOP, "GameLbl", "Singleplayer Game mode", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();
 
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
    
    /*
    Ogre::String infoText = "[TAB] - Switch input mode\n\n[W] - Forward / Mode up\n[S] - Backwards/ Mode down\n[A] - Left\n";
    infoText.append("[D] - Right\n\nPress [SHIFT] to move faster\n\n[O] - Toggle FPS / logo\n");
    infoText.append("[Print] - Take screenshot\n\n[ESC] - Exit");
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createTextBox(OgreBites::TL_RIGHT, "InfoPanel", infoText, 300, 220);
    */
    
    Ogre::StringVector chatModes;
    chatModes.push_back("Solid mode");
    chatModes.push_back("Wireframe mode");
    chatModes.push_back("Point mode");
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createLongSelectMenu(OgreBites::TL_TOPRIGHT, "ChatModeSelMenu", "ChatMode", 200, 3, chatModes);
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




