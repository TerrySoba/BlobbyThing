mkdir build
cd build

rem IF DEFINED MSVC_INSTALL_PATH (
rem ECHO MSVC_INSTALL_PATH is defined
rem )  ELSE (
rem ECHO MSVC_INSTALL_PATH is NOT defined, using default
rem set MSVC_INSTALL_PATH="C:\Program Files (x86)\Microsoft Visual Studio 12.0"
rem )

call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86

cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=%CD%/install
rem cmake .. -G "Visual Studio 12 2013" -DCMAKE_INSTALL_PREFIX:PATH=%CD%/install
nmake
nmake install
cd install
7z a %JOB_NAME%_%BUILD_ID%.zip bin
