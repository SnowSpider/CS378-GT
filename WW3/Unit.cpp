#include "Unit.h"
#include "imageloader.h"

using namespace Ogre;

int unitId = 0;

string intToStr(int number){
   stringstream ss;
   ss << number;
   return ss.str();
}

void Arrow::createObject(Ogre::SceneManager* scnMgr, char* mesh, char* mat){
    string s = "Arrow_";
    id = unitId;
    name = s.append(intToStr(unitId));
    cout << "name = " << name << endl;
    unitId += 1;
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode(name);
    myEntity = scnMgr->createEntity(name, mesh);
    myEntity->setMaterialName(mat);
    myEntity->setCastShadows(true);
    rootNode->attachObject(myEntity);
    rootNode->scale(1, 1, 1);
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
    cout << "name = " << name << endl;
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
    cout << "name = " << name << endl;
    unitId += 1;
    
    ManualObject* manual = scnMgr->createManualObject(name);
    manual->setDynamic(true);
    
    string mat;
    if(myType == Unit_INFANTRY) mat = "MyMaterials/infantry";
    else if(myType == Unit_TANK) mat = "MyMaterials/tank";
    else if(myType == Unit_SCUD) mat = "MyMaterials/scud";
    else mat = "MyMaterials/Blue";
    
    // top
    manual->begin(mat, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, 1, -1); manual->normal(0,1,0); manual->textureCoord(1,1);
    manual->position(-1, 1, -1); manual->normal(0,1,0); manual->textureCoord(0,1);
    manual->position(-1, 1, 1); manual->normal(0,1,0); manual->textureCoord(0,0);
    manual->position(1, 1, 1); manual->normal(0,1,0); manual->textureCoord(1,0);
    manual->end();
    
    // bottom
    manual->begin("MyMaterials/Blue", RenderOperation::OT_TRIANGLE_FAN);
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
    rootNode->scale(150, 100, 20);
}

void Unit::createManualObject(Ogre::SceneManager* scnMgr){
    string s = "Unit_";
    id = unitId;
    name = s.append(intToStr(unitId));
    cout << "name = " << name << endl;
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

void Unit::setDirection(btVector3& d){
    rootNode->setDirection(Ogre::Vector3(d.x(), d.y(), d.z()), Node::TS_PARENT);
}

void Unit::grow(){
    rootNode->translate(0, -buildInterval, 0, Node::TS_LOCAL);
}

