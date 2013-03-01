#include "GameObject.h"
#include <OgreEntity.h>
#include <OgreSceneManager.h>

int id = 0;

GameObject::GameObject(Ogre::SceneManager* scnMgr, Simulator* sim, btVector3 pos, char* mesh, char* mat){
    name = "GameObject_"+id;
    id += 1;
    sceneMgr = scnMgr;
    simulator = sim;
    rootNode = sceneMgr->getRootSceneNode()->createChildSceneNode(name);
    shape = NULL;
    tr.setIdentity();
    mass = 0.0f;
    inertia.setZero();
    
    position = pos;
    nextPosition = pos + speed * 0.001 * direction;
    Ogre::Entity* ball = scnMgr->createEntity(name, mesh);
    ball->setMaterialName(mat);
    ball->setCastShadows(true);
    rootNode->attachObject(ball);
    rootNode->scale(0.1f, 0.1f, 0.1f);
    rootNode->setPosition(pos.x(), pos.y(), pos.z());
    direction = btVector3(0.0f, 0.0f, 0.0f);
    speed = 0.0f;
    
    motionState = new OgreMotionState(tr, rootNode); // critical
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
    Ogre::Vector3 pos = rootNode->getPosition(); //std::cout << "a" << std::endl;
    tr.setOrigin(btVector3(pos.x, pos.y, pos.z)); //std::cout << "b" << std::endl;
    Ogre::Quaternion qt = rootNode->getOrientation(); //std::cout << "c" << std::endl;
    tr.setRotation(btQuaternion(qt.x, qt.y, qt.z, qt.w)); //std::cout << "d" << std::endl;
    if (motionState){
        motionState->updateTransform(tr); //std::cout << "e" << std::endl;
    }
    //std::cout << "f" << std::endl;
}

void GameObject::addToSimulator(){
    // using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects 
    updateTransform(); std::cout << "A" << std::endl;
    std::cout << motionState << std::endl;
    std::cout << "A2" << std::endl;
    motionState = new OgreMotionState(tr, rootNode);
    std::cout << "B" << std::endl;
    // rigidbody is dynamic if and only if mass is non zero, otherwise static
    if (mass != 0.0f){
        shape->calculateLocalInertia(mass, inertia);
        std::cout << "C" << std::endl;
    }
    std::cout << "D" << std::endl;
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, shape, inertia);
    std::cout << "E" << std::endl;
    rbInfo.m_restitution = 0.5;
    std::cout << "F" << std::endl;
    body = new btRigidBody(rbInfo);
    std::cout << "G" << std::endl;
    simulator->addObject(this);
    std::cout << "H" << std::endl;
}



