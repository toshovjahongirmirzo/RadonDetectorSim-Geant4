cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(RadonDetectorSim)

find_package(Geant4 REQUIRED)
include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(RadonDetectorSim main.cc ${sources} ${headers})
target_link_libraries(RadonDetectorSim ${Geant4_LIBRARIES})
