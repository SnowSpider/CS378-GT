//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "MenualState.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||
 
using namespace Ogre;
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
MenualState::MenualState()
{
    m_bQuit         = false;
    m_FrameEvent    = Ogre::FrameEvent();
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void MenualState::enter()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering MenualState...");
 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "MenualSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
    
    m_pCamera = m_pSceneMgr->createCamera("MenualCam");
    m_pCamera->setPosition(Vector3(0, 0, 100));
    m_pCamera->lookAt(Vector3(0, 0, 0));
    
    // Orthographic projection
    m_pCamera->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
    Ogre::Real aspect = (Ogre::Real)OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth() / (Ogre::Real)OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight();
    m_pCamera->setAspectRatio(aspect);
    m_pCamera->setNearClipDistance(1);
    m_pCamera->setOrthoWindow(aspect * 2, 2);
    // Orthographic projection
    
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
 
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showLogo(OgreBites::TL_BOTTOMRIGHT);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "EnterBtn", "Enter GameState", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "ExitBtn", "Exit AdvancedOgreFramework", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOP, "MenualLbl", "Menual mode", 250);
 
    createScene();
}

void MenualState::createScene()
{
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void MenualState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving MenualState...");
 
    m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
 
    OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);
}

bool MenualState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
    {
        m_bQuit = true;
        return true;
    }
 
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool MenualState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool MenualState::mouseMoved(const OIS::MouseEvent &evt)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool MenualState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool MenualState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
    return true;
}

void MenualState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
 
    if(m_bQuit == true)
    {
        shutdown();
        return;
    }
}

void MenualState::buttonHit(OgreBites::Button *button)
{
    if(button->getName() == "ExitBtn")
        m_bQuit = true;
    else if(button->getName() == "EnterBtn")
        changeAppState(findByName("GameState"));
}
