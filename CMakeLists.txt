#policies first
if(COMMAND cmake_policy)
    cmake_policy(SET CMP0048 NEW)
endif()

#declare project
cmake_minimum_required(VERSION 3.10.2) #stick to 18.04 Ubuntu LTS version
project(Dicer
    VERSION 0.1.1
    DESCRIPTION "Dice throws and Macros parser API"
    HOMEPAGE_URL "https://github.com/Amphaal/Dicer"
    LANGUAGES CXX
)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#cpp standards
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(dicer)

#dont build tests if included as submodule
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
