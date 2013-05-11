#include "Ballin.h"
 
//-------------------------------------------------------------------------------------
Ballin::Ballin(void)
{
}
//-------------------------------------------------------------------------------------
Ballin::~Ballin(void)
{
}
 
//-------------------------------------------------------------------------------------
void Ballin::createScene(void)
{
    // Set the default lighting.
    mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
    mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

    Ogre::Plane plane1(Ogre::Vector3::UNIT_Y, 375);
    Ogre::Plane plane2(Ogre::Vector3::NEGATIVE_UNIT_Y, -375);
    Ogre::Plane plane1b(Ogre::Vector3::UNIT_Y, -375);
    Ogre::Plane plane2b(Ogre::Vector3::NEGATIVE_UNIT_Y, 375);
    Ogre::Plane plane3(Ogre::Vector3::UNIT_X, 375);
    Ogre::Plane plane4(Ogre::Vector3::NEGATIVE_UNIT_X, -375);
    Ogre::Plane plane3b(Ogre::Vector3::UNIT_X, -375);
    Ogre::Plane plane4b(Ogre::Vector3::NEGATIVE_UNIT_X, 375);
    Ogre::Plane plane5(Ogre::Vector3::UNIT_Z, 375);
    Ogre::Plane plane6(Ogre::Vector3::NEGATIVE_UNIT_Z, -375);
    Ogre::Plane plane5b(Ogre::Vector3::UNIT_Z, -375);
    Ogre::Plane plane6b(Ogre::Vector3::NEGATIVE_UNIT_Z, 375);


    Ogre::MeshManager::getSingleton().createPlane("ground1", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane1, 750, 750, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
    Ogre::MeshManager::getSingleton().createPlane("ground2", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane2, 750, 750, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
    Ogre::MeshManager::getSingleton().createPlane("ground3", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane1b, 750, 750, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
    Ogre::MeshManager::getSingleton().createPlane("ground4", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane2b, 750, 750, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Z);
    Ogre::MeshManager::getSingleton().createPlane("ground5", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane3, 750, 750, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
    Ogre::MeshManager::getSingleton().createPlane("ground6", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane4, 750, 750, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
    Ogre::MeshManager::getSingleton().createPlane("ground7", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane3b, 750, 750, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
    Ogre::MeshManager::getSingleton().createPlane("ground8", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane4b, 750, 750, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_Y);
    Ogre::MeshManager::getSingleton().createPlane("ground9", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane5, 750, 750, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_X);
    Ogre::MeshManager::getSingleton().createPlane("ground10", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane6, 750, 750, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_X);
    Ogre::MeshManager::getSingleton().createPlane("ground11", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane5b, 750, 750, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_X);
    Ogre::MeshManager::getSingleton().createPlane("ground12", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        plane6b, 750, 750, 20, 20, true, 1, 5, 5, Ogre::Vector3::UNIT_X);


    Ogre::Entity* entGround1 = mSceneMgr->createEntity("GroundEntity1", "ground1");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround1);
    Ogre::Entity* entGround2 = mSceneMgr->createEntity("GroundEntity2", "ground2");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround2);
    Ogre::Entity* entGround3 = mSceneMgr->createEntity("GroundEntity3", "ground3");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround3);
    Ogre::Entity* entGround4 = mSceneMgr->createEntity("GroundEntity4", "ground4");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround4);
    Ogre::Entity* entGround5 = mSceneMgr->createEntity("GroundEntity5", "ground5");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround5);
    Ogre::Entity* entGround6 = mSceneMgr->createEntity("GroundEntity6", "ground6");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround6);
    Ogre::Entity* entGround7 = mSceneMgr->createEntity("GroundEntity7", "ground7");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround7);
    Ogre::Entity* entGround8 = mSceneMgr->createEntity("GroundEntity8", "ground8");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround8);
    Ogre::Entity* entGround9 = mSceneMgr->createEntity("GroundEntity9", "ground9");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround9);
    Ogre::Entity* entGround10 = mSceneMgr->createEntity("GroundEntity10", "ground10");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround10);
    Ogre::Entity* entGround11 = mSceneMgr->createEntity("GroundEntity11", "ground11");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround11);
    Ogre::Entity* entGround12 = mSceneMgr->createEntity("GroundEntity12", "ground12");
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(entGround12);
 
    std::string material;
    material = "Examples/BumpyMetal";

    entGround1->setMaterialName(material);
    entGround1->setCastShadows(false);
    entGround2->setMaterialName(material);
    entGround2->setCastShadows(false);
    entGround3->setMaterialName(material);
    entGround3->setCastShadows(false);
    entGround4->setMaterialName(material);
    entGround4->setCastShadows(false);
    entGround5->setMaterialName(material);
    entGround5->setCastShadows(false);
    entGround6->setMaterialName(material);
    entGround6->setCastShadows(false);
    entGround7->setMaterialName(material);
    entGround7->setCastShadows(false);
    entGround8->setMaterialName(material);
    entGround8->setCastShadows(false);
    entGround9->setMaterialName(material);
    entGround9->setCastShadows(false);
    entGround10->setMaterialName(material);
    entGround10->setCastShadows(false);
    entGround11->setMaterialName(material);
    entGround11->setCastShadows(false);
    entGround12->setMaterialName(material);
    entGround12->setCastShadows(false);


    entBall = mSceneMgr->createEntity("Ball", "sphere.mesh");
    entBall->setCastShadows(true);
    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("mNode");
    mNode->attachObject(entBall);
    
    entBall->setMaterialName("Examples/Chrome");
    mNode->scale(.3,.3,.3);

    // Set the camera to look at our handiwork
    mCamera->setPosition(90.0f, 280.0f, 135.0f);
    mCamera->pitch(Ogre::Degree(-30.0f));
    mCamera->yaw(Ogre::Degree(-15.0f));
}

void Ballin::createFrameListener(void){
    BaseApplication::createFrameListener();
    // Set default values for variables
    mSpeed = Ogre::Math::RangeRandom(100.0f, 650.0f);
    mDirection = Ogre::Vector3(Ogre::Math::RangeRandom(-100.0f, 100.0f) / 100, Ogre::Math::RangeRandom(-100.0f, 100.0f) / 100, Ogre::Math::RangeRandom(-100.0f, 100.0f) / 100);
}
 
bool Ballin::frameRenderingQueued(const Ogre::FrameEvent &evt){
    Ogre::Real move = mSpeed * evt.timeSinceLastFrame;
    mPosition = mNode->getPosition();
    if (mPosition.x > 335.0f || mPosition.x < -335.0f){
        mDirection.x = -1.0 * mDirection.x;
    }
    if (mPosition.y > 335.0f || mPosition.y < -335.0f){
        mDirection.y = -1.0 * mDirection.y;
    }
    if (mPosition.z > 335.0f || mPosition.z < -335.0f){
        mDirection.z = -1.0 * mDirection.z;
    }
    mNode->translate(mDirection * move);
    return BaseApplication::frameRenderingQueued(evt);
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif
 
#ifdef __cplusplus
extern "C" {
#endif
 
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        Ballin app;
 
        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }
 
        return 0;
    }
 
#ifdef __cplusplus
}
#endif
