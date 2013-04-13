noinst_HEADERS = AdvancedOgreFramework.hpp AppState.hpp AppStateManager.hpp ClientGameState.hpp DemoApp.hpp DotSceneLoader.hpp EditString.h GameObject.h HostGameState.hpp JoinState.hpp MenuState.hpp MultiGameState.hpp OgreMotionState.h PauseState.hpp Planet.h Simulator.h SingleGameState.hpp SoundManager.h rapidxml.hpp

bin_PROGRAMS = OgreApp
OgreApp_CPPFLAGS = -I$(top_srcdir)
OgreApp_SOURCES = AdvancedOgreFramework.cpp AppStateManager.cpp ClientGameState.cpp DemoApp.cpp DotSceneLoader.cpp EditString.cpp GameObject.cpp HostGameState.cpp JoinState.cpp main.cpp MenuState.cpp MultiGameState.cpp PauseState.cpp Planet.cpp Simulator.cpp SingleGameState.cpp SoundManager.cpp
OgreApp_CXXFLAGS= $(OGRE_CFLAGS) $(OGRE_TERRAIN_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(CEGUI_CFLAGS) $(CEGUI_OGRE_CFLAGS) $(sdl_CFLAGS) $(SDL_net_CFLAGS)
OgreApp_LDADD= $(OGRE_LIBS) $(OGRE_TERRAIN_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS) $(sdl_LIBS) $(SDL_net_LIBS)

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
