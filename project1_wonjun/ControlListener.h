#ifndef __ContolListener_h_
#define __ContolListener_h_

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreRenderWindow.h>
#include <SdkTrays.h>
#include <SdkCameraMan.h>

class ControlListener : public OIS::KeyListener, public OIS::MouseListener {
    private:
    int deltaX;
    int deltaY;
    int xi;
    int yi;
    
    protected:
    OIS::InputManager* inputManager;
    OIS::Mouse* mouse;
    OIS::Keyboard* keyboard;
    
    Ogre::RenderWindow* window;
    Ogre::Camera* camera;
    OgreBites::SdkTrayManager* trayManager;
    OgreBites::SdkCameraMan* cameraManager;
    OgreBites::ParamsPanel* detailsPanel;
    bool exitCmd; // mShutDown
    
     // OIS::KeyListener
    virtual bool keyPressed(const OIS::KeyEvent& arg);
    virtual bool keyReleased(const OIS::KeyEvent& arg);
    
    // OIS::MouseListener
    virtual bool mouseMoved(const OIS::MouseEvent& arg);
    virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id );
    virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id );
    
    public:
    ControlListener(Ogre::RenderWindow* window);
    //~ControlListener();
    /*
    void detach();
    void update();
    void resizeWindow(unsigned int width, unsigned int height);
    void setCamera(Ogre::Camera* cam);
    void setTrayManager(OgreBites::SdkTrayManager* tM);
    void setCameraManager(OgreBites::SdkCameraMan* cM);
    void setDetailsPanel(OgreBites::ParamsPanel* pp);
    */
    OIS::Mouse* getMouse() { return mouse; }
    bool exit() { return exitCmd; }
};

#endif // #ifndef __ContolListener_h_
