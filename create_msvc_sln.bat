mkdir build
cd build

IF DEFINED MSVC_INSTALL_PATH (
	ECHO MSVC_INSTALL_PATH is defined
)  ELSE (
	ECHO MSVC_INSTALL_PATH is NOT defined, using default
	set MSVC_INSTALL_PATH="C:\Program Files (x86)\Microsoft Visual Studio 12.0"
)

call "%MSVC_INSTALL_PATH%\VC\vcvarsall.bat" x86

rem cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=%CD%/install
cmake .. -G "Visual Studio 12 2013" -DCMAKE_INSTALL_PREFIX:PATH=%CD%/install
rem nmake
rem nmake install
rem cd install
rem 7z a %JOB_NAME%_%BUILD_ID%.zip bin
