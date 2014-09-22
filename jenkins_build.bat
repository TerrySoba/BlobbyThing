mkdir build
cd build
call "D:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86

set THIRD_PARTY_LIBDIR=D:\DevLibraries

set SDL2DIR=%THIRD_PARTY_LIBDIR%\SDL2-2.0.3
set BOOST_ROOT=%THIRD_PARTY_LIBDIR%\boost_1_56_0
set ICU_ROOT=%THIRD_PARTY_LIBDIR%\icu-54.1-win32-release
set GLEWDIR=%THIRD_PARTY_LIBDIR%\glew-1.11.0
set SDL2IMAGEDIR=%THIRD_PARTY_LIBDIR%\SDL2_image-2.0.0

cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=%CD%/install
rem cmake .. -G "Visual Studio 12 2013" -DCMAKE_BUILD_TYPE=Release
nmake