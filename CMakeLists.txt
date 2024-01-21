cmake_minimum_required(VERSION 3.14)
project(Projet_OpenCV_CMake)


set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV REQUIRED)

include_directories(include ${OpenCV_INCLUDE_DIRS})


add_executable(Projet_OpenCV_CMake
        src/main.cpp
        src/number_page.cpp
        include/number_page.hpp
        src/size_picture.cpp
        include/size_picture.hpp
        src/extract_images.cpp
        src/tools.cpp
        "include/tools.hpp"
        include/extract_images.hpp
        src/find_pictograms.cpp
        include/find_pictograms.hpp
        src/rotate.cpp
        include/rotate.hpp)

target_link_libraries(Projet_OpenCV_CMake ${OpenCV_LIBS})

