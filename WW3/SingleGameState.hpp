//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef SINGLE_GAME_STATE_HPP
#define SINGLE_GAME_STATE_HPP
 
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

#define GRAVITY 9.80665

//|||||||||||||||||||||||||||||||||||||||||||||||
 
class SingleGameState : public AppState
{
public:
	SingleGameState();
 
	DECLARE_APPSTATE_CLASS(SingleGameState)
 
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
        bool soundSwitch();
        typedef enum terrainType{
            Terrain_WATER,
            Terrain_LAND
        };

        typedef enum ownerType{
            Owner_NEUTRAL,
            Owner_RED,
            Owner_BLUE
        };

        typedef enum radiationType{
            Radiation_SAFE,
            Radiation_LOW,
            Radiation_MODERATE,
            Radiation_HIGH,
            Radiation_DEADLY
        };

        typedef enum buildType{
            Build_NONE,
            Build_ICBMSILO,
            Build_NAVYBASE,
            Build_SUBMARINE,
            Build_DESTROYER,
            Build_AIRFORCEBASE,
            Build_BOMBER,
            Build_FIGHTER,
            Build_ENLIGHTENER,
            Build_COMMANDBASE
        };

        struct Cell {
            Cell( btVector3 p );
        
            // the position of the vert in the tile,
            // and the center of the hex in space
            btVector3 m_vertPos;		
            btVector3 m_nrm;

            bool visiblity;
            terrainType terrain;
            ownerType owner;
            radiationType radiation;
            buildType build;


            // Triangle that share this hex
            //std::vector<Face*> m_faces;	
        };
 
private:
	Ogre::SceneNode*		m_pOgreHeadNode;
	Ogre::Entity*			m_pOgreHeadEntity;
	Ogre::MaterialPtr		m_pOgreHeadMat;
	Ogre::MaterialPtr		m_pOgreHeadMatHigh;
	Ogre::SceneNode*		cameraNode;
 
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
    GameObject* myPaddle;
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
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||
