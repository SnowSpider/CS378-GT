#ifndef __HexTile_h_
#define __HexTile_h_

#include "Ogre.h"
#include <btBulletCollisionCommon.h>

#include <vector>

class HexTile {
    public:
    int id;
    btVector3 pos_center; 
    btVector3 paramVerts[6];
    btVector3 normal;
    float speed;
    
    HexTile(){}
    HexTile(int i, );
    //~HexTile();
};

#endif // #ifndef __HexTile_h_
