#include "Unit.h"
#include "imageloader.h"

using namespace Ogre;

int unitId = 0;

string intToStr(int number){
   stringstream ss;
   ss << number;
   return ss.str();
}

void Unit::createObject(Ogre::SceneManager* scnMgr, char* mesh, char* mat){
    string s = "Unit_";
    id = unitId;
    name = s.append(intToStr(unitId));
    cout << "name = " << name << endl;
    unitId += 1;
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode(name);
    myEntity = scnMgr->createEntity(name, mesh);
    myEntity->setMaterialName(mat);
    myEntity->setCastShadows(true);
    rootNode->attachObject(myEntity);
    rootNode->scale(30, 30, 30);
}

void Unit::createManualObject(Ogre::SceneManager* scnMgr){
    string s = "Unit_";
    id = unitId;
    name = s.append(intToStr(unitId));
    cout << "name = " << name << endl;
    unitId += 1;
    
    ManualObject* manual = scnMgr->createManualObject(name);
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
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode(name);
    Ogre::MeshPtr newMesh = manual->convertToMesh(name);
    myEntity = scnMgr->createEntity(name, name);
    rootNode->attachObject(myEntity);
    rootNode->scale(150, 150, 150);
}
/*
void Unit::applyTexture(char* mat){

}
*/
void Unit::relocate(btVector3& destination){
    rootNode->translate(Ogre::Vector3(destination.x(), destination.y(), destination.z()));
    rootNode->setDirection(Ogre::Vector3(destination.x(), destination.y(), destination.z()));
}

