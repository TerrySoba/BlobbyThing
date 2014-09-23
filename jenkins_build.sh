mkdir build
cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=$PWD/install
ninja
ninja install
cd install
7z a ${JOB_NAME}_${BUILD_ID}.zip bin

