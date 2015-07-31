rm CMakeCache.txt
rm -r CMakeFiles
rm Makefile
rm executables/visu

mkdir -p executables
#mkdir -p build
make clean
cmake -G "Unix Makefiles"
make
executables/./visu
