// Author: Wonjun Lee

#ifndef __GameObject_h_
#define __GameObject_h_

#include "Ogre.h"

using namespace std;

class GameObject {
    //private:
    
    public:
    Ogre::Vector3 position; // xyz coordinates with respect to the world origin
    Ogre::Vector3 nextPosition; // expected position in the next frame
    Ogre::SceneNode* rootNode;
    Ogre::Vector3 direction;
    Ogre::Real speed;
    Ogre::Real mass; 
    //GameObject(){}
    
    
    GameObject(){}
    GameObject(Ogre::SceneManager* scnMgr, Ogre::Vector3 pos, char* mesh, char* mat);
    //~GameObject();
    void move(const Ogre::FrameEvent& evt);
    void setSpeed(Ogre::Real s);
    void setDirection(Ogre::Vector3 d);
    void setMass(Ogre::Real m);
    void scale(float s);
    void setPosition(Ogre::Vector3 d);
    Ogre::SceneNode* getNode() { return rootNode; }
};

#endif // #ifndef __GameObject_h_
