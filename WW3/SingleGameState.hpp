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
#include "Planet.h"
#include <btBulletCollisionCommon.h>
#include <Times.h>

#include <iostream>
#include <sstream>
#include <math.h>

#include <string.h>
#include "Unit.h"

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
 
    void deselect();
    void onLeftPressed(const OIS::MouseEvent &evt);
    void onRightPressed(const OIS::MouseEvent &evt);
        void itemSelected(OgreBites::SelectMenu* menu);
 
    void update(double timeSinceLastFrame);
        CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);
        bool soundButton(const CEGUI::EventArgs &e);
    bool soundSwitch();
    
    bool CommandBaseButton(const CEGUI::EventArgs &e);
    bool ArmyBaseButton(const CEGUI::EventArgs &e);
    bool NavyBaseButton(const CEGUI::EventArgs &e);
    bool AirForceBaseButton(const CEGUI::EventArgs &e);
    bool NuclearPlantButton(const CEGUI::EventArgs &e);
    bool ICBMSiloButton(const CEGUI::EventArgs &e);
    bool InfantryButton(const CEGUI::EventArgs &e);
    bool TankButton(const CEGUI::EventArgs &e);
    bool ScudButton(const CEGUI::EventArgs &e);
    bool SubmarineButton(const CEGUI::EventArgs &e);
    bool DestroyerButton(const CEGUI::EventArgs &e);
    bool BomberButton(const CEGUI::EventArgs &e);
    bool FighterButton(const CEGUI::EventArgs &e);
    bool CaptureButton(const CEGUI::EventArgs &e);
    bool LaunchButton(const CEGUI::EventArgs &e);
    bool FriendlyReminderButton(const CEGUI::EventArgs &e);
    
    void DeButton();
    void BuildingImages0();
    void BuildingImages1(PlanetCell &cell);
    void BuildingImagesCB2(PlanetCell &cell);
    void BuildingImagesA3(PlanetCell &cell);
    void BuildingImagesN4(PlanetCell &cell);
    void BuildingImagesAF5(PlanetCell &cell);
    void BuildingImagesNP6(PlanetCell &cell);
    void BuildingImagesICBMS7(PlanetCell &cell);
    void UnitImagesI1(PlanetCell &cell);
    void UnitImagesT2(PlanetCell &cell);
    void UnitImagesS3(PlanetCell &cell);
    void UnitImagesSM4(PlanetCell &cell);
    void UnitImagesD5(PlanetCell &cell);
    void UnitImagesB6(PlanetCell &cell);
    void UnitImagesF7(PlanetCell &cell);
    void BuildButtons();
    
    bool issueProduceOrder(PlanetCell& origin, PlanetCell& goal);
    bool createUnit(PlanetCell &goal);
    bool issueMoveOrder(Unit& unit, PlanetCell& origin, PlanetCell& goal);
    bool moveUnit(Unit& unit, PlanetCell& origin, PlanetCell& goal);
    void illuminate(PlanetCell& targetCell);
    void deluminate(PlanetCell& targetCell);
    
    void fireMissile(PlanetCell& origin, PlanetCell& goal);
    void sendBomber(PlanetCell& origin, PlanetCell& goal);
    void retireUnit(PlanetCell& targetCell);
    void killEnemyUnit(PlanetCell& targetCell);
    void nuke(PlanetCell& targetCell);

    void processEvents();
    void processResources(int, int);
    void processTimer();
    void capture(PlanetCell& targetCell);
    void attack(PlanetCell& targetCell);
    void combat(PlanetCell& yourCell,PlanetCell& enemyCell);
    void commandBaseChange(bool, bool);

    
private:
    Ogre::SceneNode*        m_pOgreHeadNode;
    Ogre::Entity*            m_pOgreHeadEntity;
    Ogre::MaterialPtr        m_pOgreHeadMat;
    Ogre::MaterialPtr        m_pOgreHeadMatHigh;
    Ogre::SceneNode*    cameraNode;
 
    OgreBites::ParamsPanel*        m_pDetailsPanel;
    bool                m_bQuit;
 
    Ogre::Vector3            m_TranslateVector;
    Ogre::Real            m_MoveSpeed;
    Ogre::Degree            m_RotateSpeed;
    float                m_MoveScale;
    Ogre::Degree            m_RotScale;
 
    Ogre::RaySceneQuery*        m_pRSQ;
    Ogre::SceneNode*        m_pCurrentObject;
    Ogre::Entity*            m_pCurrentEntity;
    bool                m_bLMouseDown, m_bRMouseDown;
    bool                m_bSettingsMode;

    Ogre::Real camDistance;
    
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
    int buildingImages;
    int myOwner;
    int myEnemy;
    int commandBaseMax;
    int commandBaseTotal;
    int myCommandBase;
    int enemyCommandBase;
    int money;
    int plutonium;
    int population;
    bool onButton;
    int acceptNeighbors[6];
    int unitBuilding;
    int unitMoving;
    bool launching;
    bool bombing;
    int unitMoney;
    int unitPlutonium;
    int unitTimer;
    int tutorial;
    time_t  currentTime;
    std::priority_queue<Times, std::vector<Times>, CompareTimes> eventQueue;
    char* musicBackground;
	char* musicBuildComplete;
	char* musicMoveComplete;
	char* musicCaptureComplete;
	char* musicEnemyUnitKilled;
    char* musicUnitKilled;
    char* musicEnemyCommandKilled;
    char* musicCommandKilled;
    char* musicVictory;
    char* musicDefeat;
    
    PlanetCell* currentCell;
    PlanetCell* lastCell;
    bool timerup;

    Planet earth;
    ofstream myfile;
    
    Ogre::SceneNode *mCurrentObject;    //pointer to our currently selected object
    Ogre::RaySceneQuery* mRayScnQuery;    //pointer to our ray scene query
    CEGUI::Renderer* mGUIRenderer;        //our CEGUI renderer
    
    bool bLMouseDown, bRMouseDown;    //true if mouse buttons are held down
    int mCount;                        //number of objects created
    float mRotateSpeed;                //the rotation speed for the camera
    
    bool showWaterLand;
    
    int intId;
    
    std::vector<Unit> units;
    std::vector<Missile> missiles;
    std::vector<Bomber> bombers;
};
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||
