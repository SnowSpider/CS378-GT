/*
-----------------------------------------------------------------------------
Filename: TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
___ __ __ _ _ _
/___\__ _ _ __ ___ / / /\ \ (_) | _(_)
// // _` | '__/ _ \ \ \/ \/ / | |/ / |
/ \_// (_| | | | __/ \ /\ /| | <| |
\___/ \__, |_| \___| \/ \/ |_|_|\_\_|
|___/
Tutorial Framework
http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"

using namespace std;

class TutorialApplication : public BaseApplication
{
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);

protected:
    
    Ogre::Real mNumber;
    Ogre::Vector3 mTempVector;
    
    Ogre::Vector3 mDirection;
    Ogre::Vector3 mCurrPosition;
    Ogre::Vector3 mNextPosition;
    
    Ogre::Entity *ball;
    Ogre::Entity *shadow;
    Ogre::Entity *paddle;
    Ogre::SceneNode *ballNode;
    Ogre::SceneNode *paddleNode;
    Ogre::SceneNode *shadowNode;


 
    Ogre::Real mBallSpeed;
    
    virtual void createScene(void);
    
    virtual void createFrameListener(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
    
};

#endif // #ifndef __TutorialApplication_h_

