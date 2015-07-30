mkdir -p executables
mkdir -p build
make clean
make main
sleep 0.1
executables/./main
