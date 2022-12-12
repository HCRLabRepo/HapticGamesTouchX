cd ../../external/fuzzylite
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
echo "Building now"
devenv fuzzylite.sln /build Release