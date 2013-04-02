//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "JoinState.hpp"
#include <SDL.h>
#include <SDL_net.h>
#include <cstring>
 
//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||
 
JoinState::JoinState()
{
    m_bQuit             = false;
    m_bQuestionActive   = false;
    m_FrameEvent        = Ogre::FrameEvent();
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||

void JoinState::enter()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering JoinState...");
 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "PauseSceneMgr");
    m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
 
    m_pCamera = m_pSceneMgr->createCamera("PauseCam");
    m_pCamera->setPosition(Vector3(0, 25, -50));
    m_pCamera->lookAt(Vector3(0, 0, 0));
    m_pCamera->setNearClipDistance(1);
 
    m_pCamera->setAspectRatio(Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualWidth()) /
        Real(OgreFramework::getSingletonPtr()->m_pViewport->getActualHeight()));
 
    OgreFramework::getSingletonPtr()->m_pViewport->setCamera(m_pCamera);
 
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->showCursor();
    
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_BOTTOM, "ConnectBtn", "Connect", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_BOTTOM, "CencelBtn", "Cancel", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOP, "MultiLbl", "Type the IP address", 250);
 
    m_bQuit = false;
 
    createScene();
}

void JoinState::createScene()
{
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    
    //CEGUI SHIT(Stuff Hard to Improve Technically)
    CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
 
    CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
 
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "CEGUIDemo/Sheet");

    CEGUI::Window *starter = wmgr.createWindow("TaharezLook/StaticText", "StartingWindow");
    starter->setText(""); // Type in the IP address
    starter->setSize(CEGUI::UVector2(CEGUI::UDim(0.30, 0), CEGUI::UDim(0.10, 0)));
    starter->setPosition(CEGUI::UVector2(CEGUI::UDim(0.35f, 0), CEGUI::UDim(0.45f, 0 )));
 
    sheet->addChildWindow(starter);
    
    CEGUI::System::getSingleton().setGUISheet(sheet);
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void JoinState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving JoinState...");
 
    m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
 
    OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);
    
    mRenderer->destroySystem();
}

bool JoinState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE) && !m_bQuestionActive)
    {
        OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();
        m_bQuit = true;
        return true;
    }
    
    if( targetAddress.injectKeyPress( keyEventRef ) == true )
    {
        CEGUI::Window* starter = CEGUI::WindowManager::getSingleton().getWindow("StartingWindow");
        starter->setText( targetAddress.getText() );
    }
    
    
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool JoinState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool JoinState::mouseMoved(const OIS::MouseEvent &evt)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool JoinState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool JoinState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
    return true;
}

void JoinState::update(double timeSinceLastFrame)
{
    m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
    OgreFramework::getSingletonPtr()->m_pTrayMgr->frameRenderingQueued(m_FrameEvent);
    
    if(m_bQuit == true)
    {
        popAppState();
        return;
    }
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void JoinState::buttonHit(OgreBites::Button *button)
{
    if(button->getName() == "CencelBtn")
    {
        popAllAndPushAppState(findByName("MultiGameState"));
    }
    if(button->getName() == "ConnectBtn")
    {
        // TODO
        // connect to the targetAddress (string)
    }
    
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void JoinState::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit)
{
    if(yesHit == true)
        shutdown();
    else
        OgreFramework::getSingletonPtr()->m_pTrayMgr->closeDialog();
 
    m_bQuestionActive = false;
}

