//|||||||||||||||||||||||||||||||||||||||||||||||
 
#ifndef MULTI_GAME_STATE_HPP
#define MULTI_GAME_STATE_HPP
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "AppState.hpp"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
class MultiGameState : public AppState
{
public:
    MultiGameState();
 
    DECLARE_APPSTATE_CLASS(MultiGameState)
 
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
};
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
#endif
 
//|||||||||||||||||||||||||||||||||||||||||||||||
