SET "arch=x64"
set "builddir=build" 
SET build=Debug
cmake -DCMAKE_BUILD_TYPE=%build% -H. -B%builddir% -DCMAKE_TOOLCHAIN_FILE=%VCPKG%\scripts\buildsystems\vcpkg.cmake
cmake --build .\%builddir% --config %build%
