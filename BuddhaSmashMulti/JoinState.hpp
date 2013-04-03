//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef JOIN_STATE_HPP
#define JOIN_STATE_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "AppState.hpp"

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/CEGUIOgreRenderer.h>
 
#include <iostream>
using namespace std; 

#include "EditString.h"

#include <iostream>
#include <fstream>
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class JoinState : public AppState
{
public:
    JoinState();
 
    DECLARE_APPSTATE_CLASS(JoinState)
 
    void enter();
    void createScene();
    void exit();
 
    bool keyPressed(const OIS::KeyEvent &keyEventRef);
    bool keyReleased(const OIS::KeyEvent &keyEventRef);
 
    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
 
    void buttonHit(OgreBites::Button* button);
    void yesNoDialogClosed(const Ogre::DisplayString& question, bool yesHit);
 
    void update(double timeSinceLastFrame);
 
private:
    bool m_bQuit;
    bool m_bQuestionActive;
    
    CEGUI::OgreRenderer* mRenderer;
    EditString targetAddress;
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||
