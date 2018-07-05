cmake_minimum_required(VERSION 2.8.9)
project(arkanoid_game)

include_directories(src)

#link_directories(/usr/local/lib)

set(PROJECT_LINK_LIBS sfml-audio sfml-window sfml-graphics sfml-system pthread X11)

file(GLOB SOURCES "src/mainfile.cpp")

add_definitions(-DFILEPATH=${CMAKE_CURRENT_BINARY_DIR}/src/fonts/LiberationMono-Regular.ttf)
add_definitions(-DBRICKTEXTUREIMAGEFILE=${CMAKE_CURRENT_BINARY_DIR}/src/images/bricks.png)
add_definitions(-DBEEPSOUNDFILE=${CMAKE_CURRENT_BINARY_DIR}/src/sounds/BeepPing.wav)

add_executable(arkanoidGame ${SOURCES})
#message("current binary dir:" ${CMAKE_CURRENT_BINARY_DIR})
#message("project binary dir:" ${PROJECT_BINARY_DIR})
#message(${CMAKE_BINARY_DIR})
target_link_libraries(arkanoidGame ${PROJECT_LINK_LIBS})
