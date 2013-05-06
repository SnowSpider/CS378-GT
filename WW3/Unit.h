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
    
    Au_COMMANDBASE = 10000,
    Au_ARMYBASE = 15000,
    Au_NAVYBASE = 20000,
    Au_AIRFORCEBASE = 25000,
    Au_NUCLEARPLANT = 150000,
    Au_ICBMSILO = 50000,
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
    BuildTime_INFANTRY = 15,
    BuildTime_TANK = 30,
    BuildTime_ARTILLERY = 30,
    BuildTime_SCUD = 60,
    BuildTime_ENGINEER = 15,
    
    BuildTime_SUBMARINE = 60,
    BuildTime_DESTROYER = 60,
    BuildTime_CARRIER = 300,
    BuildTime_CRUISER = 60,
    
    BuildTime_BOMBER = 30,
    BuildTime_FIGHTER = 30,
    BuildTime_SPYPLANE = 30,
    
    BuildTime_COMMANDBASE = 60,
    BuildTime_ARMYBASE = 60,
    BuildTime_NAVYBASE = 60,
    BuildTime_AIRFORCEBASE = 60,
    BuildTime_NUCLEARPLANT = 300,
    BuildTime_ICBMSILO = 60,
    BuildTime_SAMSITE = 30
};

enum moveTime{
    MoveTime_INFANTRY = 25,
    MoveTime_TANK = 20,
    MoveTime_ARTILLERY = 25,
    MoveTime_SCUD = 25,
    MoveTime_ENGINEER = 25,
    
    MoveTime_SUBMARINE = 25,
    MoveTime_DESTROYER = 20,
    MoveTime_CARRIER = 30,
    MoveTime_CRUISER = 20,
    
    MoveTime_BOMBER = 5,
    MoveTime_FIGHTER = 5,
    MoveTime_SPYPLANE = 5,
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
    void createObject(Ogre::SceneManager* scnMgr, char* mesh, char* mat);
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
    int timeLeft;
    Arrow myArrow;
    
    Unit(size_t myOwner, size_t label){
        owner = myOwner;
        myType = label;
        moveTime = 60;
        timeLeft = 0;
    }
    
    void createObject(Ogre::SceneManager* scnMgr, char* mesh, char* mat);
    void createManualObject(Ogre::SceneManager* scnMgr);
    //void applyTexture(char* mat);
    void extendArrow(btVector3& dest);
    void relocate(btVector3& dest);
};

#endif
