//|||||||||||||||||||||||||||||||||||||||||||||||
 
#include "DemoApp.hpp"
 
#include "MenuState.hpp"
#include "SingleGameState.hpp"
//#include "MultiGameState.hpp"
#include "PauseState.hpp"
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
DemoApp::DemoApp()
{
	m_pAppStateManager = 0;
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
DemoApp::~DemoApp()
{
	delete m_pAppStateManager;
        delete OgreFramework::getSingletonPtr();
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
 
void DemoApp::startDemo()
{
	new OgreFramework();
	if(!OgreFramework::getSingletonPtr()->initOgre("AdvancedOgreFramework", 0, 0))
		return;
 
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");
 
	m_pAppStateManager = new AppStateManager();
 
	MenuState::create(m_pAppStateManager, "MenuState");
	SingleGameState::create(m_pAppStateManager, "SingleGameState");
	//MultiGameState::create(m_pAppStateManager, "MultiGameState");
    PauseState::create(m_pAppStateManager, "PauseState");
 
	m_pAppStateManager->start(m_pAppStateManager->findByName("MenuState"));
}
 
//|||||||||||||||||||||||||||||||||||||||||||||||
