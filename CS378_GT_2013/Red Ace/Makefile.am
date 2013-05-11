noinst_HEADERS = AdvancedOgreFramework.hpp AppState.hpp AppStateManager.hpp DemoApp.hpp DotSceneLoader.hpp GameState.hpp MenuState.hpp PauseState.hpp rapidxml.hpp

bin_PROGRAMS = OgreApp
OgreApp_CPPFLAGS = -I$(top_srcdir)
OgreApp_SOURCES = AdvancedOgreFramework.cpp AppStateManager.cpp DemoApp.cpp DotSceneLoader.cpp GameState.cpp main.cpp MenuState.cpp PauseState.cpp
OgreApp_CXXFLAGS= $(OGRE_CFLAGS) $(OGRE_TERRAIN_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(CEGUI_CFLAGS) $(CEGUI_OGRE_CFLAGS) $(sdl_CFLAGS)
OgreApp_LDADD= $(OGRE_LIBS) $(OGRE_TERRAIN_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS) $(sdl_LIBS)

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
