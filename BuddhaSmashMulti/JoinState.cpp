//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "JoinState.hpp"
#include <SDL.h>
#include <SDL_net.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
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
 
    m_pSceneMgr = OgreFramework::getSingletonPtr()->m_pRoot->createSceneManager(ST_GENERIC, "JoinSceneMgr");
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
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_BOTTOM, "CancelBtn", "Cancel", 250);
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
    if(button->getName() == "CancelBtn")
    {
        popAllAndPushAppState(findByName("MultiGameState"));
    }
    if(button->getName() == "ConnectBtn")
    {
        // TODO
	// targetAddress.getText()
        // connect to the targetAddress (string)
	SDL_Init(SDL_INIT_EVERYTHING);
	IPaddress ip;		/* Server address */
	TCPsocket sd;		/* Socket descriptor */
	int quit, len;
	char buffer[512];
 
	/* Simple parameter checking */
 
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		//exit(EXIT_FAILURE);
	}
 
	/* Resolve the host we are connecting to */
	if (SDLNet_ResolveHost(&ip, targetAddress.getText().c_str(), 1234) < 0)
	{
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		//exit(EXIT_FAILURE);
	}
 
	/* Open a connection with the IP provided (listen on the host's port) */
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		//exit(EXIT_FAILURE);
	}
 
	/* Send messages */
	quit = 0;
	while (!quit)
	{
		printf("Write something:\n>");
		scanf("%s", buffer);
 
		len = strlen(buffer) + 1;
		if (SDLNet_TCP_Send(sd, (void *)buffer, len) < len)
		{
			fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			//exit(EXIT_FAILURE);
		}
 
		if(strcmp(buffer, "exit") == 0)
			quit = 1;
		if(strcmp(buffer, "quit") == 0)
			quit = 1;
	}
 
	SDLNet_TCP_Close(sd);
	SDLNet_Quit();
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

