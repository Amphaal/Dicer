#PEGTL
include(FetchContent)
FetchContent_Declare(pegtl
    GIT_REPOSITORY  "https://github.com/taocpp/PEGTL.git"
)

FetchContent_MakeAvailable(pegtl)

#lib
# add_library(dicer STATIC)
add_executable(dicer)

target_link_libraries(dicer PRIVATE taocpp::pegtl)

target_include_directories(dicer
    PRIVATE include/dicer
    INTERFACE .
    INTERFACE include
)

target_sources(dicer PRIVATE
    src/Parser.cpp
    src/DicerPEGTL.hpp
)