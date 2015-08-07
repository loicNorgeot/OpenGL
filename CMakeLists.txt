#HEADER DU PROJET
cmake_minimum_required(VERSION 2.8)
project(VISU)
set(CMAKE_BUILD_TYPE "Release")

#Options de compilation
set (CMAKE_C_FLAGS "-lGL -lGLEW -lglfw3  -lX11 -lXxf86vm -pthread -lXrandr -lXinerama -lXi -lXcursor -lm -std=c++11 -g  -O2 -lfreeimage")

#########################
#       LIBRAIRIES      #
#########################
#OPENGL & co
find_package(OpenGL REQUIRED)
find_package(GLEW   REQUIRED)
find_package(X11    REQUIRED)
#GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})
#FreeImage
find_path(SIFTGPU_INCLUDE_DIR FreeImage.h)
find_library(SIFTGPU_LIBRARY libfreeimage)
include_directories(${SIFTGPU_INCLUDE_DIR})

#Concatenation pour le linkage de la fin
set(LIBS ${GLEW_LIBRARY} ${GLFW_STATIC_LIBRARIES} ${X11_LIBRARIES} ${GL_LIBRARY} freeimage) #assimp)

#########################
#       EXECUTABLE      #
#########################
#Ajout des fichiers
include_directories("sources/")
file(GLOB_RECURSE source_files sources/*)
#Spécification de l'exe
set(EXECUTABLE_OUTPUT_PATH "executables/")
add_executable(visu ${source_files})
#Linkage final
target_link_libraries(visu ${LIBS})




