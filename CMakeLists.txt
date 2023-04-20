cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(WaterVapor)

set(CMAKE_CXX_STANDARD 14)

set(SFML_DIR "<path_to_SFML>/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)
add_executable(WaterVapor main.cpp HashTable.h StorePage.cpp StorePage.h ExcelReader.cpp ExcelReader.h)
target_link_libraries(SteamTwo sfml-graphics sfml-audio)
