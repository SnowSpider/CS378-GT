#pragma once
 
#include "OgreReferenceAppLayer.h"
 
#include "ExampleRefAppFrameListener.h"
 
class GameFrameListener : public ExampleRefAppFrameListener
{
protected:
	ApplicationObject *ball;
 
public:
    GameFrameListener(RenderWindow* win, CollideCamera* cam, ApplicationObject *pball)
        : ExampleRefAppFrameListener(win, cam), ball(pball)
    {
    }
 
 
    bool frameEnded(const FrameEvent& evt)
    {
	// Call superclass
        bool ret = ExampleRefAppFrameListener::frameEnded(evt);
 
	// Respond to keyboard: SPACE FIRES
	if (mKeyboard->isKeyDown(OIS::KC_SPACE))
        {
            ball->setPosition(mCamera->getPosition() + 
                mCamera->getDirection() * mCamera->getNearClipDistance() * 2);
            ball->setLinearVelocity(mCamera->getDirection() * 200);
            ball->setAngularVelocity(Vector3::ZERO);
        }
 
        return ret;
    }
};
