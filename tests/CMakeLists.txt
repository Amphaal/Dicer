#add tests
add_subdirectory(Catch2)
add_executable(dicer_tests tests.cpp)
target_link_libraries(dicer_tests
    dicer
    Catch2::Catch2
)

#tests handling
include(CTest)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Catch2/contrib)
include(Catch)
catch_discover_tests(dicer_tests)