#include "Unit.h"
#include "imageloader.h"

using namespace Ogre;

int unitId = 0;

string intToStr(int number){
   stringstream ss;
   ss << number;
   return ss.str();
}

void Missile::createObject(Ogre::SceneManager* scnMgr){
    string s = "Missile_";
    id = unitId;
    name = s.append(intToStr(unitId));
    unitId += 1;
    
    ManualObject* manual = scnMgr->createManualObject(name);
    manual->setDynamic(true);
    
    string mat;
    if(owner == Owner_BLUE) mat = "MyMaterials/Blue";
    else if(owner == Owner_RED) mat = "MyMaterials/Red";
    else mat = "BaseWhite";
    
    manual->begin(mat, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(0.5, 0.5, -4); manual->normal(0,0,-1); 
    manual->position(1,1,0); manual->normal(1,1,0); 
    manual->position(1,-1,0); manual->normal(1,-1,0);  
    manual->position(-1,-1,0); manual->normal(-1,-1,0); 
    manual->position(-1,1,0); manual->normal(-1,1,0);
    manual->position(1,1,0); manual->normal(1,1,0); 
    manual->end();
    
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode(name);
    Ogre::MeshPtr newMesh = manual->convertToMesh(name);
    myEntity = scnMgr->createEntity(name, name);
    rootNode->attachObject(myEntity);
    rootNode->scale(100, 100, 100);
    
    rootNode->setPosition(Ogre::Vector3(start.x(), start.y(), start.z()));
    rootNode->setDirection(Ogre::Vector3(start.x(), start.y(), start.z()), Node::TS_PARENT);
}

void Missile::relocate(btVector3& dest){
    rootNode->setPosition(Ogre::Vector3(dest.x(), dest.y(), dest.z()));
    rootNode->setDirection(Ogre::Vector3(dest.x(), dest.y(), dest.z()), Node::TS_PARENT);
}

void Missile::destroy(Ogre::SceneManager* scnMgr){
    
}

void Arrow::createObject(Ogre::SceneManager* scnMgr){
    string s = "Arrow_";
    id = unitId;
    name = s.append(intToStr(unitId));
    unitId += 1;
    
    ManualObject* manual = scnMgr->createManualObject(name);
    manual->setDynamic(true);
    
    string mat;
    if(owner == Owner_BLUE) mat = "MyMaterials/arrow_blue";
    else if(owner == Owner_RED) mat = "MyMaterials/arrow_red";
    else mat = "BaseWhiteNoLighting";
    
    btVector3 c = start.cross(end - start);
    c.normalize();
    c *= 20;
    
    btVector3 start_left = start + c;
    btVector3 start_right = start - c;
    btVector3 end_left = end + c;
    btVector3 end_right = end - c;
    btVector3 middle = (start + end) / 2.0;
    btVector3 middle_left = middle + c;
    btVector3 middle_right = middle - c;
    
    start_left *= 1.01;
    start_right *= 1.01;
    end_left *= 1.01;
    end_right *= 1.01;
    middle_left *= 1.01;
    middle_right *= 1.01;
    
    
    manual->begin(mat, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(middle_left.x(), middle_left.y(), middle_left.z()); manual->normal(middle_left.x(), middle_left.y(), middle_left.z()); manual->textureCoord(0,0.5);
    manual->position(start_left.x(), start_left.y(), start_left.z()); manual->normal(start_left.x(), start_left.y(), start_left.z()); manual->textureCoord(0,0);
    manual->position(start_right.x(), start_right.y(), start_right.z()); manual->normal(start_right.x(), start_right.y(), start_right.z()); manual->textureCoord(1,0);
    manual->position(middle_right.x(), middle_right.y(), middle_right.z()); manual->normal(middle_right.x(), middle_right.y(), middle_right.z()); manual->textureCoord(1,0,5);
    manual->position(end_right.x(), end_right.y(), end_right.z()); manual->normal(end_right.x(), end_right.y(), end_right.z()); manual->textureCoord(1,1);
    manual->position(end_left.x(), end_left.y(), end_left.z()); manual->normal(end_left.x(), end_left.y(), end_left.z()); manual->textureCoord(0,1);
    manual->end();
    
    
    manual->begin(mat, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(middle_left.x(), middle_left.y(), middle_left.z()); manual->normal(middle_left.x(), middle_left.y(), middle_left.z()); manual->textureCoord(0,0.5);
    manual->position(end_left.x(), end_left.y(), end_left.z()); manual->normal(end_left.x(), end_left.y(), end_left.z()); manual->textureCoord(0,1);
    manual->position(end_right.x(), end_right.y(), end_right.z()); manual->normal(end_right.x(), end_right.y(), end_right.z()); manual->textureCoord(1,1);
    manual->position(middle_right.x(), middle_right.y(), middle_right.z()); manual->normal(middle_right.x(), middle_right.y(), middle_right.z()); manual->textureCoord(1,0.5);
    manual->position(start_right.x(), start_right.y(), start_right.z()); manual->normal(start_right.x(), start_right.y(), start_right.z()); manual->textureCoord(1,0);
    manual->position(start_left.x(), start_left.y(), start_left.z()); manual->normal(start_left.x(), start_left.y(), start_left.z()); manual->textureCoord(0,0);
    manual->end();
    
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode(name);
    Ogre::MeshPtr newMesh = manual->convertToMesh(name);
    myEntity = scnMgr->createEntity(name, name);
    rootNode->attachObject(myEntity);
}

void Arrow::stretch(btVector3 s, btVector3 e){
    start = s;
    end = e;
}

void Arrow::setVisibility(bool flag){
    myEntity->setVisible(flag);
}

void Unit::createObject(Ogre::SceneManager* scnMgr, char* mesh, char* mat){
    string s = "Unit_";
    id = unitId;
    name = s.append(intToStr(unitId));
    unitId += 1;
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode(name);
    myEntity = scnMgr->createEntity(name, mesh);
    myEntity->setMaterialName(mat);
    myEntity->setCastShadows(true);
    rootNode->attachObject(myEntity);
    rootNode->scale(0.5, 0.5, 0.5);
}

void Unit::createSymbolObject(Ogre::SceneManager* scnMgr){
    string s = "Unit_";
    id = unitId;
    name = s.append(intToStr(unitId));
    unitId += 1;
    
    ManualObject* manual = scnMgr->createManualObject(name);
    manual->setDynamic(true);
    
    string mat;
    if(myType == Unit_INFANTRY) mat = "MyMaterials/infantry";
    else if(myType == Unit_TANK) mat = "MyMaterials/tank";
    else if(myType == Unit_SCUD) mat = "MyMaterials/scud";
    else mat = "MyMaterials/Blue";
    
    // top
    manual->begin("MyMaterials/Blue", RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, 1, -1); manual->normal(0,1,0); manual->textureCoord(1,1);
    manual->position(-1, 1, -1); manual->normal(0,1,0); manual->textureCoord(0,1);
    manual->position(-1, 1, 1); manual->normal(0,1,0); manual->textureCoord(0,0);
    manual->position(1, 1, 1); manual->normal(0,1,0); manual->textureCoord(1,0);
    manual->end();
    
    // bottom
    manual->begin(mat, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, -1, 1); manual->normal(0,-1,0); manual->textureCoord(1,1);
    manual->position(-1, -1, 1); manual->normal(0,-1,0); manual->textureCoord(0,1);
    manual->position(-1, -1, -1); manual->normal(0,-1,0); manual->textureCoord(0,0);
    manual->position(1, -1, -1); manual->normal(0,-1,0); manual->textureCoord(1,0);
    manual->end();
    
    // front
    manual->begin("MyMaterials/Blue", RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, 1, 1); manual->normal(0,0,1); manual->textureCoord(1,1);
    manual->position(-1, 1, 1); manual->normal(0,0,1); manual->textureCoord(0,1);
    manual->position(-1, -1, 1); manual->normal(0,0,1); manual->textureCoord(0,0);
    manual->position(1, -1, 1); manual->normal(0,0,1); manual->textureCoord(1,0);
    manual->end();
    
    // back
    manual->begin("MyMaterials/Blue", RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, 1, -1); manual->normal(0,0,-1); manual->textureCoord(1,1);
    manual->position(1, -1, -1); manual->normal(0,0,-1); manual->textureCoord(0,1);
    manual->position(-1, -1, -1); manual->normal(0,0,-1); manual->textureCoord(0,0);
    manual->position(-1, 1, -1); manual->normal(0,0,-1); manual->textureCoord(1,0);
    manual->end();
    
    // left
    manual->begin("MyMaterials/Blue", RenderOperation::OT_TRIANGLE_FAN);
    manual->position(-1, 1, 1); manual->normal(-1,0,0); manual->textureCoord(1,1);
    manual->position(-1, 1, -1); manual->normal(-1,0,0); manual->textureCoord(0,1);
    manual->position(-1, -1, -1); manual->normal(-1,0,0); manual->textureCoord(0,0);
    manual->position(-1, -1, 1); manual->normal(-1,0,0); manual->textureCoord(1,0);
    manual->end();
    
    // right
    manual->begin("MyMaterials/Blue", RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, 1, -1); manual->normal(1,0,0); manual->textureCoord(1,1);
    manual->position(1, 1, 1); manual->normal(1,0,0); manual->textureCoord(0,1);
    manual->position(1, -1, 1); manual->normal(1,0,0); manual->textureCoord(0,0);
    manual->position(1, -1, -1); manual->normal(1,0,0); manual->textureCoord(1,0);
    manual->end();
    
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode(name);
    Ogre::MeshPtr newMesh = manual->convertToMesh(name);
    myEntity = scnMgr->createEntity(name, name);
    rootNode->attachObject(myEntity);
    rootNode->scale(150, 20, 100);
}

void Unit::createManualObject(Ogre::SceneManager* scnMgr){
    string s = "Unit_";
    id = unitId;
    name = s.append(intToStr(unitId));
    unitId += 1;
    
    ManualObject* manual = scnMgr->createManualObject(name);
    manual->setDynamic(true);
    
    // top
    manual->begin("MyMaterials/crate", RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, 1, -1); manual->normal(0,1,0); manual->textureCoord(1,1);
    manual->position(-1, 1, -1); manual->normal(0,1,0); manual->textureCoord(0,1);
    manual->position(-1, 1, 1); manual->normal(0,1,0); manual->textureCoord(0,0);
    manual->position(1, 1, 1); manual->normal(0,1,0); manual->textureCoord(1,0);
    manual->end();
    
    // bottom
    manual->begin("MyMaterials/crate", RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, -1, 1); manual->normal(0,-1,0); manual->textureCoord(1,1);
    manual->position(-1, -1, 1); manual->normal(0,-1,0); manual->textureCoord(0,1);
    manual->position(-1, -1, -1); manual->normal(0,-1,0); manual->textureCoord(0,0);
    manual->position(1, -1, -1); manual->normal(0,-1,0); manual->textureCoord(1,0);
    manual->end();
    
    // front
    manual->begin("MyMaterials/crate", RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, 1, 1); manual->normal(0,0,1); manual->textureCoord(1,1);
    manual->position(-1, 1, 1); manual->normal(0,0,1); manual->textureCoord(0,1);
    manual->position(-1, -1, 1); manual->normal(0,0,1); manual->textureCoord(0,0);
    manual->position(1, -1, 1); manual->normal(0,0,1); manual->textureCoord(1,0);
    manual->end();
    
    // back
    manual->begin("MyMaterials/crate", RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, 1, -1); manual->normal(0,0,-1); manual->textureCoord(1,1);
    manual->position(1, -1, -1); manual->normal(0,0,-1); manual->textureCoord(0,1);
    manual->position(-1, -1, -1); manual->normal(0,0,-1); manual->textureCoord(0,0);
    manual->position(-1, 1, -1); manual->normal(0,0,-1); manual->textureCoord(1,0);
    manual->end();
    
    // left
    manual->begin("MyMaterials/crate", RenderOperation::OT_TRIANGLE_FAN);
    manual->position(-1, 1, 1); manual->normal(-1,0,0); manual->textureCoord(1,1);
    manual->position(-1, 1, -1); manual->normal(-1,0,0); manual->textureCoord(0,1);
    manual->position(-1, -1, -1); manual->normal(-1,0,0); manual->textureCoord(0,0);
    manual->position(-1, -1, 1); manual->normal(-1,0,0); manual->textureCoord(1,0);
    manual->end();
    
    // right
    manual->begin("MyMaterials/crate", RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, 1, -1); manual->normal(1,0,0); manual->textureCoord(1,1);
    manual->position(1, 1, 1); manual->normal(1,0,0); manual->textureCoord(0,1);
    manual->position(1, -1, 1); manual->normal(1,0,0); manual->textureCoord(0,0);
    manual->position(1, -1, -1); manual->normal(1,0,0); manual->textureCoord(1,0);
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
void Unit::extendArrow(btVector3& dest){
    myArrow.stretch(position, dest);
}

void Unit::relocate(btVector3& dest){
    position = dest;
    rootNode->setPosition(Ogre::Vector3(dest.x(), dest.y(), dest.z()));
    rootNode->setDirection(Ogre::Vector3(dest.x(), dest.y(), dest.z()), Node::TS_PARENT);
    rootNode->setDirection(Ogre::Vector3(0,1,0), Node::TS_LOCAL);
}

void Unit::translate(float x, float y, float z){
    position += btVector3(x,y,z);
    rootNode->translate(x,y,z, Node::TS_LOCAL);
}

void Unit::setDirection(btVector3& d){ //need repair
    rootNode->setDirection(Ogre::Vector3(d.x(), d.y(), d.z()), Node::TS_PARENT);
}

void Unit::grow(){
    rootNode->translate(0, -buildInterval, 0, Node::TS_LOCAL);
}

void Unit::destroy(Ogre::SceneManager* scnMgr){
    scnMgr->getRootSceneNode()->removeAndDestroyChild(name);
    
}

