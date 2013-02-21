#include <ControlListener.h>

#include <iostream>
using namespace std;

ControlListener::ControlListener(Ogre::RenderWindow* window){
    this->window = window;
    
    deltaX = 0;
    deltaY = 0;
    xi = 0;
    yi = 0;
    exitCmd = false; 
}

bool ControlListener::mouseMoved(const OIS::MouseEvent& arg){
    //if (trayManager->injectMouseMove(arg)) return true;
    //cameraManager->injectMouseMove(arg);
    
    deltaX = arg.state.X.rel;
    deltaY = arg.state.Y.rel;
    
    cout << "move!" << endl;
    
    return true;
}

bool ControlListener::mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id ){
    if (trayManager->injectMouseDown(arg, id)) return true;
    cameraManager->injectMouseDown(arg, id);
    
    switch (id) {
        case OIS::MB_Left: 
            cout << "Left Click" << endl;
            break;
        case OIS::MB_Right: 
            cout << "Riht Click" << endl;
            break;
    }
    
    return true;
}

bool ControlListener::mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id ){
    if (trayManager->injectMouseUp(arg, id)) return true;
    cameraManager->injectMouseUp(arg, id);
    return true;
}

bool ControlListener::keyReleased(const OIS::KeyEvent& arg){
    cameraManager->injectKeyUp(arg);
    return true;
}

bool ControlListener::keyPressed(const OIS::KeyEvent& arg){
    //cameraManager->injectKeyDown(arg);
    //return true;
    if (trayManager->isDialogVisible()) return true; // Don't process any more keys if dialo is up 
    if (arg.key == OIS::KC_F){ // Toggle visibility of advanced frame stats
        trayManager->toggleAdvancedFrameStats();
    }
    else if (arg.key == OIS::KC_G){ // Toggle visibility of even rarer debugging details
        if (detailsPanel->getTrayLocation()==OgreBites::TL_NONE){
            trayManager->moveWidgetToTray(detailsPanel, OgreBites::TL_TOPRIGHT, 0);
            detailsPanel->show();
        }
        else{
            trayManager->removeWidgetFromTray(detailsPanel);
            detailsPanel->hide();
        }
    }
    //...
    else if (arg.key == OIS::KC_F5){ // Refresh all textures
        Ogre::TextureManager::getSingleton().reloadAll();
    }
    else if (arg.key == OIS::KC_SYSRQ){ // Take a screenshot
        window->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE){
        exitCmd = true;
    }
    return true;
}












