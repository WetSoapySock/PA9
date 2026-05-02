cmake_minimum_required(VERSION 4.2)
project(PA9)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(src)

add_executable(PA9
        src/core/game.cpp
        include/core/game.h
        include/core/board.h
        include/core/cell.h
        include/core/gameState.h
        include/graphics/renderer.h
        include/graphics/spriteSheet.h
        include/graphics/UIComponents.h
        
        include/input/mouseHandler.h
        include/utils/constants.h
        include/utils/randomGenerator.h
        include/utils/timer.h
        src/utils/constants.cpp
        src/utils/timer.cpp
        src/utils/randomGenerator.cpp
        
        src/input/mouseHandler.cpp
        src/graphics/spriteSheet.cpp
        src/graphics/renderer.cpp
        src/graphics/UIComponents.cpp
        src/core/gameState.cpp
        src/core/cell.cpp
        src/core/board.cpp  "src/main/main.cpp")

        include(FetchContent)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
)

FetchContent_MakeAvailable(SFML)

target_link_libraries(PA9 PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

add_executable(ServerApp
    server/leaderboardServer.cpp
)