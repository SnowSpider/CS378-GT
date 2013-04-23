#include "Planet.h"
#include "imageloader.h"

using namespace Ogre;

void Planet::init(){
    // Clear the datastore
    vertices.clear();
    faces.clear();
    cells.clear();
    
    // Compute the 3D vertices of an icosahedron
    const float sqrt5 = sqrt (5.0f);
    const float phi = (1.0f + sqrt5) * 0.5f;
    const float cRadius = sqrt (10.0f + (2.0f * sqrt5)) / (4.0f * phi); // circumscribed radius
    double a = (radius / cRadius) * 0.5;
    double b = (radius / cRadius) / (2.0f * phi);
    
    PlanetVertex v0 ( 0,  b, -a);
    PlanetVertex v1 ( b,  a,  0); 
    PlanetVertex v2 (-b,  a,  0);
    PlanetVertex v3 ( 0,  b,  a);
    PlanetVertex v4 ( 0, -b,  a);
    PlanetVertex v5 (-a,  0,  b);
    PlanetVertex v6 ( 0, -b, -a);
    PlanetVertex v7 ( a,  0, -b);
    PlanetVertex v8 ( a,  0,  b);
    PlanetVertex v9 (-a,  0, -b);
    PlanetVertex v10( b, -a,  0);
    PlanetVertex v11(-b, -a,  0); 
    
    // define rotation axis
    btVector3 xAxis(1,0,0);
    btVector3 yAxis(0,1,0);
    btVector3 zAxis(0,0,1);
    
    // rotate everything
    float offset = atan(b/a);
    v0.rotate(zAxis, offset);
    v1.rotate(zAxis, offset);
    v2.rotate(zAxis, offset);
    v3.rotate(zAxis, offset);
    v4.rotate(zAxis, offset);
    v5.rotate(zAxis, offset);
    v6.rotate(zAxis, offset);
    v7.rotate(zAxis, offset);
    v8.rotate(zAxis, offset);
    v9.rotate(zAxis, offset);
    v10.rotate(zAxis, offset);
    v11.rotate(zAxis, offset);
    
    // Start subdivision
    subdivide (v0,  v1,  v2  , complexity);
    subdivide (v3,  v2,  v1  , complexity);
    subdivide (v3,  v4,  v5  , complexity);
    subdivide (v3,  v8,  v4  , complexity);
    subdivide (v0,  v6,  v7  , complexity);
    subdivide (v0,  v9,  v6  , complexity);
    subdivide (v4,  v10, v11 , complexity);
    subdivide (v6,  v11, v10 , complexity); 
    subdivide (v2,  v5,  v9  , complexity);
    subdivide (v11, v9,  v5  , complexity);
    subdivide (v1,  v7,  v8  , complexity);
    subdivide (v10, v8,  v7  , complexity);
    subdivide (v3,  v5,  v2  , complexity);
    subdivide (v3,  v1,  v8  , complexity);
    subdivide (v0,  v2,  v9  , complexity);
    subdivide (v0,  v7,  v1  , complexity);
    subdivide (v6,  v9,  v11 , complexity);
    subdivide (v6,  v10, v7  , complexity);
    subdivide (v4,  v11, v5  , complexity);
    subdivide (v4,  v8,  v10 , complexity);

    // Fix the seam.
    //fixSeam();
    
    // Create cells.
    genCells();
    
    // Debugging.
    cout << "number of vertex = " << vertices.size() << endl;
    cout << "number of face = " << faces.size() << endl;
    cout << "number of cell = " << cells.size() << endl;
    cout << "k = " << complexity << endl;
}

void Planet::subdivide(PlanetVertex& a, PlanetVertex& b, PlanetVertex& c, int k){
    if(k==0){
        //assign neighbors
        bool aNew = true;
        bool bNew = true;
        bool cNew = true;
        int aId, bId, cId;
        
        // OPT NOTE: This makes init() unbearably slow.
        for(int i=0;i<vertices.size();i++){ 
            PlanetVertex temp = vertices[i];
            if(temp.equals(a)){
                aNew = false;
                aId = temp.id;
            }
            if(temp.equals(b)){
                bNew = false;
                bId = temp.id;
            }
            if(temp.equals(c)){
                cNew = false;
                cId = temp.id;
            }
        }
        
        if(aNew){
            mapVertex(a);
            vertices.add(a);
            aId = vertices.currentId - 1;
        }
        if(bNew){
            mapVertex(b);
            vertices.add(b);
            bId = vertices.currentId - 1;
        }
        if(cNew){
            mapVertex(c);
            vertices.add(c);
            cId = vertices.currentId - 1;
        }
        
        // OPT NOTE: Ugly.
        vertices[aId].marry(vertices[bId]); 
        vertices[bId].marry(vertices[cId]);
        vertices[cId].marry(vertices[aId]);
        
        PlanetFace tempFace(vertices[aId], vertices[bId], vertices[cId]);
        faces.add(tempFace);
    }
    else{
        // Find edge midpoints.
        PlanetVertex ab = midpointOnSphere (a, b);
        PlanetVertex bc = midpointOnSphere (b, c);
        PlanetVertex ca = midpointOnSphere (c, a);
        
        // Create 4 subdivided triangles and recurse.
        subdivide ( a, ab, ca, k-1);
        subdivide (ab,  b, bc, k-1);
        subdivide (ca, bc,  c, k-1);
        subdivide (ab, bc, ca, k-1);
    }
}

void Planet::mapVertex(PlanetVertex& v){
    btVector3 w(v[0], 0.0, v[2]); 
    float latitude; 
    latitude = v.angle(axis)/PI; 
    float longitude; 
    if(v[0]>0) longitude = (2.0*PI - w.angle(longitude_zero))/PI/2.0; 
    else longitude = w.angle(longitude_zero)/PI/2.0; 
    if(!(longitude>=0&&longitude<=1)) longitude = 0.0; //longitude is -1.#IND00
    v.longitude = longitude;
    v.latitude = latitude;
}

void Planet::fixSeamHelper(PlanetFace& f){
    PlanetVertex& a = vertices[f.v[0]];
    PlanetVertex& b = vertices[f.v[1]];
    PlanetVertex& c = vertices[f.v[2]];
    
    float aLongitude = a.longitude;
    float bLongitude = b.longitude;
    float cLongitude = c.longitude;
    float aLatitude = a.latitude;
    float bLatitude = b.latitude;
    float cLatitude = c.latitude;
    
    // special cases for the poles
    // PlanetFaces needs bool touchsPole. need repair.
    
    /*
    if(aLongitude==0 && aLatitude==0){
        
    }
    else if(bLongitude==0 && bLatitude==0){
        
    }
    else if(cLongitude==0 && cLatitude==0){
        
    }
    */
    
    /*
    signed area = sum of 2-d cross product
    U x V = Ux*Vy-Uy*Vx
    http://howevangotburned.wordpress.com/2011/02/28/the-oddyssey-of-texturing-a-geodesic-dome/
    */
    
    // <
    // <=
    // <=
    // <=
    
    if( (cLongitude*bLatitude-cLatitude*bLongitude) +
        (bLongitude*aLatitude-bLatitude*aLongitude) +
        (aLongitude*cLatitude-aLatitude*cLongitude) > 0 ){ 
        // Signed area is negative
        
        if(c[0]==0){
            PlanetVertex newC(c);
            newC.longitude = 1;
            newC.positive = false;
            vertices.add(newC);
            f.v[2] = newC.id; //critical
        }
        if(b[0]==0){
            PlanetVertex newB(b);
            newB.longitude = 1;
            newB.positive = false;
            vertices.add(newB);
            f.v[1] = newB.id; //critical
        }
        if(a[0]==0){
            PlanetVertex newA(a);
            newA.longitude = 1;
            newA.positive = false;
            vertices.add(newA);
            f.v[0] = newA.id; //critical
        }
        f.positive = false;
    }
}

void Planet::fixSeam(){
    for(int i=0;i<faces.size();i++){
        fixSeamHelper( faces[i] );
    }
}

PlanetVertex Planet::midpointOnSphere (PlanetVertex& a, PlanetVertex& b){
    btVector3 midpoint = (a + b) * 0.5;
    btVector3 unitRadial = midpoint - center;
    unitRadial.normalize();
    PlanetVertex midPointOnSphere = center + (unitRadial * radius);
    return midPointOnSphere;
}

PlanetVertex Planet::midpointOnSphere (PlanetVertex& a, PlanetVertex& b, PlanetVertex& c){
    btVector3 midpoint = (a + b + c)/3.0;
    btVector3 unitRadial = midpoint - center;
    unitRadial.normalize();
    PlanetVertex midPointOnSphere = center + (unitRadial * radius);
    return midPointOnSphere;
}

/*
void Planet::drawFace(PlanetFace& f){
    PlanetVertex& a = vertices[f.v[0]]; 
    PlanetVertex& b = vertices[f.v[1]]; 
    PlanetVertex& c = vertices[f.v[2]]; 
    
    btVector3 triCenter = (a + b + c)/ 3.0f; // face center
    btVector3 triNormal = triCenter - center; // face normal
    
    //glLoadName(f.id);
    glBegin(GL_TRIANGLES);
        glNormal3d(triNormal[0], triNormal[1], triNormal[2]); //Normal for lighting
        glTexCoord2f(c.longitude, c.latitude);
        glVertex3d(c[0], c[1], c[2]); //Vertex c
        glTexCoord2f(b.longitude, b.latitude);
        glVertex3d(b[0], b[1], b[2]); //Vertex b
        glTexCoord2f(a.longitude, a.latitude);
        glVertex3d(a[0], a[1], a[2]); //Vertex a
    glEnd();
}
*/

/*
void Planet::renderEarth(){
    //glInitNames();
    //cout << "Rendering...";
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId_day);
    
    //blocky texture mapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    
    for(int i=0;i<faces.size();i++){
        drawFace( faces[i] );
    }
    glDisable(GL_TEXTURE_2D);
}
*/

/*
void Planet::renderWireframe(){
    glColor3f(1.0,0.0,0.0);
    for(int i=0;i<faces.size();i++){
        PlanetVertex& a = vertices[faces[i].v[0]];
        PlanetVertex& b = vertices[faces[i].v[1]];
        PlanetVertex& c = vertices[faces[i].v[2]];
        btVector3 triCenter = (a + b + c)/ 3.0f; 
        btVector3 triNormal = triCenter - center; 
        glBegin(GL_LINES);
            glNormal3d(triNormal[0], triNormal[1], triNormal[2]); 
            glVertex3d(c[0], c[1], c[2]); 
            glVertex3d(b[0], b[1], b[2]); 
            glVertex3d(a[0], a[1], a[2]); 
            glVertex3d(c[0], c[1], c[2]); 
        glEnd();
    }
}
*/

void Planet::genCells(){
    int len = vertices.size(); 
    for(int i=0;i<len;i++){
        if(vertices[i].neighbors.size()>0){
            //cout << "neighbors.size() = " << vertices[i].neighbors.size() << endl;
            PlanetCell tempCell(vertices[i]);
            std::vector<size_t> n = tempCell.neighbors;
            PlanetVertex newVert;
            for(int j=0;j<n.size();j++){
                PlanetVertex currentNeighbor = vertices[n[j]];
                newVert = midpointOnSphere(vertices[i], currentNeighbor);
                mapVertex(newVert); //necessary for the sorting part
                vertices.add(newVert); // NOT shared between cells
                tempCell.paramVerts.push_back(vertices.currentId - 1);
            }
            for(int j=0;j<faces.size();j++){
                if( vertices[faces[j].v[0]].equals(vertices[i]) || 
                    vertices[faces[j].v[1]].equals(vertices[i]) || 
                    vertices[faces[j].v[2]].equals(vertices[i]) ){
                    //newVert = faces[j].center; //this variable is not updated
                    newVert = midpointOnSphere( vertices[faces[j].v[0]], 
                                                vertices[faces[j].v[1]], 
                                                vertices[faces[j].v[2]] );
                    mapVertex(newVert);
                    vertices.add(newVert); //necessary for the sorting part
                    tempCell.paramVerts.push_back(vertices.currentId - 1);
                }
            }
            // Sort the paramVerts.
            std::vector<size_t> x = tempCell.paramVerts;
            std::vector<size_t> paramVerts_sorted;
            size_t tempId;
            int index = 0;
            float min;
            while(x.size()>0){
                tempId = x[index];
                x.erase(x.begin()+index);
                paramVerts_sorted.push_back(tempId);
                min = radius; //critical
                for(int j=0;j<x.size();j++){
                    float edgeLength = (vertices[tempId] - vertices[x[j]]).length();
                    if(edgeLength < min){
                        min = edgeLength;
                        index = j;
                    }
                }
            }
            //At this point, paramVerts may have been sorted clockwise OR counterclockwise.
            /*
            signed area = sum of 2-d cross product
            U x V = Ux*Vy-Uy*Vx
            */
            
            // Sort the paramVerts again to make them run in the same direction: counterclockwise.
            // We need to check the 3-d cross product between 2 paramVert.
            
            bool facingIn = false;
            
            btVector3 vi = vertices[i];
            btVector3 p0 = vertices[paramVerts_sorted[0]];
            btVector3 p1 = vertices[paramVerts_sorted[1]];
            btScalar t0 = vi.angle(p0);
            btScalar t1 = vi.angle(p1);
            btScalar len_p0 = p0.length();
            btScalar len_p1 = p1.length();
            btScalar len_x0 = len_p0 * cos(t0);
            btScalar len_x1 = len_p1 * cos(t1);
            btVector3 x0 = vi.normalized() * len_x0;
            btVector3 x1 = vi.normalized() * len_x1;
            btVector3 y0 = p0 - x0;
            btVector3 y1 = p1 - x1;
            
            btVector3 u = y0;
            btVector3 v = y1;
            btVector3 uxv = u.cross(v);
            uxv.normalize();
            vi.normalize();
            //cout << "uxv = (" << uxv.x() << "," << uxv.y() << "," << uxv.z() << ")" << endl;
            //cout << "vi = (" << vi.x() << "," << vi.y() << "," << vi.z() << ")\n" << endl;
            
            btScalar theta = vi.angle(uxv);
            
            if(theta > PI/2.0){
                facingIn = true;
                //cout << "A tile is facing in!" << endl;
            }
            
            if (facingIn){
                std::reverse(paramVerts_sorted.begin(), paramVerts_sorted.end());
            }
            
            std::vector<size_t> p = paramVerts_sorted;
            PlanetVertex& a = vertices[i];
            PlanetFace tempFace;
            
            for(int j=0;j<p.size();j++){
                PlanetVertex& b = vertices[p[j]];
                if(j+1==p.size()){
                    tempFace = PlanetFace(a, b, vertices[p[0]]);
                }
                else{
                    tempFace = PlanetFace(a, b, vertices[p[j+1]]);
                }
                smallFaces.add(tempFace);
                tempFace.parentCellId = tempCell.id;
                tempCell.faces.push_back(tempFace.id);
            }
            
            // Last touch
            tempCell.paramVerts = paramVerts_sorted;
            cells.add(tempCell);
        }
    }
}

/*
void Planet::renderCellBoundary(PlanetCell& c){
    std::vector<size_t> p = c.paramVerts;
    btVector3 triCenter = vertices[c.centerId];
    btVector3 triNormal = triCenter - center;
    for(int j=0;j<p.size();j++){
        glBegin(GL_LINES);
        glNormal3d(triNormal[0], triNormal[1], triNormal[2]);
        glVertex3d(vertices[p[j]][0], vertices[p[j]][1], vertices[p[j]][2]);
        if(j+1==p.size()){
            glVertex3d(vertices[p[0]][0], vertices[p[0]][1], vertices[p[0]][2]);
        }
        else{
            glVertex3d(vertices[p[j+1]][0], vertices[p[j+1]][1], vertices[p[j+1]][2]);
        }
        glEnd();
    }
}
*/

PlanetCell& Planet::getCellAt(float longitude, float latitude){ //input in degrees
    /*
    First, the point normalized to a direction vector. 
    The angle between the point and the hex center can be found by 
    
    acos( (n dot hexpos ) / planetRadius ). 
    
    Whichever hex has the smallest such angle contains the point. 
    In my demo, checking every hex was fast enough even on a large subdivision level, 
    for a game you would probably want to add an acceleration structure 
    (perhaps as a 2D kd-tree in spherical coordinates).
    */
    longitude = longitude * PI / 180.0;
    latitude = latitude * PI / 180.0;
    
    btVector3 targetRadial( center[0] + (radius * cos(longitude) * sin(latitude)),
                       center[0] + (radius * sin(longitude) * sin(latitude)), 
                       center[0] + (radius * cos(latitude)));
    float min = PI;
    float temp;
    int tempId = -1;
    for(int i=0;i<cells.size();i++){
        temp = targetRadial.angle(vertices[cells[i].centerId]);
        if(temp < min){
            min = temp;
            tempId = i;
        }
    }
    return cells[tempId];
}

PlanetCell& Planet::getCellAt( btVector3 p ){
    btVector3 targetRadial = p - center;
    float minAngle = 2 * PI;
    float currentAngle;
    int bestCell;
    for(int i=0;i<cells.size();i++){
        currentAngle = targetRadial.angle(vertices[cells[i].centerId]);
        if(currentAngle < minAngle){
            minAngle = currentAngle;
            bestCell = i;
        }
    }
    return cells[bestCell];
}

size_t Planet::getCellIdAt( btVector3 p ){
    // clever cheat -- just use the dot product to find the 
    // smallest angle -- and thus the containing hex
    btVector3 targetRadial = p - center;
    float minAngle = 2 * PI;
    float currentAngle;
    int bestCell;
    for(int i=0;i<cells.size();i++){
        currentAngle = targetRadial.angle(vertices[cells[i].centerId]);
        if(currentAngle < minAngle){
            minAngle = currentAngle;
            bestCell = i;
        }
    }
    return bestCell;
}

/*
void Planet::renderAxis(){
    glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
    
    glColor3f(1.0,0.0,0.0);
    glVertex3f(center[0],center[1],center[2]);
    glVertex3f(center[0]+1.0,center[1],center[2]);
    
    glColor3f(0.0,1.0,0.0);
    glVertex3f(center[0],center[1],center[2]);
    glVertex3f(center[0],center[1]+1.0,center[2]);
    
    glColor3f(0.0,0.0,1.0);
    glVertex3f(center[0],center[1],center[2]);
    glVertex3f(center[0],center[1],center[2]+1.0);
    
    glEnd();
    glEnable(GL_LIGHTING);
}
*/

/*
void Planet::renderCells(){
    glColor3f(1.0,1.0,1.0);
    for(int i=0;i<cells.size();i++){
        renderCellBoundary(cells[i]);
    }
}
*/

bool Planet::rayHitPlanet( btVector3 p, btVector3 dir, btVector3 &result ){
    float a,b,c,d;
    a = dir.dot(dir);
    b = (2.0f*dir).dot(p);
    c = p.dot(p) - (radius*radius);
    d = b*b - 4.0f*a*c;
    if (d <=0 ) return false;
    result = p + ((-b - sqrt(d)) / 2.0f*a)*dir;
    return true;
}

void Planet::createManualObjects(Ogre::SceneManager* scnMgr){
    
    for(int i=0;i<smallFaces.size();i++){
        fixSeamHelper( smallFaces[i] );
    }
    
    for (int i=0; i<cells.size(); i++){
        string s = "Cell_";
        stringstream ss;
        Ogre::String name_manobj;
        ss << i;
        name_manobj = s.append(ss.str()); // Be careful not to append indefinitely.
        
        ManualObject* manual = scnMgr->createManualObject(name_manobj);
        manual->begin("MyMaterials/earth_day", RenderOperation::OT_TRIANGLE_LIST);
        //manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
        
        std::vector<size_t> p = cells[i].paramVerts;
        PlanetVertex& triCenter = vertices[cells[i].centerId];
        //btVector3 triNormal = triCenter - center;
        
        int mvc = 0; // mesh vertex counter
        
        for (int j=0; j<cells[i].faces.size(); j++){
            PlanetFace& tempFace = smallFaces[cells[i].faces[j]];
            PlanetVertex& a = vertices[tempFace.v[0]]; 
            PlanetVertex& b = vertices[tempFace.v[1]]; 
            PlanetVertex& c = vertices[tempFace.v[2]]; 
            
            manual->position( a[0], a[1], a[2] );
            manual->normal( a[0], a[1], a[2] );
            manual->textureCoord( 1-a.longitude, a.latitude );
            //if(cells[i].terrain==2) manual->colour( 1, 0, 0);
            //if(cells[i].terrain==1) manual->colour( 1, 1, 0);
            //if(cells[i].terrain==0) manual->colour( 0, 0, 1);
            mvc++;
            
            manual->position( b[0], b[1], b[2] );
            manual->normal( b[0], b[1], b[2] );
            manual->textureCoord( 1-b.longitude, b.latitude );
            //if(cells[i].terrain==2) manual->colour( 1, 0, 0);
            //if(cells[i].terrain==1) manual->colour( 1, 1, 0);
            //if(cells[i].terrain==0) manual->colour( 0, 0, 1);
            mvc++;
            
            manual->position( c[0], c[1], c[2] );
            manual->normal( c[0], c[1], c[2] );
            manual->textureCoord( 1-c.longitude, c.latitude );
            //if(cells[i].terrain==2) manual->colour( 1, 0, 0);
            //if(cells[i].terrain==1) manual->colour( 1, 1, 0);
            //if(cells[i].terrain==0) manual->colour( 0, 0, 1);
            mvc++;
        }
        
        for (int j=0; j<mvc; j++){
            manual->index(j);
        }
        
        manual->end();
        manual->begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_LIST);
        // cell boundary
        btVector3 temp;
        for(int j=0;j<p.size();j++){
            temp = vertices[p[j]] * 1.001;
            manual->position( temp.x(), temp.y(), temp.z() );
            manual->normal( temp.x(), temp.y(), temp.z() );
            //manual->colour( 0.5, 0.5, 0.5 );
            if(cells[i].terrain==2) manual->colour( 1, 0, 0);
            if(cells[i].terrain==1) manual->colour( 1, 1, 0);
            if(cells[i].terrain==0) manual->colour( 0, 0, 1);
            if(j+1==p.size()){
                temp = vertices[p[0]] * 1.001;
                manual->position( temp.x(), temp.y(), temp.z() );
                manual->normal( temp.x(), temp.y(), temp.z() );
                //manual->colour( 0.5, 0.5, 0.5 );
                if(cells[i].terrain==2) manual->colour( 1, 0, 0);
                if(cells[i].terrain==1) manual->colour( 1, 1, 0);
                if(cells[i].terrain==0) manual->colour( 0, 0, 1);
            }
            else{
                temp = vertices[p[j+1]] * 1.001;
                manual->position( temp.x(), temp.y(), temp.z() );
                manual->normal( temp.x(), temp.y(), temp.z() );
                //manual->colour( 0.5, 0.5, 0.5 );
                if(cells[i].terrain==2) manual->colour( 1, 0, 0);
                if(cells[i].terrain==1) manual->colour( 1, 1, 0);
                if(cells[i].terrain==0) manual->colour( 0, 0, 1);
            }
        }
        
        manual->end();
        manual->setQueryFlags(i);
        Ogre::SceneNode* cellNode = scnMgr->getRootSceneNode()->createChildSceneNode(name_manobj);
        cellNode->attachObject(manual);
        //cout << "Cell " << i << " created" << endl;
    }
    
    btVector3 one(1,0,0);
    btVector3 two(0,1,0);
    float three = one.angle(two);
    cout << "angle = " << three << endl; // radian!
    /*
    for (int i=0; i<cells.size(); i++){
        string s = "Edge_";
        stringstream ss;
        Ogre::String name_manobj;
        ss << i;
        name_manobj = s.append(ss.str()); // Be careful not to append indefinitely.
        
        ManualObject* manual = scnMgr->createManualObject(name_manobj);
        manual->begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_LIST);
        
        std::vector<size_t> p = cells[i].paramVerts;
        PlanetVertex& triCenter = vertices[cells[i].centerId];
        //btVector3 triNormal = triCenter - center;
        
        int mvc = 0; // mesh vertex counter
        
        btVector3 temp;
        
        // cell boundary
        for(int j=0;j<p.size();j++){
            temp = vertices[p[j]] * 1.001;
            manual->position( temp.x(), temp.y(), temp.z() );
            manual->normal( temp.x(), temp.y(), temp.z() );
            manual->colour( 0.5, 0.5, 0.5 );
            if(j+1==p.size()){
                temp = vertices[p[0]] * 1.001;
                manual->position( temp.x(), temp.y(), temp.z() );
                manual->normal( temp.x(), temp.y(), temp.z() );
                manual->colour( 0.5, 0.5, 0.5 );
            }
            else{
                temp = vertices[p[j+1]] * 1.001;
                manual->position( temp.x(), temp.y(), temp.z() );
                manual->normal( temp.x(), temp.y(), temp.z() );
                manual->colour( 0.5, 0.5, 0.5 );
            }
        }
        
        manual->end();
        manual->setQueryFlags(i);
        Ogre::SceneNode* cellNode = scnMgr->getRootSceneNode()->createChildSceneNode(name_manobj);
        cellNode->attachObject(manual);
    }
    */
}

bool inside(int x0, int y0, int x1, int y1, int x, int y){
    float a = y0 - y1;
    float b = x1 - x0;
    float c = (a*(x0+x1)) + (b*(y0+y1));
    c /= (-2.0);
    if ((a*x)+(b*y)+c > 0) return true;
    else return false;
}


void Planet::mapTerrain(){
    MyImage* image = loadBMP("media/materials/textures/earth_spec.bmp");
    
    int numLand = 0;
    int numWaterLand = 0;
    int numWater = 0;
    
    int x0, x1, x2;
    int y0, y1, y2;
    int top, bottom, left, right;
    float numPixelPerCell;
    unsigned char color;
    float fcolor;
    float sum_color;
    
    for(int i=0;i<cells.size();i++){
        sum_color = 0;
        numPixelPerCell = 0;
        for (int j=0; j<cells[i].faces.size(); j++){
            PlanetFace& tempFace = smallFaces[cells[i].faces[j]];
            PlanetVertex& a = vertices[tempFace.v[0]]; 
            PlanetVertex& b = vertices[tempFace.v[1]]; 
            PlanetVertex& c = vertices[tempFace.v[2]]; 
            
            x0 = -(image->width * a.longitude);
            y0 = -(image->height * a.latitude);
            x1 = -(image->width * b.longitude);
            y1 = -(image->height * b.latitude);
            x2 = -(image->width * c.longitude);
            y2 = -(image->height * c.latitude);
            
            top = max(y0, max(y1,y2));
            bottom = min(y0, min(y1,y2));
            left = min(x0, min(x1,x2));
            right = max(x0, max(x1,x2));
            
            for(int y=bottom; y<top; y++){
                for(int x=left; x<right; x++){
                    
                    if( inside(x0, y0, x1, y1, x, y) &&
                        inside(x1, y1, x2, y2, x, y) && 
                        inside(x2, y2, x0, y0, x, y) ){
                        color = (unsigned char)image->pixels[3 * (y * image->width + x)];
                        fcolor = color;
                        sum_color += (float)fcolor;
                        numPixelPerCell += 1;
                    } 
                }
            }
        }
        sum_color /= numPixelPerCell;
        
        if (sum_color <= 25){
            cells[i].terrain = Terrain_LAND;
            numLand++;
        }
        else if (sum_color > 25 && sum_color <= 250){
            cells[i].terrain = Terrain_WATERLAND;
            numWaterLand++;
        }
        else if (sum_color > 250){
            numWater++;
        }
        cells[i].avg_color = sum_color;
    }
    delete image;
    cout << "numLand" << numLand << endl;
    cout << "numWaterLand" << numWaterLand << endl;
    cout << "numWater" << numWater << endl;
}



