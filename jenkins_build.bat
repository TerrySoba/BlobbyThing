mkdir build
cd build
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86

cmake .. -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=%CD%/install
rem cmake .. -G "Visual Studio 12 2013" -DCMAKE_INSTALL_PREFIX:PATH=%CD%/install
nmake
nmake install
cd install
7z a BlobbyThing.zip bin
pause
