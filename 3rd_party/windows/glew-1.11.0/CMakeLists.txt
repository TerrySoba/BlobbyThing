set(GLEW_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include
	CACHE INTERNAL "GLEW Include Directory")

set(GLEW_LIBRARY
	${CMAKE_CURRENT_SOURCE_DIR}/lib/Release/Win32/glew32.lib
	CACHE INTERNAL "GLEW libs")
	
INSTALL(FILES ${CMAKE_CURRENT_SOURCE_DIR}/bin/Release/Win32/glew32.dll
  DESTINATION bin)