cmake_minimum_required(VERSION 2.8.9)
project(TetrisGame)

set(PROJECT_LINK_LIBS sfml-graphics sfml-system sfml-audio sfml-window pthread boost_system)


add_executable(tetris main.cpp)
target_link_libraries(tetris ${PROJECT_LINK_LIBS})
