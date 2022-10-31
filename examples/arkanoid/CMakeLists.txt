project(arkanoid)
add_executable(${PROJECT_NAME} main.cpp window.cpp blocks.cpp bullets.cpp
                               paddle.cpp starlayers.cpp)
enable_abcg(${PROJECT_NAME})
