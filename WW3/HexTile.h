#ifndef __HexTile_h_
#define __HexTile_h_

#include "Ogre.h"
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <string.h>
#include <iostream>
#include <btBulletCollisionCommon.h>
#include <vector>

class HexTile {
    public:
    size_t id; 
    size_t owner; //id of the player who owns the territory
    vector<size_t> neighbors; // neigboring cells
    btVector3 center;
    vector<btVector3> paramVerts; //parameter vertices
    float height;
    float longitude;
    float latitude;
    
    size_t radiation; // 0, 1, 2, 3
    size_t population; // 0, 1, 2, 3
    
    
    PlanetCell(){
        
    }
    
    
    void select();
    void deselect();
    void setOwner(size_t him);
    
};

#endif // #ifndef __HexTile_h_
