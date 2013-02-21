// Author: Wonjun Lee

#ifndef __GameObject_h_
#define __GameObject_h_

#include "Ogre.h"

using namespace std;

class GameObject {
    //private:
    public:
    Ogre::SceneNode* rootNode;
    Ogre::Vector3 direction;
    Ogre::Vector3 speed;
    Ogre::Vector3 location; //xyz coordinates with respect to the world origin
    Ogre::Real mass; 
    //GameObject(){}
    
    
    GameObject(){}
    GameObject(Ogre::SceneManager* scnMgr, Ogre::Vector3 loc, char* mesh, char* mat);
    //~GameObject();
    void move(const Ogre::FrameEvent& evt);
    void setSpeed(Ogre::Real s);
    void setDirection(Ogre::Vector3 d);
    void setMass(Ogre::Real m);
    void scale(float s);
    Ogre::SceneNode* getNode() { return rootNode; }
};

#endif // #ifndef __GameObject_h_
