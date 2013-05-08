#ifndef __PLANET_H__
#define __PLANET_H__

#include <OgreSubEntity.h>
#include <OgreMaterialManager.h>
#include <vector>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()
#include <iostream>
#include <sstream>
#include <math.h>
#include <cmath>
#include <string>
#include <algorithm>
#include <btBulletCollisionCommon.h>
#include "Ogre.h"

#define PI 3.1415926535897932384626433832795

using namespace std;

class PlanetVertex: public btVector3{
    public:
    size_t id;
    float altitude;
    float longitude; 
    float latitude;
    vector<size_t> neighbors;
    bool positive;
    
    PlanetVertex(){
        altitude = 0;
        longitude = 0;
        latitude = 0;
        positive = true;
    }
    
    PlanetVertex( const float a, const float b, const float c ){
        setX(a);
        setY(b);
        setZ(c);
        altitude = 0;
        longitude = 0;
        latitude = 0;
        positive = true;
    }
    
    PlanetVertex( const btVector3& v ){
        setX(v.x()); 
        setY(v.y()); 
        setZ(v.z());
        altitude = 0;
        longitude = 0;
        latitude = 0;
        positive = true;
    }
    
    PlanetVertex( const PlanetVertex& v ){
        id = v.id; //critical
        setX(v.x()); 
        setY(v.y()); 
        setZ(v.z());
        altitude = v.altitude;
        longitude = v.longitude;
        latitude = v.latitude;
        positive = v.positive;
        if(positive) neighbors = v.neighbors;
    }
    
    PlanetVertex( btVector3& v ){
        setX(v.x()); 
        setY(v.y()); 
        setZ(v.z());
        altitude = 0;
        longitude = 0;
        latitude = 0;
        positive = true;
    }
        
    PlanetVertex( PlanetVertex& v ){
        id = v.id; //critical
        setX(v.x()); 
        setY(v.y()); 
        setZ(v.z());
        altitude = v.altitude;
        longitude = v.longitude;
        latitude = v.latitude;
        positive = v.positive;
        if(positive) neighbors = v.neighbors;
    }
    
    PlanetVertex& operator=( const PlanetVertex& v ){
        id = v.id; //critical
        setX(v.x()); 
        setY(v.y()); 
        setZ(v.z());
        altitude = v.altitude;
        longitude = v.longitude;
        latitude = v.latitude;
        positive = v.positive;
        if(positive) neighbors = v.neighbors;
        return *this;
    }
    
    bool equals(const PlanetVertex& v) const {
        if(x()==v.x() && y()==v.y() && z()==v.z()) return true;
        else return false;
    }
    
    operator bool() const {
        return positive;
    }
    
    void marry(PlanetVertex& v){
        if(positive && v.positive){
            bool isNew = true;
            for(int i=0;i<neighbors.size();i++){
                if(neighbors[i]==v.id){
                    isNew = false;
                    break;
                }
            }
            if(isNew){
                neighbors.push_back(v.id);
                v.neighbors.push_back(id);
            }
        }
    }
    
    bool isMarriedTo(PlanetVertex& v){
        for(int i=0;i<neighbors.size();i++){
            if(neighbors[i]==v.id) return true;
        }
        return true;
    }
};

class PlanetFace{
    public:
    size_t id;
    int v[3];
    btVector3 center;
    btVector3 normal;
    float angle;
    bool positive;
    size_t parentCellId;
    
    PlanetFace(){
        v[0] = -1;
        v[1] = -1;
        v[2] = -1;
        center = btVector3(0,0,0);
        normal = btVector3(0,0,0);
        float angle = 0;
        positive = true;
        parentCellId = -1;
    }
    
    PlanetFace(PlanetVertex& a, PlanetVertex& b, PlanetVertex& c){
        v[0] = a.id;
        v[1] = b.id;
        v[2] = c.id;
        center = (a + b + c)/3.0;
        normal = (b-a).cross(c-a);
        angle = center.angle(normal);
        positive = true;
        parentCellId = -1;
    }
        
    PlanetFace(const PlanetFace& f){
        v[0] = f.v[0];
        v[1] = f.v[1];
        v[2] = f.v[2];
        center = f.center;
        normal = f.normal;
        angle = f.angle;
        positive = f.positive;
        parentCellId = f.parentCellId;
    }
        
    PlanetFace& operator=(const PlanetFace& f){
        id = f.id;
        v[0] = f.v[0];
        v[1] = f.v[1];
        v[2] = f.v[2];
        center = f.center;
        normal = f.normal;
        angle = f.angle;
        positive = f.positive;
        parentCellId = f.parentCellId;
        return *this;
    }
    
    operator bool() const {
        return positive;
    }
    
    void setParent(size_t id){
        parentCellId = id;
    }
};

enum terrainType{
    Terrain_WATER = 0,
    Terrain_WATERLAND = 1,
    Terrain_LAND = 2
};

enum radiationLevel{
    Radiation_SAFE = 0,
    Radiation_LOW = 1,
    Radiation_MODERATE = 2,
    Radiation_HIGH = 3,
    Radiation_DEADLY = 4
};

enum ownerType{
    Owner_RED = 0,
    Owner_NEUTRAL = 1,
    Owner_BLUE = 2
}; 

class PlanetCell{
    public: 
    size_t id; 
    size_t centerId;
    size_t owner; //id of the player who owns the territory
    size_t occupier; //id of the player who occupies the territory
    vector<size_t> neighbors; //same as the center vertex neighbors
    vector<size_t> paramVerts; //parameter vertices
    vector<size_t> borderSegments; 
    float altitude;
    float longitude;
    float latitude;
    vector<size_t> faces; //triangle faces that belong to this cell
    
    terrainType terrain;
    float avg_color;
    float popDensity;
    
    bool visible;
    bool selected;
    
    int myUnit;
    int myUnit_pending;
    int myUnitId;
    int goalId;
    
    bool moving;
    bool building;
    bool capturing;
    bool unitbuilding;
    int timer;
    time_t timeNeeded;
    int growsCompleted;
    
    btVector3 myUnitDirection;
    string baseMaterial;
    
    PlanetCell(){
        id = -1;
        owner = Owner_NEUTRAL;
        occupier = Owner_NEUTRAL;
        altitude = 0;
        longitude = 0;
        latitude = 0;
        terrain = Terrain_WATER;
        avg_color = 0;
        popDensity = 0;
        visible = false;
        selected = false;
        moving = false;
        building = false;
        capturing = false;
        unitbuilding = false;
        myUnit = 0;
        myUnit_pending = 0;
        myUnitId = -1;
        goalId = -1;
        timeNeeded = 0;
        growsCompleted = 0;
        baseMaterial = "MyMaterials/earth_day_bw";
    }
    
    PlanetCell(PlanetVertex& v){
        //id = v.id; //not a good idea
        centerId = v.id;
        owner = Owner_NEUTRAL;
        occupier = Owner_NEUTRAL;
        neighbors = v.neighbors;
        altitude = v.altitude;
        longitude = v.longitude;
        latitude = v.latitude;
        terrain = Terrain_WATER;
        avg_color = 0;
        popDensity = 0;
        visible = false;
        selected = false;
        moving = false;
        building = false;
        capturing = false;
        unitbuilding = false;
        myUnit = 0;
        myUnit_pending = 0;
        myUnitId = -1;
        goalId = -1;
        timeNeeded = 0;
        growsCompleted = 0;
        baseMaterial = "MyMaterials/earth_day_bw";
    }
    
    PlanetCell( const PlanetCell& c ){
        id = c.id; //critical
        centerId = c.centerId;
        owner = c.owner;
        occupier = Owner_NEUTRAL;
        neighbors = c.neighbors;
        paramVerts = c.paramVerts;
        altitude = c.altitude;
        longitude = c.longitude;
        latitude = c.latitude;
        faces = c.faces;
        terrain = c.terrain;
        avg_color = c.avg_color;
        popDensity = c.popDensity;
        visible = false;
        selected = false;
        moving = false;
        building = false;
        capturing = false;
        unitbuilding = false;
        myUnit = 0;
        myUnit_pending = 0;
        myUnitId = -1;
        goalId = -1;
        timeNeeded = 0;
        growsCompleted = 0;
        baseMaterial = "MyMaterials/earth_day_bw";
    }
        
    PlanetCell( PlanetCell& c ){
        id = c.id; //critical
        centerId = c.centerId;
        owner = c.owner;
        occupier = Owner_NEUTRAL;
        neighbors = c.neighbors;
        paramVerts = c.paramVerts;
        altitude = c.altitude;
        longitude = c.longitude;
        latitude = c.latitude;
        faces = c.faces;
        terrain = c.terrain;
        avg_color = c.avg_color;
        popDensity = c.popDensity;
        visible = false;
        selected = false;
        moving = false;
        building = false;
        capturing = false;
        unitbuilding = false;
        myUnit = 0;
        myUnit_pending = 0;
        myUnitId = -1;
        goalId = -1;
        timeNeeded = 0;
        growsCompleted = 0;
        baseMaterial = "MyMaterials/earth_day_bw";
    }
    
    PlanetCell& operator=( const PlanetCell& c ){
        id = c.id; //critical
        centerId = c.centerId;
        owner = c.owner;
        occupier = Owner_NEUTRAL;
        neighbors = c.neighbors;
        paramVerts = c.paramVerts;
        altitude = c.altitude;
        longitude = c.longitude;
        latitude = c.latitude;
        faces = c.faces;
        terrain = c.terrain;
        avg_color = c.avg_color;
        popDensity = c.popDensity;
        visible = false;
        selected = false;
        moving = false;
        building = false;
        capturing = false;
        unitbuilding = false;
        myUnit = 0;
        myUnit_pending = 0;
        myUnitId = -1;
        goalId = -1;
        timeNeeded = 0;
        growsCompleted = 0;
        baseMaterial = "MyMaterials/earth_day_bw";
    }
    
    void setOwner(size_t him){
        owner = him;
    }
};

class VertexList{
    public:
    vector<PlanetVertex> _vertices;
    size_t currentId;
    
    VertexList(){
        currentId = 0;
    }
    
    void add(PlanetVertex& v){
        v.id = currentId++; // DO NOT SWITCH THE ORDER OF THESE 2 LINES
        _vertices.push_back(v);
    }
    
    PlanetVertex& operator []( int i ){ 
        return _vertices[i];
    }
    
    PlanetVertex operator []( int i ) const { 
        return _vertices[i]; 
    }
    
    int size(){
        return _vertices.size();
    }
    
    void clear(){
        _vertices.clear();
        currentId = 0;
    }
    
};

class FaceList{
    public:
    vector<PlanetFace> _faces;
    size_t currentId;
    
    FaceList(){
        currentId = 0;
    }
    
    void add(PlanetFace& f){
        f.id = currentId++;
        _faces.push_back(f);
    }
    
    PlanetFace& operator []( int i ){ 
        return _faces[i];
    }
    
    PlanetFace operator []( int i ) const { 
        return _faces[i]; 
    }
    
    int size(){
        return _faces.size();
    }
    
    void clear(){
        _faces.clear();
        currentId = 0;
    }
};

class CellList{
    public:
    vector<PlanetCell> _cells;
    size_t currentId;
    
    CellList(){
        currentId = 0;
    }
    
    void add(PlanetCell& c){
        c.id = currentId++;
        _cells.push_back(c);
    }
    
    PlanetCell& operator []( int i ){ 
        return _cells[i];
    }
    
    PlanetCell operator []( int i ) const { 
        return _cells[i]; 
    }
    
    int size(){
        return _cells.size();
    }
    
    void clear(){
        _cells.clear();
        currentId = 0;
    }
};

class Planet {
    public:
    VertexList vertices;
    FaceList faces;
    CellList cells;
    FaceList smallFaces;
    
    btVector3 center;
    btVector3 axis;
    btVector3 longitude_zero; 
    float radius;
    int complexity;
    
    float spinAngle;
    
    Planet(){
        center = btVector3(0,0,0);
        axis = btVector3(0,1,0);
        longitude_zero = btVector3(0,0,1);
        radius = 6371;
        complexity = 3;
    }
    
    Planet(btVector3 c, btVector3 a, btVector3 lz, float r, int k){
        center = c;
        axis = a;
        longitude_zero = lz;
        radius = r;
        complexity = k;
    }
    
    void init();
    void subdivide (PlanetVertex& a, PlanetVertex& b, PlanetVertex& c, int _k);
    void mapVertex(PlanetVertex& v);
    void fixSeamHelper (PlanetFace& f);
    void fixSeam();
    void fixSeam_small();
    PlanetVertex midpointOnSphere (PlanetVertex& a, PlanetVertex& b);
    PlanetVertex midpointOnSphere (PlanetVertex& a, PlanetVertex& b, PlanetVertex& c);
    //void drawFace (PlanetFace& f);
    //void renderEarth();
    //void renderWireframe();
    void genCells();
    //void renderCellBoundary(PlanetCell& c);
    PlanetCell& getCellAt(float longitude, float latitude);
    PlanetCell& getCellAt( btVector3 surfPos );
    size_t getCellIdAt( btVector3 p );
    //void renderAxis();
    //void renderCells();
    bool rayHitPlanet( btVector3 p, btVector3 dir, btVector3 &result );
    void createManualObjects(Ogre::SceneManager* scnMgr);
    void createManualObjects2(Ogre::SceneManager* scnMgr);
    void mapTerrain();
    void mapFaction();
    void mapPopDensity();
    void changeMaterial(Ogre::SceneManager* scnMgr, PlanetCell& targetCell, string matName);
    void own(Ogre::SceneManager* scnMgr, PlanetCell& targetCell);
    void disown(Ogre::SceneManager* scnMgr, PlanetCell& targetCell);
    void createBorderSegments(Ogre::SceneManager* scnMgr, float k);
    void updateBorderSegments(Ogre::SceneManager* scnMgr);
};

#endif
