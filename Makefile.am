ACLOCAL_AMFLAGS= -I m4
noinst_HEADERS= BaseApplication.h TutorialApplication.h

bin_PROGRAMS= Balla
Balla_CPPFLAGS= -I$(top_srcdir)
Balla_SOURCES= BaseApplication.cpp TutorialApplication.cpp
Balla_CXXFLAGS= $(OGRE_CFLAGS) $(OIS_CFLAGS)
Balla_LDADD= $(OGRE_LIBS) $(OIS_LIBS)

EXTRA_DIST= buildit makeit
AUTOMAKE_OPTIONS= foreign
