# fetch
if(NOT TARGET Catch2::Catch2)
    message("Including Catch2 !")
    Include(FetchContent)

    FetchContent_Declare(Catch2
        GIT_REPOSITORY "https://github.com/catchorg/Catch2"
        GIT_TAG "v2.x"
    )

    FetchContent_MakeAvailable(Catch2)
    FetchContent_GetProperties(Catch2
        SOURCE_DIR CATCH_SOURCE_DIR
    )
endif()

#add tests
add_executable(dicer_tests 
    tests.cpp
    specialized/TestUtility.hpp
)

target_link_libraries(dicer_tests
    dicer
    Catch2::Catch2
)

#tests handling
include(CTest)

list(APPEND CMAKE_MODULE_PATH ${CATCH_SOURCE_DIR}/contrib)
include(Catch)
catch_discover_tests(dicer_tests)