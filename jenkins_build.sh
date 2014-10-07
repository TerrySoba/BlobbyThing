mkdir build
cd build
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX:PATH=$PWD/install -DBLOBBY_VERSION="Build ${MERCURIAL_REVISION_SHORT}:${BUILD_NUMBER}"
make -j 4
make install
cd install
7z a ${JOB_NAME}_${BUILD_ID}.zip bin

