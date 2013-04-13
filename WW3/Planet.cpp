#include "Planet.h"

bool Planet::m_initStaticRes = false;
int Planet::g_texTemplate;
int Planet::g_texTileset;
int Planet::g_texTilesetGrid;

float Planet::m_radius = 6371.0f;

Cell::Cell( btVector3 p ) : m_vertPos( p ) {
	m_terrain = Cell::Terrain_DESERT;
	m_nrm = p.normalized();
}

Face::Face( size_t a, size_t b, size_t c) : m_cellA(a), m_cellB(b), m_cellC(c) {
	m_nbAB = NULL;
	m_nbBC = NULL;
	m_nbCA = NULL;
	
	// Mark newvert as uninitialized
	m_tmp.m_newvert = std::string::npos;
}

btVector3 Face::getCenter( const std::vector<Cell> &cells ) {
	btVector3 ret = cells[ m_cellA ].m_vertPos;
	ret += cells[ m_cellB ].m_vertPos;
	ret += cells[ m_cellC ].m_vertPos;
	ret /= 3.0f;
	return ret;
}

Planet::Planet( int subd_level ) : m_subdLevel(0) {
	// build initial (level 0) mesh
	buildLevel0();

	// subdivide until desired level
	while (m_subdLevel < subd_level) {
		subdivide();
	}

	// planetize if we're at level 0
	if (subd_level == 0) {
		projectToSphere();
	}
}

//=============================
// buildLevel0 -- builds the initial icosahedron
// for the planet mesh
//=============================
void Planet::buildLevel0()
{
	// hard code an icosahedron (20 sided die)
	m_cells.erase( m_cells.begin(), m_cells.end() );	
	m_cells.push_back( Cell( btVector3( 0.723606f, 0.0f, 1.17082f )));
	m_cells.push_back( Cell( btVector3( 0.0f, 1.17082f, 0.723606f )));
	m_cells.push_back( Cell( btVector3( -0.723606f, 0.0f, 1.17082f )));
	m_cells.push_back( Cell( btVector3( 0.0f, -1.17082f,  0.723606f )));
	m_cells.push_back( Cell( btVector3( 0.723606f, 0.0f, -1.17082f )));
	m_cells.push_back( Cell( btVector3( 0.0f, -1.17082f, -0.723606f )));
	m_cells.push_back( Cell( btVector3( -0.723606f, 0.0f, -1.17082f )));
	m_cells.push_back( Cell( btVector3( 0.0f, 1.17082f, -0.723606f )));
	m_cells.push_back( Cell( btVector3( 1.17082f, -0.723606f, 0.0f )));
	m_cells.push_back( Cell( btVector3( 1.17082f, 0.723606f, 0.0f )));
	m_cells.push_back( Cell( btVector3( -1.17082f, 0.723606f, 0.0f )));
	m_cells.push_back( Cell( btVector3( -1.17082f, -0.723606f,  0.0f )));

	m_hexdual.push_back( Face( 5, 11, 6 ));
	m_hexdual.push_back( Face( 1, 2, 0 ));
	m_hexdual.push_back( Face( 0, 2, 3 ));
	m_hexdual.push_back( Face( 5, 6, 4 ));
	m_hexdual.push_back( Face( 4, 6, 7 ));
	m_hexdual.push_back( Face( 9, 1, 0 ));
	m_hexdual.push_back( Face( 10, 2, 1 ));
	m_hexdual.push_back( Face( 2, 10, 11 ));
	m_hexdual.push_back( Face( 11, 3, 2 ));
	m_hexdual.push_back( Face( 8, 9, 0 ));
	m_hexdual.push_back( Face( 0, 3, 8 ));
	m_hexdual.push_back( Face( 11, 10, 6 ));
	m_hexdual.push_back( Face( 4, 7, 9 ));
	m_hexdual.push_back( Face( 9, 8, 4 ));
	m_hexdual.push_back( Face( 7, 6, 10 ));
	m_hexdual.push_back( Face( 1, 9, 7 ));
	m_hexdual.push_back( Face( 10, 1, 7 ));
	m_hexdual.push_back( Face( 5, 4, 8 ));
	m_hexdual.push_back( Face( 3, 11, 5 ));
	m_hexdual.push_back( Face( 8, 3, 5 ));	

	// make planet sized
	// projectToSphere();

	// assign neighbors
	findNeighbors();
}

// Used in subdivide
void createTrisFromEdge( std::vector< std::pair< size_t, size_t > > &edgeDone,
                         std::vector<Face> &trilist,
                         Face &tri, Face &otherTri,
                         size_t eA, size_t eB ) {
	std::pair<size_t, size_t> eid( std::min( eA, eB ), std::max( eA, eB ) );
	if (std::find( edgeDone.begin(), edgeDone.end(), eid ) == edgeDone.end() ) {
		trilist.push_back( Face( eA, tri.m_tmp.m_newvert, otherTri.m_tmp.m_newvert ) );					
		trilist.push_back( Face( tri.m_tmp.m_newvert, otherTri.m_tmp.m_newvert, eB ) );
		edgeDone.push_back( eid );
	}
}

//=============================
// subdivide()
// Perform sqrt(3) subdivision on
// the mesh
//=============================
void Planet::subdivide() {
	// Subdivide by creating two triangles in 
	// the next level mesh for every edge in the
	// src mesh. Keep track of which edges have
	// already been handled
	std::vector< std::pair< size_t, size_t > > edgeDone;

	// The new mesh that will be created	
	std::vector<Face> newHexdual;

	// Go through each triangle in the old mesh and create
	// a new vert at the center of each one
	for (std::vector<Face>::iterator ti = m_hexdual.begin();
		 ti != m_hexdual.end(); ti++ ) {
		// Create a new vert at the center of the triangle
		btVector3 pNewVert;
		(*ti).m_tmp.m_newvert = m_cells.size();
		pNewVert = (*ti).getCenter( m_cells );		

		// add it to the list of cells
		m_cells.push_back( Cell( pNewVert ) );
	}

	// Go through each triangle in the old mesh and create
	// a new pair of triangles for each edge
	for (std::vector<Face>::iterator ti = m_hexdual.begin();
		 ti != m_hexdual.end(); ti++ ) {
		Face &t = (*ti);

		// Create a pair for edge AB
		createTrisFromEdge( edgeDone, newHexdual, t, *(t.m_nbAB), t.m_cellA, t.m_cellB );

		// Create a pair for edge BC
		createTrisFromEdge( edgeDone, newHexdual, t, *(t.m_nbBC), t.m_cellB, t.m_cellC );
		
		// Create a pair for edge CA
		createTrisFromEdge( edgeDone, newHexdual, t, *(t.m_nbCA), t.m_cellC, t.m_cellA );
		
	}

	// replace the current set of cells with the dual
	m_hexdual = newHexdual;

	// find new neighbors
	findNeighbors();

	// reproject back to sphere
	projectToSphere();

	// note the subdivision
	m_subdLevel++;
}


//=============================
// findNeighbors() -- it would be more
// efficent to just keep track of the neighbors
// during subdivision, but this is easier
//=============================
bool edgeMatch( size_t a, size_t b,
				size_t otherA, size_t otherB, size_t otherC ) {
	if ( ((a==otherA) && (b==otherB)) ||
		 ((a==otherB) && (b==otherC)) ||
		 ((a==otherC) && (b==otherA)) ||
		 ((b==otherA) && (a==otherB)) ||
		 ((b==otherB) && (a==otherC)) ||
		 ((b==otherC) && (a==otherA)) ) return true;
	return false;
}

bool _cmpAngle( Face *a, Face *b ) {
	return a->m_tmp.m_angle < b->m_tmp.m_angle;
}

void Planet::findNeighbors() {
	// Clear the Face list on the cells
	for (std::vector<Cell>::iterator hi = m_cells.begin();
		hi != m_cells.end(); hi++ ) {
		(*hi).m_faces.erase( (*hi).m_faces.begin(), (*hi).m_faces.end() );
	}

	// Find edge adjacentcy -- slow and brute force. Should
	// do this as part of the subdivide step if this were a
	// non-prototype implementation.
	for (std::vector<Face>::iterator ti = m_hexdual.begin();
		ti != m_hexdual.end(); ti++) {
		// find the neighbors for ti
		for (std::vector<Face>::iterator tj = m_hexdual.begin();
			tj != m_hexdual.end(); tj++) {

			// Don't match ourselves
			if (ti==tj) continue;

			// Neighbor across edge AB
			if ( edgeMatch( (*ti).m_cellA, (*ti).m_cellB,
							(*tj).m_cellA, (*tj).m_cellB, (*tj).m_cellC ) ) {
				(*ti).m_nbAB = &(*tj);
			}

			// Neighbor across edge BC
			if ( edgeMatch( (*ti).m_cellB, (*ti).m_cellC,
							(*tj).m_cellA, (*tj).m_cellB, (*tj).m_cellC ) ) {
				(*ti).m_nbBC = &(*tj);
			}

			// Neighbor across edge CA
			if ( edgeMatch( (*ti).m_cellC, (*ti).m_cellA,
							(*tj).m_cellA, (*tj).m_cellB, (*tj).m_cellC ) ) {
				(*ti).m_nbCA = &(*tj);
			}
		}


		// Also as part of findNeighbors, set up the Face pointers
		m_cells[(*ti).m_cellA].m_faces.push_back( &(*ti) );
		m_cells[(*ti).m_cellB].m_faces.push_back( &(*ti) );
		m_cells[(*ti).m_cellC].m_faces.push_back( &(*ti) );
	}

	// Now sort the Face list on the cells by the angle
	// around the hex center
	for (std::vector<Cell>::iterator hi = m_cells.begin();
		hi != m_cells.end(); hi++ ) {
		// assign angles 
		for (std::vector<Face*>::iterator ti = (*hi).m_faces.begin();
			  ti != (*hi).m_faces.end(); ti++ ) {
			// arbitrarily use the first one as starting angle
			// it doesn't matter		
			btVector3 v1 = (*hi).m_faces.back()->getCenter( m_cells ) - (*hi).m_vertPos;			
			btVector3 nrm = (*ti)->getCenter( m_cells );
			btVector3 v2 = nrm - (*hi).m_vertPos;			
			nrm.normalize();
			v1.normalize();
			v2.normalize();

			float ang = acos( v1.dot(v2) );
			float dir = nrm.dot( v1.cross(v2) );
			if (dir < 0.0f) ang = M_PI + (M_PI - ang);

			(*ti)->m_tmp.m_angle = ang;
		}

		// Sort them
		std::sort( (*hi).m_faces.begin(), (*hi).m_faces.end(), _cmpAngle );
	}
	
}

//=============================
// projectToSphere()
//=============================
void Planet::projectToSphere() {
	for (std::vector<Cell>::iterator ti = m_cells.begin();
		 ti != m_cells.end(); ti++ ) {
		btVector3 p = (*ti).m_vertPos;
		p.normalize();
		p *= m_radius;
		(*ti).m_vertPos = p;
	}
}

size_t Planet::getNumCells() {
	return m_cells.size();
}

size_t Planet::getHexIndexFromPoint( btVector3 surfPos ) {
	size_t best_hex = 0;
	float best_dot;

	// normalize
	btVector3 p = surfPos;
	p.normalize();
	best_dot = acos( (m_cells[0].m_vertPos.dot(p) )  / m_radius );


	// clever cheat -- just use the dot product to find the 
	// smallest angle -- and thus the containing hex
	for (size_t ndx = 1; ndx < m_cells.size(); ndx++) {
		float d = acos( ( m_cells[ndx].m_vertPos.dot(p) ) / m_radius );
		if (d < best_dot) {
			best_hex = ndx;
			best_dot = d;
		}
	}

	return best_hex;
}

// returns the polygon representation of this
// hex. Usually 6-sided but could be a pentagon	
void Planet::getPolygon( Cell &tile, std::vector<btVector3> &poly, float offset ) {
	// clear list
	poly.erase( poly.begin(), poly.end() );

	// construct polygon
	for ( std::vector<Face*>::iterator ti = tile.m_faces.begin();
		  ti != tile.m_faces.end(); ti++ ) {
		btVector3 p = (*ti)->getCenter( m_cells );
		p.normalize();
		p *= m_radius + offset;
		poly.push_back( p );
	}
}

// returns the indices of the neighbors of this tile
// Usually 6, could be 5
void Planet::getNeighbors( size_t tileNdx, std::vector<size_t> &nbrs ) {
	// clear list
	nbrs.erase( nbrs.begin(), nbrs.end() );

	// find neighbors
	for ( std::vector<Face*>::iterator ti = m_cells[tileNdx].m_faces.begin();
		  ti != m_cells[tileNdx].m_faces.end(); ti++ ) {
		// Check all all the cells on neiboring 
		// Facees (except ourself), checking for dups

		// HEX A
		if ( ( ((*ti)->m_cellA) != tileNdx ) &&
			 (find( nbrs.begin(), nbrs.end(), ((*ti)->m_cellA) ) == nbrs.end() ) ) {
			nbrs.push_back( ((*ti)->m_cellA) );
		}

		// HEX B
		if ( ( ((*ti)->m_cellB) != tileNdx ) &&
			 (find( nbrs.begin(), nbrs.end(), ((*ti)->m_cellB) ) == nbrs.end() ) ) {
			nbrs.push_back( ((*ti)->m_cellB) );
		}
		
		// HEX C
		if ( ( ((*ti)->m_cellC) != tileNdx ) &&
			 (find( nbrs.begin(), nbrs.end(), ((*ti)->m_cellC) ) == nbrs.end() ) ) {
			nbrs.push_back( ((*ti)->m_cellC) );
		}
	}
}

// Returns a point on the planet's surface given a ray
bool Planet::rayHitPlanet( btVector3 p, btVector3 dir, btVector3 &result ) {
	float a,b,c,d;
	a = dir.dot(dir);
	b = (2.0f*dir).dot(p);
	c = (p.dot(p)) - (m_radius*m_radius);
	d = b*b - 4.0f*a*c;
	if (d <=0 ) return false;
	result = p + ((-b - sqrt(d)) / 2.0f*a)*dir;
	return true;
}

void Planet::createOgreMesh(string name){
    /* create the mesh and a single sub mesh */
    Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().createManual("CustomMesh", "General");
    Ogre::SubMesh *subMesh = mesh->createSubMesh();

    /* create the vertex data structure */
    mesh->sharedVertexData = new Ogre::VertexData;
    mesh->sharedVertexData->vertexCount = 3;

    /* declare how the vertices will be represented */
    Ogre::VertexDeclaration *decl = mesh->sharedVertexData->vertexDeclaration;
    size_t offset = 0;

    /* the first three floats of each vertex represent the position */
    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    /* the second three floats of each vertex represent the colour */
    decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_COLOUR);
    offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);

    /* create the vertex buffer */
    Ogre::HardwareBuffer *vertexBuffer = Ogre::HardwareBufferManager::getSingleton().
        createVertexBuffer(offset, mesh->sharedVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC);

    /* lock the buffer so we can get exclusive access to its data */
    float *vertices = static_cast<float *>(vertexBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));

    /* populate the buffer with some data */
    vertices[0] = 0; vertices[1] = 1; vertices[2] = 0; /* position */
    vertices[3] = 1; vertices[4] = 0; vertices[5] = 0; /* colour */

    vertices[6] = -1; vertices[7] = -1; vertices[8] = 0; /* position */
    vertices[9] = 0; vertices[10] = 1; vertices[11] = 0; /* colour */

    vertices[12] = 1; vertices[13] = -1; vertices[14] = 0; /* position */
    vertices[15] = 0; vertices[16] = 0; vertices[17] = 1; /* colour */

    /* unlock the buffer */
    vertexBuffer->unlock();

    /* create the index buffer */
    Ogre::HardwareBuffer *indexBuffer = Ogre::HardwareBufferManager::getSingleton().
        createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, mesh->sharedVertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC);

    /* lock the buffer so we can get exclusive access to its data */
    uint16_t *indices = static_cast<uint16_t *>(indexBuffer->lock(Ogre::HardwareBuffer::HBL_NORMAL));

    /* define our triangle */
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;

    /* unlock the buffer */
    indexBuffer->unlock();

    /* attach the buffers to the mesh */
    mesh->sharedVertexData->vertexBufferBinding->setBinding(0, vertexBuffer);
    subMesh->useSharedVertices = true;
    subMesh->indexData->indexBuffer = indexBuffer;
    subMesh->indexData->indexCount = mesh->sharedVertexData->vertexCount;
    subMesh->indexData->indexStart = 0;

    /* set the bounds of the mesh */
    mesh->_setBounds(Ogre::AxisAlignedBox(-1, -1, -1, 1, 1, 1));

    /* notify the mesh that we're all ready */
    mesh->load();

    /* you can now create an entity/scene node based on your mesh, e.g. */
    Ogre::Entity *entity = sceneManager->createEntity("CustomEntity", "CustomMesh", "General");
    entity->setMaterialName("YourMaterial", "General");
    Ogre::SceneNode *node = rootNode->createChildSceneNode();
    node->attachObject(entity);
}

