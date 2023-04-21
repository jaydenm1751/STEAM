cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(WaterVapor)

set(CMAKE_CXX_STANDARD 14)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS system window graphics audio network REQUIRED)
add_executable(WaterVapor main.cpp HashTable.h StorePage.cpp StorePage.h ExcelReader.cpp ExcelReader.h HashTableApp.h HashTableApp.h)
target_link_libraries(WaterVapor sfml-graphics sfml-audio)
