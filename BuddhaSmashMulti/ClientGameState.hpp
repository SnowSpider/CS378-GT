//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef CLIENT_GAME_STATE_HPP
#define CLIENT_GAME_STATE_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "AppState.hpp"
 
#include "DotSceneLoader.hpp"
 
#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
#include "GameObject.h"
#include "SoundManager.h"
#include <vector>
using namespace std;

#include <SDL.h>
#include <SDL_net.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GRAVITY 9.80665

#define PORT 1234       // The port we are connecting to
#define BUFFER 512      // Size of message buffer

//|||||||||||||||||||||||||||||||||||||||||||||||
 
class ClientGameState : public AppState
{
public:
	ClientGameState();
 
	DECLARE_APPSTATE_CLASS(ClientGameState)
 
	void enter();
	void createScene();
	void exit();
	bool pause();
	void resume();
 
	void moveCamera();
	void getInput();
        void buildGUI();
 
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
 
	bool mouseMoved(const OIS::MouseEvent &arg);
	bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
 
	void onLeftPressed(const OIS::MouseEvent &evt);
        void itemSelected(OgreBites::SelectMenu* menu);
 
	void update(double timeSinceLastFrame);
 
private:
	Ogre::SceneNode*		m_pOgreHeadNode;
	Ogre::Entity*			m_pOgreHeadEntity;
	Ogre::MaterialPtr		m_pOgreHeadMat;
	Ogre::MaterialPtr		m_pOgreHeadMatHigh;
 
    OgreBites::ParamsPanel*		m_pDetailsPanel;
	bool				m_bQuit;
 
	Ogre::Vector3			m_TranslateVector;
	Ogre::Real			m_MoveSpeed;
	Ogre::Degree			m_RotateSpeed;
	float				m_MoveScale;
	Ogre::Degree			m_RotScale;
 
	Ogre::SceneNode*		m_pCurrentObject;
	Ogre::Entity*			m_pCurrentEntity;
	bool				m_bLMouseDown, m_bRMouseDown;
	bool				m_bSettingsMode;
	
	// stuff from Assignment 2
    int deltaX;
    int deltaY;
    int xi;
    int yi;
    GameObject* hostPaddle;
    GameObject* clientPaddle;
    GameObject* myBall;
    SoundManager sound;
    bool ready;
    Simulator simulator;
    vector<GameObject> m_objects; 
    float speed_default;
    float increment;
    CEGUI::OgreRenderer* mRenderer;
    int score;
    int highScore;
    bool soundIs;
    bool startUp;
    bool lost;
    
    /*
    IPaddress ip;		// Server address 
    TCPsocket sd;		// Socket descriptor 
    int quit, len;
    btVector3* buffer; // client paddle position
    string targetAddress;
    */
    
    // Variables
    const char *host;       // Where we store the host name
    IPaddress ipaddress;    // The IP we will connect to
    TCPsocket tcpsock;      // The socket to use
    string servername;      // The server name
    int result;
    unsigned short len;
    string msg;
    
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||
