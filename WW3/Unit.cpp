#include <math.h>
#include <cmath>
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

void Unit::createBomberObject(Ogre::SceneManager* scnMgr){
    string s = "Unit_";
    id = unitId;
    name = s.append(intToStr(unitId));
    unitId += 1;
    
    ManualObject* manual = scnMgr->createManualObject(name);
    manual->setDynamic(true);
    
    string mat_body = "MyMaterials/bomber";
    string mat_wing;
    
    if(owner == Owner_BLUE) mat_wing = "MyMaterials/wing_blue";
    else if(owner == Owner_RED) mat_wing = "MyMaterials/wing_red";
    else mat_wing = "MyMaterials/White";
    
    
    // right wing - top
    manual->begin(mat_wing, RenderOperation::OT_TRIANGLE_LIST);
    
    manual->position(4.5, 0,-1); manual->normal(0,0,-1); manual->textureCoord(1, 0);
    manual->position(4.5, -1,-1); manual->normal(0,0,-1); manual->textureCoord(1, 0.5);
    manual->position(0.5, 0,-1); manual->normal(0,0,-1); manual->textureCoord(0, 0);
    
    manual->position(0.5, 2,-1); manual->normal(0,0,-1); manual->textureCoord(0, 0);
    manual->position(4.5, 0,-1); manual->normal(0,0,-1); manual->textureCoord(1, 0.5);
    manual->position(0.5, 0,-1); manual->normal(0,0,-1); manual->textureCoord(0, 0.5);
    
    // right wing - bottom
    manual->position(0.5, 0,-1); manual->normal(0,0,1); manual->textureCoord(0, 0);
    manual->position(4.5, -1,-1); manual->normal(0,0,1); manual->textureCoord(1, 0.5);
    manual->position(4.5, 0,-1); manual->normal(0,0,1); manual->textureCoord(1, 0);
    
    manual->position(0.5, 0,-1); manual->normal(0,0,1); manual->textureCoord(0, 0.5);
    manual->position(4.5, 0,-1); manual->normal(0,0,1); manual->textureCoord(1, 0.5);
    manual->position(0.5, 2,-1); manual->normal(0,0,1); manual->textureCoord(0, 0);
    
    // left wing - top
    manual->position(-0.5, 0,-1); manual->normal(0,0,-1); manual->textureCoord(0, 0);
    manual->position(-4.5, -1,-1); manual->normal(0,0,-1); manual->textureCoord(1, 0.5);
    manual->position(-4.5, 0,-1); manual->normal(0,0,-1); manual->textureCoord(1, 0);
    
    manual->position(-0.5, 0,-1); manual->normal(0,0,-1); manual->textureCoord(0, 0.5);
    manual->position(-4.5, 0,-1); manual->normal(0,0,-1); manual->textureCoord(1, 0.5);
    manual->position(-0.5, 2,-1); manual->normal(0,0,-1); manual->textureCoord(0, 0);
    
    // left wing - bottom
    manual->position(-4.5, 0,-1); manual->normal(0,0,1); manual->textureCoord(1, 0);
    manual->position(-4.5, -1,-1); manual->normal(0,0,1); manual->textureCoord(1, 0.5);
    manual->position(-0.5, 0,-1); manual->normal(0,0,1); manual->textureCoord(0, 0);
    
    manual->position(-0.5, 2,-1); manual->normal(0,0,1); manual->textureCoord(0, 0);
    manual->position(-4.5, 0,-1); manual->normal(0,0,1); manual->textureCoord(1, 0.5);
    manual->position(-0.5, 0,-1); manual->normal(0,0,1); manual->textureCoord(0, 0.5);
    
    manual->end();
    
    // fuselage - top
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(0.5, 3,-1); manual->normal(0.5, 3,-1); manual->textureCoord(1, 0.5);
    manual->position(0.5, -1,-1); manual->normal(0.5, -1,-1); manual->textureCoord(1, 1);
    manual->position(-0.5, -1,-1); manual->normal(-0.5, -1,-1); manual->textureCoord(0.5, 1);
    manual->position(-0.5, 3,-1); manual->normal(-0.5, 3,-1); manual->textureCoord(0.5, 0.5);
    manual->position(0.5, 3,-1); manual->normal(0.5, 3,-1); manual->textureCoord(1, 0.5);
    manual->end();
    
    // fuselage - bottom
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(0.5, 3,0); manual->normal(0.5, 3,0); manual->textureCoord(1, 0.5);
    manual->position(-0.5, 3,0); manual->normal(-0.5, 3,0); manual->textureCoord(0.5, 0.5);
    manual->position(-0.5, -1,0); manual->normal(-0.5, -1,0); manual->textureCoord(0.5, 1);
    manual->position(0.5, -1,0); manual->normal(0.5, -1,0); manual->textureCoord(1, 1);
    manual->position(0.5, 3,0); manual->normal(0.5, 3,0); manual->textureCoord(1, 0.5);
    manual->end();
    
    // fuselage - left
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(-0.5, 3,-1); manual->normal(-0.5, 3,-1); manual->textureCoord(1, 0.5);
    manual->position(-0.5, -1,-1); manual->normal(-0.5, -1,-1); manual->textureCoord(1, 1);
    manual->position(-0.5, -1,0); manual->normal(-0.5, -1,0); manual->textureCoord(0.5, 1);
    manual->position(-0.5, 3,0); manual->normal(-0.5, 3,0); manual->textureCoord(0.5, 0.5);
    manual->position(-0.5, 3,-1); manual->normal(-0.5, 3,-1); manual->textureCoord(1, 0.5);
    manual->end();
    
    // fuselage - right
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(0.5, 3,-1); manual->normal(0.5, 3,-1); manual->textureCoord(1, 0.5);
    manual->position(0.5, 3,0); manual->normal(0.5, 3,0); manual->textureCoord(0.5, 0.5);
    manual->position(0.5, -1,0); manual->normal(0.5, -1,0); manual->textureCoord(0.5, 1);
    manual->position(0.5, -1,-1); manual->normal(0.5, -1,-1); manual->textureCoord(1, 1);
    manual->position(0.5, 3,-1); manual->normal(0.5, 3,-1); manual->textureCoord(1, 0.5);
    manual->end();
    
    // tail - top
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_LIST);
    
    manual->position(0, -4,-1); manual->normal(0, -4,-1); manual->textureCoord(0.5, 0.5);
    manual->position(-0.5, -1,0); manual->normal(-0.5, -1,0); manual->textureCoord(1, 0.5);
    manual->position(-0.5, -1,-1); manual->normal(-0.5, -1,-1); manual->textureCoord(1, 1);
    
    manual->position(0, -4,-1); manual->normal(0, -4,-1); manual->textureCoord(0.5, 0.5);
    manual->position(-0.5, -1,-1); manual->normal(-0.5, -1,-1); manual->textureCoord(1, 0.5);
    manual->position(0.5, -1,-1); manual->normal(0.5, -1,-1); manual->textureCoord(1, 1);
    
    manual->position(0, -4,-1); manual->normal(0, -4,-1); manual->textureCoord(0.5, 0.5);
    manual->position(0.5, -1,-1); manual->normal(0.5, -1,-1); manual->textureCoord(1, 0.5);
    manual->position(0.5, -1,0); manual->normal(0.5, -1,-1); manual->textureCoord(1, 1);
    
    manual->end();
    
    // tail - bottom
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_LIST);
    
    manual->position(0, -4,-0.5); manual->normal(0, -4,-0.5); manual->textureCoord(0.5, 0.5);
    manual->position(0, -4,-1); manual->normal(0, -4,-1); manual->textureCoord(1, 0.5);
    manual->position(0.5, -1,0); manual->normal(0.5, -1,0); manual->textureCoord(1, 1);
    
    manual->position(0, -4,-0.5); manual->normal(0, -4,-0.5); manual->textureCoord(0.5, 0.5);
    manual->position(0.5, -1,0); manual->normal(0.5, -1,0); manual->textureCoord(1, 0.5);
    manual->position(-0.5, -1,0); manual->normal(-0.5, -1,0); manual->textureCoord(1, 1);
    
    manual->position(0, -4,-0.5); manual->normal(0, -4,-0.5); manual->textureCoord(0.5, 0.5);
    manual->position(-0.5, -1,0); manual->normal(-0.5, -1,0); manual->textureCoord(1, 0.5);
    manual->position(0, -4,-1); manual->normal(0, -4,-1); manual->textureCoord(1, 1);
    manual->end();
    
    // nose
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(0, 4,-0.5); manual->normal(0, 4,-0.5); manual->textureCoord(0.25, 0.75);
    manual->position(0.5, 3,-1); manual->normal(0.5, 3,-1); manual->textureCoord(0, 0.5);
    manual->position(-0.5, 3,-1); manual->normal(-0.5, 3,-1); manual->textureCoord(0.5, 0.5);
    manual->position(-0.5, 3,0); manual->normal(-0.5, 3,0); manual->textureCoord(0.5, 1);
    manual->position(0.5, 3,0); manual->normal(0.5, 3,0); manual->textureCoord(0, 1);
    manual->position(0.5, 3,-1); manual->normal(0.5, 3,-1); manual->textureCoord(0, 0.5);
    manual->end();
    
    // Engines are counted from right to left
    
    // 1st engine - front
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(-3.5, 1.25,-1); manual->normal(-3.5, 1.25,-1); manual->textureCoord(0, 0.5);
    manual->position(-3.5, 1.25, -0.75); manual->normal(-3.5, 1.25, -0.75); manual->textureCoord(0, 0);
    manual->position(-3, 1.25, -0.75); manual->normal(-3, 1.25, -0.75); manual->textureCoord(1, 0);
    manual->position(-3, 1.25, -1); manual->normal(-3, 1.25, -1); manual->textureCoord(1, 0.5);
    manual->end();
    // 1st engine - back
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(-3, 1.25, -1); manual->normal(-3, 1.25, -1); manual->textureCoord(1, 0.5);
    manual->position(-3, 1.25, -0.75); manual->normal(-3, 1.25, -0.75); manual->textureCoord(1, 0);
    manual->position(-3.5, 1.25, -0.75); manual->normal(-3.5, 1.25, -0.75); manual->textureCoord(0, 0);
    manual->position(-3.5, 1.25,-1); manual->normal(-3.5, 1.25,-1); manual->textureCoord(0, 0.5);
    manual->end();
    // 1st engine - top 
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(-3.5, 0.5,-1); manual->normal(-3.5, 0.5,-1); manual->textureCoord(0.5, 0.5);
    manual->position(-3.5, 1.25, -1); manual->normal(-3.5, 1.25, -1); manual->textureCoord(1, 0.5);
    manual->position(-3, 1.25, -1); manual->normal(-3, 1.25, -1); manual->textureCoord(1, 1);
    manual->position(-3, 0.5, -1); manual->normal(-3, 0.5, -1); manual->textureCoord(0.5, 0.5);
    manual->end();
    // 1st engine - right
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(-3.5, 0.5, -0.75); manual->normal(-3.5, 0.5, -0.75); manual->textureCoord(0.5, 0.5);
    manual->position(-3.5, 1.25, -0.75); manual->normal(-3.5, 1.25, -0.75); manual->textureCoord(1, 0.5);
    manual->position(-3.5, 1.25, -1); manual->normal(-3.5, 1.25, -1); manual->textureCoord(1, 1);
    manual->position(-3.5, 0.5, -1); manual->normal(-3.5, 0.5, -1); manual->textureCoord(0.5, 0.5);
    manual->end();
    // 1st engine - left
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(-3, 0.5, -1); manual->normal(-3, 0.5, -1); manual->textureCoord(0.5, 0.5);
    manual->position(-3, 1.25, -1); manual->normal(-3, 1.25, -1); manual->textureCoord(1, 1);
    manual->position(-3, 1.25, -0.75); manual->normal(-3, 1.25, -0.75); manual->textureCoord(1, 0.5);
    manual->position(-3, 0.5, -0.75); manual->normal(-3, 0.5, -0.75); manual->textureCoord(0.5, 0.5);
    manual->end();
    // 1st engine - bottom (counted from right)
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(-3, 1.25, -0.75); manual->normal(-3, 1.25, -0.75); manual->textureCoord(1, 1);
    manual->position(-3.5, 1.25, -0.75); manual->normal(-3.5, 1.25, -0.75); manual->textureCoord(1, 0.5);
    manual->position(-3.5, 0.5,-0.75); manual->normal(-3.5, 0.5, -0.75); manual->textureCoord(0.5, 0.5);
    manual->position(-3, 0.5, -0.75); manual->normal(-3, 0.5, -0.75); manual->textureCoord(0.5, 0.5);
    manual->end();
    
    // 2nd engine - front
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(-2, 2,-1); manual->normal(-2, 2,-1); manual->textureCoord(0, 0.5);
    manual->position(-2, 2, -0.75); manual->normal(-2, 2, -0.75); manual->textureCoord(0, 0);
    manual->position(-1.5, 2, -0.75); manual->normal(-1.5, 2, -0.75); manual->textureCoord(1, 0);
    manual->position(-1.5, 2, -1); manual->normal(-1.5, 2, -1); manual->textureCoord(1, 0.5);
    manual->end();
    // 2nd engine - back
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(-1.5, 2, -1); manual->normal(-1.5, 2, -1); manual->textureCoord(1, 0.5);
    manual->position(-1.5, 2, -0.75); manual->normal(-1.5, 2, -0.75); manual->textureCoord(1, 0);
    manual->position(-2, 2, -0.75); manual->normal(-2, 2, -0.75); manual->textureCoord(0, 0);
    manual->position(-2, 2,-1); manual->normal(-2, 2,-1); manual->textureCoord(0, 0.5);
    manual->end();
    // 2nd engine - top 
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(-2, 1.25,-1); manual->normal(-2, 1.25,-1); manual->textureCoord(0.5, 0.5);
    manual->position(-2, 2, -1); manual->normal(-2, 2, -1); manual->textureCoord(1, 0.5);
    manual->position(-1.5, 2, -1); manual->normal(-1.5, 2, -1); manual->textureCoord(1, 1);
    manual->position(-1.5, 1.25, -1); manual->normal(-1.5, 1.25, -1); manual->textureCoord(0.5, 0.5);
    manual->end();
    // 2nd engine - right
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(-2, 1.25, -0.75); manual->normal(-2, 1.25, -0.75); manual->textureCoord(0.5, 0.5);
    manual->position(-2, 2, -0.75); manual->normal(-2, 2, -0.75); manual->textureCoord(1, 0.5);
    manual->position(-2, 2, -1); manual->normal(-2, 2, -1); manual->textureCoord(1, 1);
    manual->position(-2, 1.25, -1); manual->normal(-2, 1.25, -1); manual->textureCoord(0.5, 0.5);
    manual->end();
    // 2nd engine - left
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(-1.5, 1.25, -1); manual->normal(-1.5, 1.25, -1); manual->textureCoord(0.5, 0.5);
    manual->position(-1.5, 2, -1); manual->normal(-1.5, 2, -1); manual->textureCoord(1, 1);
    manual->position(-1.5, 2, -0.75); manual->normal(-1.5, 2, -0.75); manual->textureCoord(1, 0.5);
    manual->position(-1.5, 1.25, -0.75); manual->normal(-1.5, 1.25, -0.75); manual->textureCoord(0.5, 0.5);
    manual->end();
    // 2nd engine - bottom (counted from right)
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(-1.5, 2, -0.75); manual->normal(-1.5, 2, -0.75); manual->textureCoord(1, 1);
    manual->position(-2, 2, -0.75); manual->normal(-2, 2, -0.75); manual->textureCoord(1, 0.5);
    manual->position(-2, 1.25,-0.75); manual->normal(-2, 1.25, -0.75); manual->textureCoord(0.5, 0.5);
    manual->position(-1.5, 1.25, -0.75); manual->normal(-1.5, 1.25, -0.75); manual->textureCoord(0.5, 0.5);
    manual->end();
    
    // 3rd engine - front
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1.5, 2, -1); manual->normal(1.5, 2, -1); manual->textureCoord(1, 0.5);
    manual->position(1.5, 2, -0.75); manual->normal(1.5, 2, -0.75); manual->textureCoord(1, 0);
    manual->position(2, 2, -0.75); manual->normal(2, 2, -0.75); manual->textureCoord(0, 0);
    manual->position(2, 2,-1); manual->normal(2, 2,-1); manual->textureCoord(0, 0.5);
    manual->end();
    // 3rd engine - back
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(2, 1.25,-1); manual->normal(2, 1.25,-1); manual->textureCoord(0, 0.5);
    manual->position(2, 1.25, -0.75); manual->normal(2, 1.25, -0.75); manual->textureCoord(0, 0);
    manual->position(1.5, 1.25, -0.75); manual->normal(1.5, 1.25, -0.75); manual->textureCoord(1, 0);
    manual->position(1.5, 1.25, -1); manual->normal(1.5, 1.25, -1); manual->textureCoord(1, 0.5);
    manual->end();
    // 3rd engine - top 
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1.5, 1.25, -1); manual->normal(1.5, 1.25, -1); manual->textureCoord(0.5, 0.5);
    manual->position(1.5, 2, -1); manual->normal(1.5, 2, -1); manual->textureCoord(1, 1);
    manual->position(2, 2, -1); manual->normal(2, 2, -1); manual->textureCoord(1, 0.5);
    manual->position(2, 1.25,-1); manual->normal(2, 1.25,-1); manual->textureCoord(0.5, 0.5);
    manual->end();
    // 3rd engine - right
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(2, 1.25, -1); manual->normal(2, 1.25, -1); manual->textureCoord(0.5, 0.5);
    manual->position(2, 2, -1); manual->normal(2, 2, -1); manual->textureCoord(1, 1);
    manual->position(2, 2, -0.75); manual->normal(2, 2, -0.75); manual->textureCoord(1, 0.5);
    manual->position(2, 1.25, -0.75); manual->normal(2, 1.25, -0.75); manual->textureCoord(0.5, 0.5);
    manual->end();
    // 3rd engine - left
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1.5, 1.25, -0.75); manual->normal(1.5, 1.25, -0.75); manual->textureCoord(0.5, 0.5);
    manual->position(1.5, 2, -0.75); manual->normal(1.5, 2, -0.75); manual->textureCoord(1, 0.5);
    manual->position(1.5, 2, -1); manual->normal(1.5, 2, -1); manual->textureCoord(1, 1);
    manual->position(1.5, 1.25, -1); manual->normal(1.5, 1.25, -1); manual->textureCoord(0.5, 0.5);
    manual->end();
    // 3rd engine - bottom (counted from right)
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1.5, 1.25, -0.75); manual->normal(1.5, 1.25, -0.75); manual->textureCoord(0.5, 0.5);
    manual->position(2, 1.25, -0.75); manual->normal(2, 1.25, -0.75); manual->textureCoord(0.5, 0.5);
    manual->position(2, 2, -0.75); manual->normal(2, 2, -0.75); manual->textureCoord(1, 0.5);
    manual->position(1.5, 2, -0.75); manual->normal(1.5, 2, -0.75); manual->textureCoord(1, 1);
    manual->end();
    
    // 4th engine - front
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(3, 1.25, -1); manual->normal(3, 1.25, -1); manual->textureCoord(1, 0.5);
    manual->position(3, 1.25, -0.75); manual->normal(3, 1.25, -0.75); manual->textureCoord(1, 0);
    manual->position(3.5, 1.25, -0.75); manual->normal(-3.5, 1.25, -0.75); manual->textureCoord(0, 0);
    manual->position(3.5, 1.25,-1); manual->normal(3.5, 1.25,-1); manual->textureCoord(0, 0.5);
    manual->end();
    // 4th engine - back
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(3.5, 1.25,-1); manual->normal(3.5, 1.25,-1); manual->textureCoord(0, 0.5);
    manual->position(3.5, 1.25, -0.75); manual->normal(-3.5, 1.25, -0.75); manual->textureCoord(0, 0);
    manual->position(3, 1.25, -0.75); manual->normal(3, 1.25, -0.75); manual->textureCoord(1, 0);
    manual->position(3, 1.25, -1); manual->normal(3, 1.25, -1); manual->textureCoord(1, 0.5);
    manual->end();
    // 4th engine - top
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(3, 0.5, -1); manual->normal(3, 0.5, -1); manual->textureCoord(0.5, 0.5);
    manual->position(3, 1.25, -1); manual->normal(3, 1.25, -1); manual->textureCoord(1, 1);
    manual->position(3.5, 1.25, -1); manual->normal(3.5, 1.25, -1); manual->textureCoord(1, 0.5);
    manual->position(3.5, 0.5,-1); manual->normal(3.5, 0.5,-1); manual->textureCoord(0.5, 0.5);
    manual->end();
    // 4th engine - right
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(3.5, 0.5, -1); manual->normal(3.5, 0.5, -1); manual->textureCoord(0.5, 0.5);
    manual->position(3.5, 1.25, -1); manual->normal(3.5, 1.25, -1); manual->textureCoord(1, 1);
    manual->position(3.5, 1.25, -0.75); manual->normal(3.5, 1.25, -0.75); manual->textureCoord(1, 0.5);
    manual->position(3.5, 0.5, -0.75); manual->normal(3.5, 0.5, -0.75); manual->textureCoord(0.5, 0.5);
    manual->end();
    // 4th engine - left
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(3, 0.5, -0.75); manual->normal(3, 0.5, -0.75); manual->textureCoord(0.5, 0.5);
    manual->position(3, 1.25, -0.75); manual->normal(3, 1.25, -0.75); manual->textureCoord(1, 0.5);
    manual->position(3, 1.25, -1); manual->normal(3, 1.25, -1); manual->textureCoord(1, 1);
    manual->position(3, 0.5, -1); manual->normal(3, 0.5, -1); manual->textureCoord(0.5, 0.5);
    manual->end();
    // 4th engine - bottom (counted from right)
    manual->begin(mat_body, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(3, 0.5, -0.75); manual->normal(3, 0.5, -0.75); manual->textureCoord(0.5, 0.5);
    manual->position(3.5, 0.5,-0.75); manual->normal(3.5, 0.5, -0.75); manual->textureCoord(0.5, 0.5);
    manual->position(3.5, 1.25, -0.75); manual->normal(3.5, 1.25, -0.75); manual->textureCoord(1, 0.5);
    manual->position(3, 1.25, -0.75); manual->normal(3, 1.25, -0.75); manual->textureCoord(1, 1);
    manual->end();
    
    
    manual->begin(mat_wing, RenderOperation::OT_TRIANGLE_LIST);
    // rudder - left
    manual->position(0, -3, -1); manual->normal(-1, 0, 0); manual->textureCoord(0, 1);
    manual->position(0, -4,-2); manual->normal(-1, 0, 0); manual->textureCoord(0.5, 0.5);
    manual->position(0, -4, -1); manual->normal(-1, 0, 0); manual->textureCoord(0.5, 1);
    manual->position(0, -4, -1); manual->normal(-1, 0, 0); manual->textureCoord(0.5, 1);
    manual->position(0, -4, -2); manual->normal(-1, 0, 0); manual->textureCoord(0.5, 0.5);
    manual->position(0, -4.5, -2); manual->normal(-1, 0, 0); manual->textureCoord(1, 0.5);
    // rudder - right
    manual->position(0, -4, -1); manual->normal(1, 0, 0); manual->textureCoord(0.5, 1);
    manual->position(0, -4,-2); manual->normal(1, 0, 0); manual->textureCoord(0.5, 0.5);
    manual->position(0, -3, -1); manual->normal(1, 0, 0); manual->textureCoord(0, 1);
    manual->position(0, -4.5, -2); manual->normal(1, 0, 0); manual->textureCoord(1, 0.5);
    manual->position(0, -4, -2); manual->normal(1, 0, 0); manual->textureCoord(0.5, 0.5);
    manual->position(0, -4, -1); manual->normal(1, 0, 0); manual->textureCoord(0.5, 1);
    manual->end();
    
    manual->begin(mat_wing, RenderOperation::OT_TRIANGLE_LIST);
    // right elevator - top
    manual->position(0, -3, -0.75); manual->normal(0, 0, -1); manual->textureCoord(1, 0.5);
    manual->position(-1, -4,-0.75); manual->normal(0, 0, -1); manual->textureCoord(0, 0);
    manual->position(0, -4, -0.75); manual->normal(0, 0, -1); manual->textureCoord(1, 0);
    manual->position(-1, -4, -0.75); manual->normal(0, 0, -1); manual->textureCoord(1, 0);
    manual->position(-1, -4.5, -0.75); manual->normal(0, 0, -1); manual->textureCoord(1, 0.5);
    manual->position(0, -4, -0.75); manual->normal(0, 0, -1); manual->textureCoord(0, 0);
    // right elevator - bottom
    manual->position(0, -4, -0.75); manual->normal(0, 0, 1); manual->textureCoord(1, 0);
    manual->position(-1, -4,-0.75); manual->normal(0, 0, 1); manual->textureCoord(0, 0);
    manual->position(0, -3, -0.75); manual->normal(0, 0, 1); manual->textureCoord(1, 0.5);
    manual->position(0, -4, -0.75); manual->normal(0, 0, 1); manual->textureCoord(0, 0);
    manual->position(-1, -4.5, -0.75); manual->normal(0, 0, 1); manual->textureCoord(1, 0.5);
    manual->position(-1, -4, -0.75); manual->normal(0, 0, 1); manual->textureCoord(1, 0);
    // left elevator - top
    manual->position(0, -4, -0.75); manual->normal(0, 0, -1); manual->textureCoord(0, 0);
    manual->position(1, -4.5, -0.75); manual->normal(0, 0, -1); manual->textureCoord(1, 0.5);
    manual->position(1, -4, -0.75); manual->normal(0, 0, -1); manual->textureCoord(1, 0);
    manual->position(0, -4, -0.75); manual->normal(0, 0, -1); manual->textureCoord(1, 0);
    manual->position(1, -4,-0.75); manual->normal(0, 0, -1); manual->textureCoord(0, 0);
    manual->position(0, -3, -0.75); manual->normal(0, 0, -1); manual->textureCoord(1, 0.5);
    // left elevator - bottom
    manual->position(1, -4, -0.75); manual->normal(0, 0, 1); manual->textureCoord(1, 0);
    manual->position(1, -4.5, -0.75); manual->normal(0, 0, 1); manual->textureCoord(1, 0.5);
    manual->position(0, -4, -0.75); manual->normal(0, 0, 1); manual->textureCoord(0, 0);
    manual->position(0, -3, -0.75); manual->normal(0, 0, 1); manual->textureCoord(1, 0.5);
    manual->position(1, -4,-0.75); manual->normal(0, 0, 1); manual->textureCoord(0, 0);
    manual->position(0, -4, -0.75); manual->normal(0, 0, 1); manual->textureCoord(1, 0);
    manual->end();
    
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode(name);
    Ogre::MeshPtr newMesh = manual->convertToMesh(name);
    myEntity = scnMgr->createEntity(name, name);
    rootNode->attachObject(myEntity);
    rootNode->scale(50, 50, 50);
    //rootNode->translate(0,0,-300, Node::TS_LOCAL);
}

void Unit::createFighterObject(Ogre::SceneManager* scnMgr){
    string s = "Unit_";
    id = unitId;
    name = s.append(intToStr(unitId));
    unitId += 1;
    
    ManualObject* manual = scnMgr->createManualObject(name);
    manual->setDynamic(true);
    
    string mat;
    if(owner == Owner_BLUE) mat = "MyMaterials/fighter_blue";
    else if(owner == Owner_RED) mat = "MyMaterials/fighter_red";
    else mat = "MyMaterials/White";
    
    // body - top
    manual->begin(mat, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(0, 2, 0); manual->normal(0, 2, 0); manual->textureCoord(0.5, 0);  
    manual->position(2, -3, 0); manual->normal(0, -3, 0); manual->textureCoord(0, 1);  
    manual->position(-2, -3, 0); manual->normal(0, -3, 0); manual->textureCoord(1, 1);
    manual->position(0, 2, 0); manual->normal(0, 2, 0); manual->textureCoord(0.5, 0);
    manual->position(0, 1, -0.75); manual->normal(0, 1, 0.75); manual->textureCoord(0.5, 0.25);
    manual->end();
    
    // body - bottom
    manual->begin("MyMaterials/White", RenderOperation::OT_TRIANGLE_LIST);
    manual->position(0, 2, 0); manual->normal(0, 0, 1); 
    manual->position(2, -3, 0); manual->normal(0, 0, 1); 
    manual->position(-2, -3, 0); manual->normal(0, 0, 1); 
    manual->end();
    
    // rudder - left side
    manual->begin(mat, RenderOperation::OT_TRIANGLE_LIST);
    manual->position(0, -2, 0); manual->normal(1, 0, 0); manual->textureCoord(1, 0.5);
    manual->position(0, -3, -1); manual->normal(1, 0, 0); manual->textureCoord(0.5, 0);
    manual->position(0, -3, 0); manual->normal(1, 0, 0); manual->textureCoord(1, 0);
    manual->end();
    
    // rudder - right side
    manual->begin(mat, RenderOperation::OT_TRIANGLE_LIST);
    manual->position(0, -3, 0); manual->normal(-1, 0, 0); manual->textureCoord(0, 0);
    manual->position(0, -3, -1); manual->normal(-1, 0, 0); manual->textureCoord(0.5, 0);
    manual->position(0, -2, 0); manual->normal(-1, 0, 0); manual->textureCoord(0, 0.5);
    manual->end();
    
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode(name);
    Ogre::MeshPtr newMesh = manual->convertToMesh(name);
    myEntity = scnMgr->createEntity(name, name);
    rootNode->attachObject(myEntity);
    rootNode->scale(50, 50, 50);
    //rootNode->translate(0,0,-300, Node::TS_LOCAL);
}

void Unit::createFlatObject(Ogre::SceneManager* scnMgr){
    
}

void Unit::createNavyObject(Ogre::SceneManager* scnMgr){
    string s = "Unit_";
    id = unitId;
    name = s.append(intToStr(unitId));
    unitId += 1;
    
    ManualObject* manual = scnMgr->createManualObject(name);
    manual->setDynamic(true);
    
    string mat;
    if(myType == Unit_SUBMARINE){
        if(owner == Owner_BLUE) mat = "MyMaterials/submarine_blue";
        else if(owner == Owner_RED) mat = "MyMaterials/submarine_red";
        else mat = "MyMaterials/White";
    }
    else if(myType == Unit_DESTROYER){
        if(owner == Owner_BLUE) mat = "MyMaterials/destroyer_blue";
        else if(owner == Owner_RED) mat = "MyMaterials/destroyer_red";
        else mat = "MyMaterials/White";
    } 
    else if(myType == Unit_CARRIER){
        if(owner == Owner_BLUE) mat = "MyMaterials/carrier_blue";
        else if(owner == Owner_RED) mat = "MyMaterials/carrier_red";
        else mat = "MyMaterials/White";
    } 
    else{
        if(owner == Owner_BLUE) mat = "MyMaterials/Blue";
        else if(owner == Owner_RED) mat = "MyMaterials/Red";
        else mat = "MyMaterials/White";
    }
    
    float twoPi = 6.28318530718;
    int numEdge = 24;
    float dtheta = twoPi / numEdge;
    float r = 1.0;
    float theta = 0;
    
    float x, y;
    
    
    // front 
    manual->begin("MyMaterials/White", RenderOperation::OT_TRIANGLE_FAN);
    for(int i=0;i<numEdge;i++){
        theta = i * dtheta; 
        x = r*cos(theta);
        y = r*sin(theta);
        manual->position(x, y, 1); manual->normal(0,0,1); 
    } 
    manual->position(0, 0, 1); manual->normal(0,0,1); 
    manual->end();
    
    // back
    manual->begin(mat, RenderOperation::OT_TRIANGLE_FAN);
    for(int i=numEdge;i>0;i--){
        theta = i * dtheta; 
        x = r*cos(theta);
        y = r*sin(theta);
        manual->position(x, y, -1); manual->normal(0,0,-1); manual->textureCoord((x+1)*0.5, (y+1)*0.5);
    } 
    manual->position(0, 0, -1); manual->normal(0,0,-1); manual->textureCoord((x+1)*0.5, (y+1)*0.5);
    manual->end();
    
    // sides
    float x_next, y_next;
    for(int i=0;i<numEdge;i++){
        theta = i * dtheta; 
        x = r*cos(theta);
        y = r*sin(theta);
        x_next = r*cos(theta + dtheta);
        y_next = r*sin(theta + dtheta);
        manual->begin("MyMaterials/White", RenderOperation::OT_TRIANGLE_FAN);
        manual->position(x, y, -1); manual->normal(0,0,-1); 
        manual->position(x_next, y_next, -1); manual->normal(0,0,-1); 
        manual->position(x_next, y_next, 1); manual->normal(0,0,1); 
        manual->position(x, y, 1); manual->normal(0,0,1); 
        manual->position(x, y, -1); manual->normal(0,0,-1); 
        manual->end();
    }
    
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode(name);
    Ogre::MeshPtr newMesh = manual->convertToMesh(name);
    myEntity = scnMgr->createEntity(name, name);
    rootNode->attachObject(myEntity);
    rootNode->scale(150, 150, 30);
    rootNode->rotate(Ogre::Vector3(0,0,1), Ogre::Radian(3.14159265359), Node::TS_LOCAL);
}

void Unit::createSymbolObject(Ogre::SceneManager* scnMgr){
    string s = "Unit_";
    id = unitId;
    name = s.append(intToStr(unitId));
    unitId += 1;
    
    ManualObject* manual = scnMgr->createManualObject(name);
    manual->setDynamic(true);
    
    string mat;
    if(myType == Unit_INFANTRY){
        if(owner == Owner_BLUE) mat = "MyMaterials/infantry_blue";
        else if(owner == Owner_RED) mat = "MyMaterials/infantry_red";
        else mat = "MyMaterials/infantry";
    }
    else if(myType == Unit_TANK){
        if(owner == Owner_BLUE) mat = "MyMaterials/tank_blue";
        else if(owner == Owner_RED) mat = "MyMaterials/tank_red";
        else mat = "MyMaterials/tank";
    } 
    else if(myType == Unit_SCUD){
        if(owner == Owner_BLUE) mat = "MyMaterials/scud_blue";
        else if(owner == Owner_RED) mat = "MyMaterials/scud_red";
        else mat = "MyMaterials/scud";
    } 
    else{
        if(owner == Owner_BLUE) mat = "MyMaterials/Blue";
        else if(owner == Owner_RED) mat = "MyMaterials/Red";
        else mat = "MyMaterials/White";
    }
    
    // top
    manual->begin("MyMaterials/White", RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, 1, -1); manual->normal(0,1,0); manual->textureCoord(1,1);
    manual->position(-1, 1, -1); manual->normal(0,1,0); manual->textureCoord(0,1);
    manual->position(-1, 1, 1); manual->normal(0,1,0); manual->textureCoord(0,0);
    manual->position(1, 1, 1); manual->normal(0,1,0); manual->textureCoord(1,0);
    manual->end();
    
    // bottom
    manual->begin("MyMaterials/White", RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, -1, 1); manual->normal(0,-1,0); manual->textureCoord(1,1);
    manual->position(-1, -1, 1); manual->normal(0,-1,0); manual->textureCoord(0,1);
    manual->position(-1, -1, -1); manual->normal(0,-1,0); manual->textureCoord(0,0);
    manual->position(1, -1, -1); manual->normal(0,-1,0); manual->textureCoord(1,0);
    manual->end();
    
    // front
    manual->begin("MyMaterials/White", RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, 1, 1); manual->normal(0,0,1); manual->textureCoord(1,1);
    manual->position(-1, 1, 1); manual->normal(0,0,1); manual->textureCoord(0,1);
    manual->position(-1, -1, 1); manual->normal(0,0,1); manual->textureCoord(0,0);
    manual->position(1, -1, 1); manual->normal(0,0,1); manual->textureCoord(1,0);
    manual->end();
    
    // back
    manual->begin(mat, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, 1, -1); manual->normal(0,0,-1); manual->textureCoord(1,1);
    manual->position(1, -1, -1); manual->normal(0,0,-1); manual->textureCoord(0,1);
    manual->position(-1, -1, -1); manual->normal(0,0,-1); manual->textureCoord(0,0);
    manual->position(-1, 1, -1); manual->normal(0,0,-1); manual->textureCoord(1,0);
    manual->end();
    
    // left
    manual->begin("MyMaterials/White", RenderOperation::OT_TRIANGLE_FAN);
    manual->position(-1, 1, 1); manual->normal(-1,0,0); manual->textureCoord(1,1);
    manual->position(-1, 1, -1); manual->normal(-1,0,0); manual->textureCoord(0,1);
    manual->position(-1, -1, -1); manual->normal(-1,0,0); manual->textureCoord(0,0);
    manual->position(-1, -1, 1); manual->normal(-1,0,0); manual->textureCoord(1,0);
    manual->end();
    
    // right
    manual->begin("MyMaterials/White", RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, 1, -1); manual->normal(1,0,0); manual->textureCoord(1,1);
    manual->position(1, 1, 1); manual->normal(1,0,0); manual->textureCoord(0,1);
    manual->position(1, -1, 1); manual->normal(1,0,0); manual->textureCoord(0,0);
    manual->position(1, -1, -1); manual->normal(1,0,0); manual->textureCoord(1,0);
    manual->end();
    
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode(name);
    Ogre::MeshPtr newMesh = manual->convertToMesh(name);
    myEntity = scnMgr->createEntity(name, name);
    rootNode->attachObject(myEntity);
    rootNode->scale(100, 150, 30);
}

void Unit::createCommandObject(Ogre::SceneManager* scnMgr){
    string s = "Unit_";
    id = unitId;
    name = s.append(intToStr(unitId));
    unitId += 1;
    
    ManualObject* manual = scnMgr->createManualObject(name);
    manual->setDynamic(true);
    
    string mat;
    if(owner == Owner_BLUE) mat = "MyMaterials/command_blue";
    else if(owner == Owner_RED) mat = "MyMaterials/command_red";
    else mat = "MyMaterials/White";
    
    // top
    manual->begin(mat, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(0.75, 0.75, -1); manual->normal(0,1,0); manual->textureCoord(0.5,0.5);
    manual->position(-0.75, 0.75, -1); manual->normal(0,1,0); manual->textureCoord(1,0.5);
    manual->position(-1, 1, 0); manual->normal(0,1,0); manual->textureCoord(1,1);
    manual->position(1, 1, 0); manual->normal(0,1,0); manual->textureCoord(0.5,1);
    manual->end();
    
    // bottom
    manual->begin(mat, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, -1, 0); manual->normal(0,-1,0); manual->textureCoord(0.5,1);
    manual->position(-1, -1, 0); manual->normal(0,-1,0); manual->textureCoord(1,1);
    manual->position(-0.75, -0.75, -1); manual->normal(0,-1,0); manual->textureCoord(1,0.5);
    manual->position(0.75, -0.75, -1); manual->normal(0,-1,0); manual->textureCoord(0.5,0.5);
    manual->end();
    
    // front
    manual->begin("MyMaterials/White", RenderOperation::OT_TRIANGLE_FAN);
    manual->position(1, 1, 0); manual->normal(0,0,1); 
    manual->position(-1, 1, 0); manual->normal(0,0,1); 
    manual->position(-1, -1, 0); manual->normal(0,0,1); 
    manual->position(1, -1, 0); manual->normal(0,0,1); 
    manual->end();
    
    // back
    manual->begin(mat, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(0.75, 0.75, -1); manual->normal(0,0,-1); manual->textureCoord(0,0);
    manual->position(0.75, -0.75, -1); manual->normal(0,0,-1); manual->textureCoord(0,0.5);
    manual->position(-0.75, -0.75, -1); manual->normal(0,0,-1); manual->textureCoord(0.5,0.5);
    manual->position(-0.75, 0.75, -1); manual->normal(0,0,-1); manual->textureCoord(0.5,0);
    manual->end();
    
    // left
    manual->begin(mat, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(-1, 1, 0); manual->normal(-1,0,0); manual->textureCoord(0.5,1);
    manual->position(-0.75, 0.75, -1); manual->normal(-1,0,0); manual->textureCoord(0.5,0.5);
    manual->position(-0.75, -0.75, -1); manual->normal(-1,0,0); manual->textureCoord(0,0.5);
    manual->position(-1, -1, 0); manual->normal(-1,0,0); manual->textureCoord(0,1);
    manual->end();
    
    // right
    manual->begin(mat, RenderOperation::OT_TRIANGLE_FAN);
    manual->position(0.75, 0.75, -1); manual->normal(1,0,0); manual->textureCoord(0.5,0.5);
    manual->position(1, 1, 0); manual->normal(1,0,0); manual->textureCoord(0.5,1);
    manual->position(1, -1, 0); manual->normal(1,0,0); manual->textureCoord(0,1);
    manual->position(0.75, -0.75, -1); manual->normal(1,0,0); manual->textureCoord(0,0.5);
    manual->end();
    
    rootNode = scnMgr->getRootSceneNode()->createChildSceneNode(name);
    Ogre::MeshPtr newMesh = manual->convertToMesh(name);
    myEntity = scnMgr->createEntity(name, name);
    rootNode->attachObject(myEntity);
    rootNode->scale(150, 150, height);
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
    rootNode->scale(150, 150, height);
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
    if(myType >= 10 && myType <= 12){
        rootNode->translate(0,0,-300, Node::TS_LOCAL);
    }
    rootNode->translate(0,0,height, Node::TS_LOCAL);
}

void Unit::translate(float x, float y, float z){
    position += btVector3(x,y,z);
    rootNode->translate(x,y,z, Node::TS_LOCAL);
}

void Unit::setDirection(btVector3& d){ //need repair
    rootNode->setDirection(Ogre::Vector3(d.x(), d.y(), d.z()), Node::TS_PARENT);
}

void Unit::grow(){
    rootNode->translate(0, 0, -buildInterval, Node::TS_LOCAL);
}

void Unit::destroy(Ogre::SceneManager* scnMgr){
    scnMgr->getRootSceneNode()->removeAndDestroyChild(name);
    
}

