noinst_HEADERS= BaseApplication.h Ballin.h
 
bin_PROGRAMS= OgreApp
 OgreApp_CPPFLAGS= -I$(top_srcdir)
 OgreApp_SOURCES= BaseApplication.cpp Ballin.cpp
 OgreApp_CXXFLAGS= $(OGRE_CFLAGS) $(OGRE_TERRAIN_CFLAGS) $(OIS_CFLAGS)
 OgreApp_LDADD= $(OGRE_LIBS) $(OGRE_TERRAIN_LIBS) $(OIS_LIBS)
 
 EXTRA_DIST = bootstrap
 AUTOMAKE_OPTIONS = foreign
