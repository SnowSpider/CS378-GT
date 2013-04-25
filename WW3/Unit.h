#ifndef __Type_H__
#define __Type_H__

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
    Type_INFANTRY = 0,
    Type_TANK = 1,
    Type_ARTILLERY = 2,
    Type_SCUD = 3,
    Type_ENGINEER = 4,
    
    Type_SUBMARINE = 5,
    Type_DESTROYER = 6,
    Type_CARRIER = 7,
    Type_CRUISER = 8,
    
    Type_BOMBER = 9,
    Type_FIGHTER = 10,
    Type_SPYPLANE = 11
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
