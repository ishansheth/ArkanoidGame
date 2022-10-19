cmake_minimum_required(VERSION 2.8.9)
project(arkanoid_game)

# all the header files are available in 'src' folder
include_directories(include)
include_directories(src)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)
set(CMAKE_CXX_FLAGS "-fsanitize=address -fsanitize-address-use-after-scope")
# /usr/local/lib/cmake/SFML has the SFMLConfig.cmake
find_package(SFML COMPONENTS graphics system audio window REQUIRED)

if(SFML_GRAPHICS_FOUND)
  message(STATUS "SFML Found")
else()
  message(FATAL_ERROR "SFML NOT Found")
endif()

set(PROJECT_LINK_LIBS sfml-graphics sfml-system sfml-audio sfml-window pthread boost_system)

file(GLOB SOURCES
  src/*.cpp
  )

# check if resource files exist(e.g. font files, PNG images and sound files)
# one can create just simple 'res' folder for all the resources and then check if that exist. That is simpler and short IF statement
if(EXISTS "${CMAKE_SOURCE_DIR}/resources/fonts/LiberationMono-Regular.ttf" AND
    EXISTS "${CMAKE_SOURCE_DIR}/resources/images/bricks.png" AND
    EXISTS "${CMAKE_SOURCE_DIR}/resources/sounds/BeepPing.wav" AND
    EXISTS "${CMAKE_SOURCE_DIR}/resources/sounds/booster.wav" )
  MESSAGE(STATUS "Resources are available")
  add_definitions(-DFONTFILEPATH=${CMAKE_SOURCE_DIR}/resources/fonts/LiberationMono-Regular.ttf)
  add_definitions(-DBRICKTEXTUREIMAGEFILE=${CMAKE_SOURCE_DIR}/resources/images/bricks.png)
  add_definitions(-DBEEPSOUNDFILE=${CMAKE_SOURCE_DIR}/resources/sounds/BeepPing.wav)
  add_definitions(-DBOOSTERSOUNDFILE=${CMAKE_SOURCE_DIR}/resources/sounds/booster.wav)
  add_definitions(-DBULLETSOUNDFILE=${CMAKE_SOURCE_DIR}/resources/sounds/bullet.wav)

  # if yes, then build the Game, otherwise show message and leave
  add_executable(arkanoidGame ${SOURCES})
  target_link_libraries(arkanoidGame ${PROJECT_LINK_LIBS})

  install(TARGETS arkanoidGame DESTINATION bin)
else()
    MESSAGE(FATAL_ERROR "ALL Resources are NOT available, Build failed")
endif()


