ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BaseApplication.h TutorialApplication.h

bin_PROGRAMS= OgreApp
OgreApp_CPPFLAGS= -I$(top_srcdir)
OgreApp_SOURCES= BaseApplication.cpp TutorialApplication.cpp GameObject.cpp Simulator.cpp
OgreApp_CXXFLAGS= $(OGRE_CFLAGS) $(OGRE_TERRAIN_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(CEGUI_CFLAGS) $(CEGUI_OGRE_CFLAGS)
OgreApp_LDADD= $(OGRE_LIBS) $(OGRE_TERRAIN_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(CEGUI_LIBS) $(CEGUI_OGRE_LIBS)

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
