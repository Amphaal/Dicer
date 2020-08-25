#PEGTL
include(FetchContent)
FetchContent_Declare(pegtl
    GIT_REPOSITORY  "https://github.com/taocpp/PEGTL.git"
)

FetchContent_MakeAvailable(pegtl)

#lib
add_library(dicer STATIC)
set_target_properties(dicer PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(dicer PUBLIC taocpp::pegtl)

target_include_directories(dicer
    PRIVATE include/dicer
    INTERFACE .
    INTERFACE include
)

target_sources(dicer PRIVATE
    include/dicer/DicerPEGTL.hpp
    include/dicer/Throw.hpp
    include/dicer/Resolver.hpp
)