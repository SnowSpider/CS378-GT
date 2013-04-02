//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "MultiGameState.hpp"
#include <SDL.h>
#include <SDL_net.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
using namespace Ogre;
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
MultiGameState::MultiGameState()
{
    m_bQuit             = false;
    m_bQuestionActive   = false;
    m_FrameEvent        = Ogre::FrameEvent();
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void MultiGameState::enter()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Entering MultiGameState...");
 
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
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "CreateRoomBtn", "Create Room", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "JoinBtn", "Join", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "BackToMenuBtn", "Return to Menu", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createButton(OgreBites::TL_CENTER, "ExitBtn", "Exit AdvancedOgreFramework", 250);
    OgreFramework::getSingletonPtr()->m_pTrayMgr->createLabel(OgreBites::TL_TOP, "MultiLbl", "Multiplayer mode", 250);
 
    m_bQuit = false;
 
    createScene();
}

void MultiGameState::createScene()
{
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void MultiGameState::exit()
{
    OgreFramework::getSingletonPtr()->m_pLog->logMessage("Leaving MultiGameState...");
 
    m_pSceneMgr->destroyCamera(m_pCamera);
    if(m_pSceneMgr)
        OgreFramework::getSingletonPtr()->m_pRoot->destroySceneManager(m_pSceneMgr);
 
    OgreFramework::getSingletonPtr()->m_pTrayMgr->clearAllTrays();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->destroyAllWidgets();
    OgreFramework::getSingletonPtr()->m_pTrayMgr->setListener(0);
}

bool MultiGameState::keyPressed(const OIS::KeyEvent &keyEventRef)
{
    if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_ESCAPE) && !m_bQuestionActive)
    {
        OgreFramework::getSingletonPtr()->m_pTrayMgr->hideCursor();
        m_bQuit = true;
        return true;
    }
 
    OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool MultiGameState::keyReleased(const OIS::KeyEvent &keyEventRef)
{
    OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
 
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool MultiGameState::mouseMoved(const OIS::MouseEvent &evt)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseMove(evt)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool MultiGameState::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseDown(evt, id)) return true;
    return true;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
bool MultiGameState::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    if(OgreFramework::getSingletonPtr()->m_pTrayMgr->injectMouseUp(evt, id)) return true;
    return true;
}

void MultiGameState::update(double timeSinceLastFrame)
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
 
void MultiGameState::buttonHit(OgreBites::Button *button)
{
    if(button->getName() == "ExitBtn")
    {
        OgreFramework::getSingletonPtr()->m_pTrayMgr->showYesNoDialog("Sure?", "Really leave?");
        m_bQuestionActive = true;
    }
    else if(button->getName() == "BackToMenuBtn")
    {
        popAllAndPushAppState(findByName("MenuState"));
    }
    else if(button->getName() == "CreateRoomBtn")
    {
        // Create a room
	SDL_Init(SDL_INIT_EVERYTHING);
	TCPsocket sd, csd; /* Socket descriptor, Client socket descriptor */
	IPaddress ip, *remoteIP;
	int quit, quit2;
	char buffer[512];
 
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		//exit(EXIT_FAILURE);
	}
 
	/* Resolving the host using NULL make network interface to listen */
	if (SDLNet_ResolveHost(&ip, NULL, 1234) < 0)
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
 
	/* Wait for a connection, send data and term */
	quit = 0;
	while (!quit)
	{
		/* This check the sd if there is a pending connection.
		* If there is one, accept that, and open a new socket for communicating */
		if ((csd = SDLNet_TCP_Accept(sd)))
		{
			/* Now we can communicate with the client using csd socket
			* sd will remain opened waiting other connections */
 
			/* Get the remote address */
			if ((remoteIP = SDLNet_TCP_GetPeerAddress(csd)))
				/* Print the address, converting in the host format */
				printf("Host connected: %x %d\n", SDLNet_Read32(&remoteIP->host), SDLNet_Read16(&remoteIP->port));
			else
				fprintf(stderr, "SDLNet_TCP_GetPeerAddress: %s\n", SDLNet_GetError());
 
			quit2 = 0;
			while (!quit2)
			{
				if (SDLNet_TCP_Recv(csd, buffer, 512) > 0)
				{
					printf("Client say: %s\n", buffer);
 
					if(strcmp(buffer, "exit") == 0)	/* Terminate this connection */
					{
						quit2 = 1;
						printf("Terminate connection\n");
					}
					if(strcmp(buffer, "quit") == 0)	/* Quit the program */
					{
						quit2 = 1;
						quit = 1;
						printf("Quit program\n");
					}
				}
			}
 
			/* Close the client socket */
			SDLNet_TCP_Close(csd);
		}
	}
 
	SDLNet_TCP_Close(sd);
	SDLNet_Quit();
	
    }
    else if(button->getName() == "JoinBtn")
    {
        popAllAndPushAppState(findByName("JoinState"));
        
        /*
        // Join an existing room
	    SDL_Init(SDL_INIT_EVERYTHING);
	    SDLNet_Init();

	    IPaddress ip;
	    SDLNet_ResolveHost(&ip, "128.83.120.190", 1234);

	    TCPsocket client = SDLNet_TCP_Open(&ip);

	    char text[100];
	    SDLNet_TCP_Recv(client,text,100);
	    std::cout << text;

	    SDLNet_TCP_Close(client);		

	    SDLNet_Quit();
	    SDL_Quit();
	    */
    }
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void MultiGameState::yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit)
{
    if(yesHit == true)
        shutdown();
    else
        OgreFramework::getSingletonPtr()->m_pTrayMgr->closeDialog();
 
    m_bQuestionActive = false;
}

