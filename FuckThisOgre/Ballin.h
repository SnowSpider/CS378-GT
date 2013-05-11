#ifndef __Ballin_h_
#define __Ballin_h_
 
#include "BaseApplication.h"
 
class Ballin : public BaseApplication
{
public:
    Ballin(void);
    virtual ~Ballin(void);
 
protected:
    virtual void createScene(void);

    virtual void createFrameListener(void);
 
    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
 
    Ogre::Vector3 mDirection;              // The direction the object is moving
    Ogre::Vector3 mPosition;
 
    Ogre::Entity *entBall;                 // The Entity we are animating
    Ogre::SceneNode *mNode;                // The -SceneNode that the Entity is attached to
 
    Ogre::Real mSpeed;                 // The speed at which the object is moving

};
 
#endif // #ifndef __Ballin_h_
