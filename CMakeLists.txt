cmake_minimum_required(VERSION 2.8.9)
project(arkanoid_game)

# all the header files are available in 'src' folder
include_directories(src)

#link_directories(/usr/local/lib)

set(PROJECT_LINK_LIBS sfml-audio sfml-window sfml-graphics sfml-system pthread boost_system X11)

file(GLOB SOURCES "src/mainfile.cpp")

# check if resource files exist(e.g. font files, PNG images and sound files)
# one can create just simple 'res' folder for all the resources and then check if that exist. That is simpler and short IF statement
if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/src/fonts/LiberationMono-Regular.ttf" AND
    EXISTS "${CMAKE_CURRENT_BINARY_DIR}/src/images/bricks.png" AND
    EXISTS "${CMAKE_CURRENT_BINARY_DIR}/src/sounds/BeepPing.wav" AND
    EXISTS "${CMAKE_CURRENT_BINARY_DIR}/src/sounds/booster.wav" )
  MESSAGE("Resources are available")
  add_definitions(-DFONTFILEPATH=${CMAKE_CURRENT_BINARY_DIR}/src/fonts/LiberationMono-Regular.ttf)
  add_definitions(-DBRICKTEXTUREIMAGEFILE=${CMAKE_CURRENT_BINARY_DIR}/src/images/bricks.png)
  add_definitions(-DBEEPSOUNDFILE=${CMAKE_CURRENT_BINARY_DIR}/src/sounds/BeepPing.wav)
  add_definitions(-DBOOSTERSOUNDFILE=${CMAKE_CURRENT_BINARY_DIR}/src/sounds/booster.wav)

  # if yes, then build the Game, otherwise show message and leave
  add_executable(arkanoidGame ${SOURCES})
  #message("current binary dir:" ${CMAKE_CURRENT_BINARY_DIR})
  #message("project binary dir:" ${PROJECT_BINARY_DIR})
  #message(${CMAKE_BINARY_DIR})
  target_link_libraries(arkanoidGame ${PROJECT_LINK_LIBS})

else()
    MESSAGE("ALL Resources are NOT available, Build can not proceed")
endif()


