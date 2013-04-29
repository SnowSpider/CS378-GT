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

enum unitType{
    Unit_INFANTRY = 0,
    Unit_TANK = 1,
    Unit_ARTILLERY = 2,
    Unit_SCUD = 3,
    Unit_ENGINEER = 4,
    
    Unit_SUBMARINE = 5,
    Unit_DESTROYER = 6,
    Unit_CARRIER = 7,
    Unit_CRUISER = 8,
    
    Unit_BOMBER = 9,
    Unit_FIGHTER = 10,
    Unit_SPYPLANE = 11,
    
    Unit_COMMANDBASE = 12,
    Unit_ARMYBASE = 13,
    Unit_NAVYBASE = 14,
    Unit_AIRFORCEBASE = 15,
    Unit_ICBMSILO = 16,
    Unit_SAMSITE = 17
};

class Unit {
    public:
    Ogre::SceneNode* rootNode;    
    int id;
    int myType;
    int owner;
    int currentCellId;
    btVector3 position;
    btVector3 front;
    int hitPoints; 
    int damage; // Attack value
    int moveInterval; // The number of seconds this unit takes to move to a neighboring cell
    
    Unit(){
        id = -1;
        currentCellId = -1;
        owner = 1;
    }
    
    void createManualObject(Ogre::SceneManager* scnMgr);
};

class UnitList{
    public:
    vector<Unit> _units;
    size_t currentId;
    
    UnitList(){
        currentId = 0;
    }
    
    void add(Unit& u){
        u.id = currentId++;
        _units.push_back(u);
    }
    
    Unit& operator []( int i ){ 
        return _units[i];
    }
    
    Unit operator []( int i ) const { 
        return _units[i]; 
    }
    
    int size(){
        return _units.size();
    }
    
    void clear(){
        _units.clear();
        currentId = 0;
    }
};


#endif
