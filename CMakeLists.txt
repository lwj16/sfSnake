cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/PJ/bin)

include(FetchContent)
FetchContent_Declare(SFML
    URL file://${CMAKE_SOURCE_DIR}/3rd/rar/zip/SFML-2.6.2-sources.zip
    )
FetchContent_MakeAvailable(SFML)

add_executable(sfSnake src/main.cpp src/Fruit.cpp src/Map.cpp src/Game.cpp src/GameOverScreen.cpp src/GameScreen.cpp src/MenuScreen.cpp src/SettingsScreen.cpp src/Screen.cpp src/AutoSnake.cpp src/Snake.cpp src/SnakeNode.cpp)
target_compile_features(sfSnake PRIVATE cxx_std_17)
# target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/3rd/include)
# target_link_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/3rd/lib)
target_link_libraries(sfSnake PRIVATE sfml-graphics sfml-window sfml-system sfml-audio sfml-network)