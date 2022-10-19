cmake_minimum_required(VERSION 3.0)
project(GameLauncher)

include_directories(gameloader common)

set(PROJECT_LINK_LIBS sfml-graphics sfml-system sfml-audio sfml-window pthread boost_system)

find_package(SFML COMPONENTS graphics system audio window REQUIRED)

if(SFML_GRAPHICS_FOUND)
  message(STATUS "SFML Found")
else()
  message(FATAL_ERROR "SFML NOT Found")
endif()

add_definitions(-DFONTFILEPATH=${CMAKE_SOURCE_DIR}/arknoid/resources/fonts/LiberationMono-Regular.ttf)
add_definitions(-DTETRIS_IMAGES_PATH=${CMAKE_SOURCE_DIR}/tetris/images)


add_executable(GameLauncher gameloader/main_file.cpp gameloader/GameLoader.cpp gameloader/MenuClass.cpp common/starEntity.cpp tetris/TetrisLoader.cpp)
target_link_libraries(GameLauncher ${PROJECT_LINK_LIBS})
target_include_directories(GameLauncher PUBLIC tetris gameloader)