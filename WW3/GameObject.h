#ifndef __GameObject_h_
#define __GameObject_h_

#include "Ogre.h"
#include <btBulletCollisionCommon.h>

#include "OgreMotionState.h"
#include "Simulator.h"

using namespace std;

class Simulator;

class GameObject {
    //protected:
    public:
    Ogre::String name;
    Ogre::SceneManager* sceneMgr;
    Simulator* simulator;
    Ogre::SceneNode* rootNode;
    btCollisionShape* shape;
    btScalar mass;
    btRigidBody* body;
    btTransform tr;
    btVector3 inertia;
    OgreMotionState* motionState;
    
    //private:
    
    //public:
    btVector3 position; // xyz coordinates with respect to the world origin
    btVector3 nextPosition; // expected position in the next frame
    btVector3 direction;
    float speed;
    
    GameObject(){}
    GameObject(Ogre::SceneManager* scnMgr, Simulator* sim, btVector3 pos, char* mesh, char* mat);
    //~GameObject();
    void move(const Ogre::FrameEvent& evt);
    void setSpeed(float s);
    void setDirection(btVector3 d);
    void setMass(float m);
    void scale(float s);
    void scale(float a, float b, float c);
    void setPosition(btVector3 d);
    Ogre::SceneNode* getNode() { return rootNode; }
    void updateTransform();
    void addToSimulator();
    btRigidBody* getBody() { return body; }
    
};

#endif // #ifndef __GameObject_h_
