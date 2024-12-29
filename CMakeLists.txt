cmake_minimum_required(VERSION 3.5.0)
project(RunAwayFromTheGoblin VERSION 0.1.0 LANGUAGES C CXX)

find_package(SFML 2.5 COMPONENTS 
             graphics 
             audio 
             window 
             system 
             REQUIRED)

add_executable(${PROJECT_NAME} 
               src/main.cpp
               src/game.cpp
               src/playerCharacter.cpp
               src/goblin.cpp)

target_include_directories(${PROJECT_NAME}  PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE 
                      sfml-graphics 
                      sfml-audio 
                      sfml-window 
                      sfml-system)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
