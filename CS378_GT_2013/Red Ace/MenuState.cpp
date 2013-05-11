//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "MenuState.hpp"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
using namespace Ogre;
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
MenuState::MenuState()
{
    m_bQuit         = false;
    m_FrameEvent    = Ogre::FrameEvent();
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void MenuState::enter()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering MenuState...");
 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "MenuSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
    
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
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "EnterBtn", "Campaign", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "ManualBtn", "Field Manual", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "ExitBtn", "Exit Program", 250);
    
    createScene();
}

void MenuState::createScene()
{
    Ogre::Plane plane_z_neg_in(Ogre::Vector3::UNIT_Z, -200);
    
    Ogre::Real aspect = (Ogre::Real)OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth() / (Ogre::Real)OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight();
    
    // plane_z_neg_in
	Ogre::MeshManager::getSingleton().createPlane("plane_z_neg_in", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane_z_neg_in, aspect * 2, 2, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_Y);
	Ogre::Entity* plane6 = m_pSceneMgr->createEntity("ent_plane_z_neg_in", "plane_z_neg_in");
	plane6->setMaterialName("RedAce/Newspaper");
	plane6->setCastShadows(false);
	m_pSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(plane6);
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void MenuState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving MenuState...");
 
    m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
 
    OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);
}

bool MenuState::keyPressed(const OIS::KeyEvent &keyEventRef)
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
 
bool MenuState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool MenuState::mouseMoved(const OIS::MouseEvent &evt)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool MenuState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool MenuState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
    return true;
}

void MenuState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
 
    if(m_bQuit == true)
    {
        shutdown();
        return;
    }
}

void MenuState::buttonHit(OgreBites::Button *button)
{
    if(button->getName() == "ExitBtn")
        m_bQuit = true;
    else if(button->getName() == "EnterBtn")
        changeAppState(findByName("GameState"));
}
