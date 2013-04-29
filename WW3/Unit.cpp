#include "Unit.h"
#include "imageloader.h"

using namespace Ogre;

int unitId = 0;

void Unit::createManualObject(Ogre::SceneManager* scnMgr){
    string s = "Unit_";
    stringstream ss;
    Ogre::String name_manobj;
    ss << unitId++;
    name_manobj = s.append(ss.str()); // Be careful not to append indefinitely.
    
    ManualObject* manual = scnMgr->createManualObject(name_manobj);
    manual->setDynamic(true);
    manual->begin("SolidColour", RenderOperation::OT_TRIANGLE_LIST);
    if (owner == Owner_RED) manual->colour(1,0,0);
    else if (owner == Owner_NEUTRAL) manual->colour(1,1,0);
    else if (owner == Owner_BLUE) manual->colour(0,0,1); 
    
    // top
    manual->normal(0,1,0);
    manual->position(1, 1, -1);
    manual->position(-1, 1, -1);
    manual->position(-1, 1, 1);
    manual->position(1, 1, 1);
    manual->quad(0,1,2,3);
    
    // bottom
    manual->normal(0,-1,0);
    manual->position(1, -1, 1);
    manual->position(-1, -1, 1);
    manual->position(-1, -1, -1);
    manual->position(1, -1, -1);
    manual->quad(4,5,6,7);
    
    // front
    manual->normal(0,0,1);
    manual->position(1, 1, 1);
    manual->position(-1, 1, 1);
    manual->position(-1, -1, 1);
    manual->position(1, -1, 1);
    manual->quad(8,9,10,11);
    
    // back
    manual->normal(0,0,-1);
    manual->position(1, 1, -1);
    manual->position(1, -1, -1);
    manual->position(-1, -1, -1);
    manual->position(-1, 1, -1);
    manual->quad(12,13,14,15);
    
    // left
    manual->normal(-1,0,0);
    manual->position(-1, 1, 1);
    manual->position(-1, 1, -1);
    manual->position(-1, -1, -1);
    manual->position(-1, -1, 1);
    manual->quad(16,17,18,19);
    
    // right
    manual->normal(1,0,0);
    manual->position(1, 1, -1);
    manual->position(1, 1, 1);
    manual->position(1, -1, 1);
    manual->position(1, -1, -1);
    manual->quad(20,21,22,23);
    
    manual->end();
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode(name_manobj);
    rootNode->attachObject(manual);
    
    rootNode->scale(150, 150, 150);
}
/*
void Unit::applyTexture(){

}
*/
void Unit::relocate(btVector3& destination){
    rootNode->translate(Ogre::Vector3(destination.x(), destination.y(), destination.z()));
    rootNode->setDirection(Ogre::Vector3(destination.x(), destination.y(), destination.z()));
}

