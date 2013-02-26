#include "GameObject.h"
#include <OgreEntity.h>
#include <OgreSceneManager.h>

int id = 0;

GameObject::GameObject(Ogre::String nym, Ogre::SceneManager* mgr, Simulator* sim){
    name = nym;
    sceneMgr = mgr;
    simulator = sim;
    rootNode = sceneMgr->getRootSceneNode()->createChildSceneNode(name);
    shape = NULL;
    tr.setIdentity();
    mass = 0.0f;
    inertia.setZero();
}

GameObject::GameObject(Ogre::SceneManager* scnMgr, btVector3 pos, char* mesh, char* mat){
    position = pos;
    nextPosition = pos + speed * 0.001 * direction;
    Ogre::Entity* ball = scnMgr->createEntity("GameObject"+id, mesh);
    ball->setMaterialName(mat);
    ball->setCastShadows(true);
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("GameObject"+id);
    rootNode->attachObject(ball);
    rootNode->scale(0.1f, 0.1f, 0.1f);
    rootNode->setPosition(pos.x(), pos.y(), pos.z());
    direction = btVector3(0.0f, 0.0f, 0.0f);
    //direction.normalise();
    speed = 0.0f;
    mass = 0.0f;
    id++;
}

void GameObject::move(const Ogre::FrameEvent& evt){
    btVector3 temp = speed * evt.timeSinceLastFrame * direction;
    rootNode->translate(Ogre::Vector3(temp.x(), temp.y(), temp.z()));
    position += speed * evt.timeSinceLastFrame * direction;
    nextPosition = position + speed * 0.001 * direction; 
    // TODO: I need optimization here
    // Future: calculate nextSpeed also, to accelerate/decelerate the object
}

void GameObject::setSpeed(float s){
    speed = s;
}

void GameObject::setDirection(btVector3 d){
    direction = d;
}

void GameObject::setMass(float m){
    mass = m;
}

void GameObject::scale(float s){
    rootNode->scale(s, s, s);
}

void GameObject::scale(float a, float b, float c){
    rootNode->scale(a, b, c);
}

void GameObject::setPosition(btVector3 pos){
    position = pos;
    nextPosition = position + speed * 0.001 * direction; // critical
    rootNode->setPosition(Ogre::Vector3(pos.x(), pos.y(), pos.z()));
}

void GameObject::updateTransform(){
    Ogre::Vector3 pos = rootNode->getPosition();
    tr.setOrigin(btVector3(pos.x, pos.y, pos.z));
    Ogre::Quaternion qt = rootNode->getOrientation();
    tr.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w));
    if (motionState){
        motionState->updateTransform(tr);
    }
}

void GameObject::addToSimulator(){
    // using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects 
    updateTransform();
    motionState = new OgreMotionState(tr, rootNode);
    // rigidbody is dynamic if and only if mass is non zero, otherwise static
    if (mass != 0.0f){
        shape->calculateLocalInertia(mass, inertia);
    }
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
    body = new btRigidBody(rbInfo);
    simulator->addObject(this);
}



