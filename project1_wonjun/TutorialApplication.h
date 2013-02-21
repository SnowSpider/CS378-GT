/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"
#include "GameObject.h"

using namespace std;

class TutorialApplication : public BaseApplication
{
private:
    int deltaX;
    int deltaY;
    int xi;
    int yi;
    GameObject myPaddle;
    
public:
    TutorialApplication(void);
    virtual ~TutorialApplication(void);

protected:
    
    Ogre::Real mNumber;
    Ogre::Vector3 mTempVector;
    
    Ogre::Vector3 mDirection;
    Ogre::Vector3 mCurrPosition;
    Ogre::Vector3 mNextPosition;
    
    Ogre::Entity *mBall;
    Ogre::SceneNode *mNode;
 
    Ogre::Real mBallSpeed;
    
    bool exitCmd;
    
    virtual void createScene(void);
    
    // Ogre::FrameListener
    virtual void createFrameListener(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
    
    // OIS::KeyListener
    //virtual bool keyPressed( const OIS::KeyEvent &evt );
    //virtual bool keyReleased( const OIS::KeyEvent &evt );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &evt );
    virtual bool mousePressed( const OIS::MouseEvent &evt, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &evt, OIS::MouseButtonID id );
};

#endif // #ifndef __TutorialApplication_h_
