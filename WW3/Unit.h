#ifndef __Unit_H__
#define __Unit_H__

#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>
#include <vector>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()
#include <iostream>
#include <sstream>
#include <math.h>
#include <cmath>
#include <string>
#include <algorithm>
#include <btBulletCollisionCommon.h>
#include "Ogre.h"
#include "Planet.h"

#define PI 3.1415926535897932384626433832795

using namespace std;

enum unitCost_gold{
    Au_INFANTRY = 100,
    Au_TANK = 1000,
    Au_ARTILLERY = 800,
    Au_SCUD = 1500,
    Au_ENGINEER = 200,
    
    Au_SUBMARINE = 3000,
    Au_DESTROYER = 2400,
    Au_CARRIER = 15000,
    Au_CRUISER = 4000,
    
    Au_BOMBER = 2000,
    Au_FIGHTER = 1200,
    Au_SPYPLANE = 3000,
    
    Au_COMMANDBASE = 0,
    Au_ARMYBASE = 15000,
    Au_NAVYBASE = 20000,
    Au_AIRFORCEBASE = 25000,
    Au_NUCLEARPLANT = 150000,
    Au_ICBMSILO = 50000,
    Au_LAUNCH = 100000,
    Au_CAPTURE = 0,
    Au_SAMSITE = 1000
};

enum unitCost_plutonium{
    Pt_INFANTRY = 0,
    Pt_TANK = 0,
    Pt_ARTILLERY = 0,
    Pt_SCUD = 50,
    Pt_ENGINEER = 0,
    
    Pt_SUBMARINE = 300,
    Pt_DESTROYER = 0,
    Pt_CARRIER = 100,
    Pt_CRUISER = 0,
    
    Pt_BOMBER = 150,
    Pt_FIGHTER = 0,
    Pt_SPYPLANE = 0,
    
    Pt_COMMANDBASE = 0,
    Pt_ARMYBASE = 0,
    Pt_NAVYBASE = 0,
    Pt_AIRFORCEBASE = 0,
    Pt_NUCLEARPLANT = 2500,
    Pt_ICBMSILO = 1000,
    Pt_LAUNCH = 100000,
    Pt_CAPTURE = 0,
    Pt_SAMSITE = 0
};

enum unitType{
    Unit_EMPTY = 0,
    Unit_INFANTRY = 1,
    Unit_TANK = 2,
    Unit_ARTILLERY = 3,
    Unit_SCUD = 4,
    Unit_ENGINEER = 5,
    
    Unit_SUBMARINE = 6,
    Unit_DESTROYER = 7,
    Unit_CARRIER = 8,
    Unit_CRUISER = 9,
    
    Unit_BOMBER = 10,
    Unit_FIGHTER = 11,
    Unit_SPYPLANE = 12,
    
    Unit_COMMANDBASE = 13,
    Unit_ARMYBASE = 14,
    Unit_NAVYBASE = 15,
    Unit_AIRFORCEBASE = 16,
    Unit_NUCLEARPLANT = 17,
    Unit_ICBMSILO = 18,
    Unit_SAMSITE = 19
};

enum buildTime{
    BuildTime_INFANTRY = 5,
    BuildTime_TANK = 5,
    BuildTime_ARTILLERY = 5,
    BuildTime_SCUD = 5,
    BuildTime_ENGINEER = 5,
    
    BuildTime_SUBMARINE = 5,
    BuildTime_DESTROYER = 5,
    BuildTime_CARRIER = 5,
    BuildTime_CRUISER = 5,
    
    BuildTime_BOMBER = 5,
    BuildTime_FIGHTER = 5,
    BuildTime_SPYPLANE = 5,
    
    BuildTime_COMMANDBASE = 5,
    BuildTime_ARMYBASE = 5,
    BuildTime_NAVYBASE = 5,
    BuildTime_AIRFORCEBASE = 5,
    BuildTime_NUCLEARPLANT = 5,
    BuildTime_ICBMSILO = 5,
    BuildTime_LAUNCH = 5,
    BuildTime_CAPTURE = 10,
    BuildTime_SAMSITE = 5
};

enum moveTime{
    MoveTime_INFANTRY = 5,
    MoveTime_TANK = 5,
    MoveTime_ARTILLERY = 5,
    MoveTime_SCUD = 5,
    MoveTime_ENGINEER = 5,
    
    MoveTime_SUBMARINE = 5,
    MoveTime_DESTROYER = 5,
    MoveTime_CARRIER = 5,
    MoveTime_CRUISER = 5,
    
    MoveTime_BOMBER = 5,
    MoveTime_FIGHTER = 5,
    MoveTime_SPYPLANE = 5,
};

class Missile {
    public:
    Ogre::SceneNode* rootNode;
    Ogre::Entity* myEntity;
    size_t id;
    string name;
    int owner;
    btVector3 start;
    btVector3 end;
    
    std::vector<float> altitudes;
    std::vector<btVector3> positions;
    std::vector<btVector3> vectors;
    
    Missile(){
        owner = Owner_NEUTRAL;
    }
    
    Missile(size_t myOwner, btVector3 s, btVector3 e){
        owner = myOwner;
        start = s;
        end = e;
    }
    void createObject(Ogre::SceneManager* scnMgr);
    void relocate(btVector3& dest);
    void destroy(Ogre::SceneManager* scnMgr);
};

class Arrow {
    public:
    Ogre::SceneNode* rootNode;
    Ogre::Entity* myEntity;
    size_t id;
    string name;
    int owner;
    btVector3 start;
    btVector3 end;
    
    Arrow(){
        owner = Owner_NEUTRAL;
    }
    
    Arrow(size_t myOwner, btVector3 s, btVector3 e){
        owner = myOwner;
        start = s;
        end = e;
    }
    void createObject(Ogre::SceneManager* scnMgr);
    void stretch(btVector3 s, btVector3 e);
    void setVisibility(bool flag);
};

class Unit {
    public:
    Ogre::SceneNode* rootNode;
    Ogre::Entity* myEntity;
    size_t id;
    string name;
    int myType;
    int owner;
    btVector3 position;
    btVector3 front;
    int hitPoints; 
    int damage; // Attack value
    int moveTime; // The number of seconds this unit takes to move to a neighboring cell
    int buildTime; // The number of seconds this unit takes to move to a neighboring cell
    float buildInterval;
    Arrow myArrow;
    
    Unit(size_t myOwner, size_t label){
        owner = myOwner;
        myType = label;
        if(myType == Unit_INFANTRY){ moveTime = MoveTime_INFANTRY; buildTime = BuildTime_INFANTRY; }
        else if(myType == Unit_TANK){ moveTime = MoveTime_TANK; buildTime = BuildTime_TANK; }
        else if(myType == Unit_ARTILLERY){ moveTime = MoveTime_ARTILLERY; buildTime = BuildTime_ARTILLERY; }
        else if(myType == Unit_SCUD){ moveTime = MoveTime_SCUD; buildTime = BuildTime_SCUD; }
        else if(myType == Unit_ENGINEER){ moveTime = MoveTime_ENGINEER; buildTime = BuildTime_ENGINEER; }
        else if(myType == Unit_SUBMARINE){ moveTime = MoveTime_SUBMARINE; buildTime = BuildTime_SUBMARINE; }
        else if(myType == Unit_DESTROYER){ moveTime = MoveTime_DESTROYER; buildTime = BuildTime_DESTROYER; }
        else if(myType == Unit_CARRIER){ moveTime = MoveTime_CARRIER; buildTime = BuildTime_CARRIER; }
        else if(myType == Unit_CRUISER){ moveTime = MoveTime_CRUISER; buildTime = BuildTime_CRUISER; }
        else if(myType == Unit_BOMBER){ moveTime = MoveTime_BOMBER; buildTime = BuildTime_BOMBER; }
        else if(myType == Unit_FIGHTER){ moveTime = MoveTime_FIGHTER; buildTime = BuildTime_FIGHTER; }
        else if(myType == Unit_SPYPLANE){ moveTime = MoveTime_SPYPLANE; buildTime = BuildTime_SPYPLANE; }
        else if(myType == Unit_COMMANDBASE){ buildTime = BuildTime_COMMANDBASE; }
        else if(myType == Unit_ARMYBASE){ buildTime = BuildTime_ARMYBASE; }
        else if(myType == Unit_NAVYBASE){ buildTime = BuildTime_NAVYBASE; }
        else if(myType == Unit_AIRFORCEBASE){ buildTime = BuildTime_AIRFORCEBASE; }
        else if(myType == Unit_NUCLEARPLANT){ buildTime = BuildTime_NUCLEARPLANT; }
        else if(myType == Unit_ICBMSILO){ buildTime = BuildTime_ICBMSILO; }
        else if(myType == Unit_SAMSITE){ buildTime = BuildTime_SAMSITE; }
        else{ moveTime = 0; buildTime = 0; }
        if(buildTime == 0) buildInterval = 0; //critical : to prevent divide-by-zero
        else buildInterval = 300.0 / buildTime;
    }
    
    void createObject(Ogre::SceneManager* scnMgr, char* mesh, char* mat);
    void createNavyObject(Ogre::SceneManager* scnMgr);
    void createSymbolObject(Ogre::SceneManager* scnMgr);
    void createManualObject(Ogre::SceneManager* scnMgr);
    //void applyTexture(char* mat);
    void extendArrow(btVector3& dest);
    void relocate(btVector3& dest);
    void translate(float x, float y, float z);
    void setDirection(btVector3& d);
    void grow();
    void destroy(Ogre::SceneManager* scnMgr);
};

#endif
