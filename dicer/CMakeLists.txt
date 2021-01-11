#PEGTL
SET(PEGTL_BUILD_TESTS OFF CACHE BOOL "" FORCE)
SET(PEGTL_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(PEGTL EXCLUDE_FROM_ALL)

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
    include/dicer/PEGTL/Actions.hpp
    include/dicer/PEGTL/Grammar.hpp
    include/dicer/PEGTL/ResolvingMethods.hpp
    include/dicer/_Base.hpp
    include/dicer/DiceThrow.hpp
    include/dicer/FacedDiceThrow.hpp
    include/dicer/NamedDiceThrow.hpp
    include/dicer/ThrowCommand.hpp
    include/dicer/ThrowCommandExtract.hpp
    include/dicer/ThrowCommandStack.hpp
    include/dicer/IDescriptible.hpp
    include/dicer/Resolvable.hpp
    include/dicer/Resolver.hpp
    include/dicer/Parser.hpp
    include/dicer/ThrowRepartition.hpp
    include/dicer/NamedDice.hpp
    include/dicer/Contexts.hpp
    include/dicer/CommandDescriptorHelper.hpp
    include/dicer/Exceptions.hpp
)