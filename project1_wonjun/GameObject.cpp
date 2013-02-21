#include "GameObject.h"
#include <OgreEntity.h>
#include <OgreSceneManager.h>

int id = 0;

GameObject::GameObject(Ogre::SceneManager* scnMgr, Ogre::Vector3 pos, char* mesh, char* mat){
    position = pos;
    nextPosition = pos + speed * 0.001 * direction;
    Ogre::Entity* ball = scnMgr->createEntity("GameObject"+id, mesh);
    ball->setMaterialName(mat);
    ball->setCastShadows(true);
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode("GameObject"+id);
    rootNode->attachObject(ball);
    rootNode->scale(0.1f, 0.1f, 0.1f);
    rootNode->setPosition(pos.x, pos.y, pos.z);
    direction = Ogre::Vector3(0.0f, 0.0f, 0.0f);
    //direction.normalise();
    speed = 0.0f;
    mass = 0.0f;
    id++;
}

void GameObject::move(const Ogre::FrameEvent& evt){
    //Ogre::Vector3 position = rootNode->getPosition();
    rootNode->translate(speed * evt.timeSinceLastFrame * direction);
    position += speed * evt.timeSinceLastFrame * direction;
    nextPosition = position + speed * 0.001 * direction; 
    // TODO: I need optimization here
    // Future: calculate nextSpeed also, to accelerate/decelerate the object
}

void GameObject::setSpeed(Ogre::Real s){
    speed = s;
}

void GameObject::setDirection(Ogre::Vector3 d){
    direction = d;
}

void GameObject::setMass(Ogre::Real m){
    mass = m;
}

void GameObject::scale(float s){
    rootNode->scale(s, s, s);
}

void GameObject::setPosition(Ogre::Vector3 pos){
    position = pos;
    nextPosition = position + speed * 0.001 * direction; // critical
    rootNode->setPosition(pos);
}
