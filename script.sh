mkdir -p executables
mkdir -p build
make clean
cmake -G "Unix Makefiles"
make
#make clean
#make main
executables/./visu
