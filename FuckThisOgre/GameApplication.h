#pragma once
 
#include "GameFrameListener.h"
 
#include "OgreReferenceAppLayer.h"
 
#include "ExampleRefAppApplication.h"
#include "OgreStringConverter.h"
 
class GameApplication : public ExampleRefAppApplication
{
public:
    GameApplication() { }
    ~GameApplication() { }
 
protected:
	ApplicationObject *ball;
 
	void createScene(void)
    {
        mSceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_MODULATIVE);
 
	// Set ambient light
        mSceneMgr->setAmbientLight(ColourValue(0.2, 0.2, 0.2));
 
		// Create a point light
        Light* l = mSceneMgr->createLight("MainLight");
        l->setPosition(-100,50,100);
        l->setAttenuation(8000,1,0,0);
 
 
        // Setup World
        mWorld->setGravity(Vector3(0, 0, -60));
 
        // modify camera for close work
        mCamera->setNearClipDistance(10);
        mCamera->setFarClipDistance(20000);
 
        // Also change position, and set Quake-type orientation
        // Get random player start point
        ViewPoint vp = mSceneMgr->getSuggestedViewpoint(true);
        mCamera->setPosition(vp.position);
        mCamera->pitch(Degree(90)); // Quake uses X/Y horizon, Z up
        mCamera->rotate(vp.orientation);
 
        // Don't yaw along variable axis, causes leaning
        mCamera->setFixedYawAxis(true, Vector3::UNIT_Z);
 
	// Look at the boxes
	mCamera->lookAt(-150,40,30);
 
	// create ball
        ball = mWorld->createBall("ball", 7, vp.position + Vector3(0,0,80));
        ball->setDynamicsEnabled(true);
        ball->getEntity()->setMaterialName("Ogre/Eyes");
 
	// Create walls
	OgreRefApp::FinitePlane* walls[4];
 
	//Right
	walls[0] = mWorld->createPlane("Wall0",100,100,Vector3(-150,+100,0),Quaternion(Radian(Degree(90)),Vector3(1,0,0)));
        walls[0]->getEntity()->setMaterialName("Examples/Rocky");
 
	//Left
	walls[1] = mWorld->createPlane("Wall1",100,100,Vector3(-150,-100,0),Quaternion(Radian(Degree(-90)),Vector3(1,0,0)));
        walls[1]->getEntity()->setMaterialName("Examples/Rocky");
 
	//Back
	walls[2] = mWorld->createPlane("Wall2",100,100,Vector3(-250,0,0),Quaternion(Radian(Degree(90)),Vector3(0,1,0)));
        walls[2]->getEntity()->setMaterialName("Examples/Rocky");
 
	//bottom
	walls[3] = mWorld->createPlane("Wall3",100,100,Vector3(-150,0,-100),Quaternion(Radian(Degree(0)),Vector3(1,0,0)));
        walls[3]->getEntity()->setMaterialName("Examples/Rocky");
 
	// Shelf
	OgreRefApp::Box* box = mWorld->createBox("shelf", 75, 125, 5, Vector3(-150, 40, 30));
        box->getEntity()->setMaterialName("Examples/Rocky");
 
	// Cubes
        static const Real BOX_SIZE = 15.0f;
        static const int num_rows = 3;
 
        for (int row = 0; row < num_rows; ++row)
        {
            for (int i = 0; i < (num_rows-row); ++i)
            {
                Real row_size = (num_rows - row) * BOX_SIZE * 1.25;
                String name = "box";
                name += StringConverter::toString((row*num_rows) + i);
                box = mWorld->createBox(name, BOX_SIZE,BOX_SIZE,BOX_SIZE , 
                    Vector3(-150, 
                        40 - (row_size * 0.5) + (i * BOX_SIZE * 1.25) , 
                        32.5 + (BOX_SIZE / 2) + (row * BOX_SIZE)));
                box->setDynamicsEnabled(false, true);
                box->getEntity()->setMaterialName("Examples/10PointBlock");
            }
        }
 
        mCamera->setCollisionEnabled(false);
    }
 
    // Create new frame listener
    void createFrameListener(void)
    {
        mFrameListener= new GameFrameListener(mWindow, mCamera, ball);
        mRoot->addFrameListener(mFrameListener);
    }
};
