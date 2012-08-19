# Locate GLEW library
# This module defines
# GLEW_LIBRARY, the name of the library to link against
# GLEW_FOUND, if false, do not try to link to GLEW
# GLEW_INCLUDE_DIR, where to find glew.h
#
# $GLEWDIR is an environment variable 
#
# This was influenced by the FindSDL.cmake 
# module.

#=============================================================================
# Copyright 2005-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

FIND_PATH(GLEW_INCLUDE_DIR GL/glew.h
  HINTS
  $ENV{GLEWDIR}
  PATH_SUFFIXES include
  PATHS
  /usr/include
  /opt/include
)

FIND_LIBRARY(GLEW_LIBRARY 
  NAMES GLEW
  HINTS
  $ENV{GLEWDIR}
  PATH_SUFFIXES lib64 lib
  PATHS
  ~/Library/Frameworks
  /Library/Frameworks
  /usr/local
  /usr
  /sw
  /opt/local
  /opt/csw
  /opt
)

SET(GLEW_FOUND "NO")
IF(GLEW_LIBRARY AND GLEW_INCLUDE_DIR)
  SET(GLEW_FOUND "YES")
ENDIF(GLEW_LIBRARY AND GLEW_INCLUDE_DIR)

