# Add the semantic analysis source files
file(GLOB_RECURSE UTILS_SOURCES "*.cpp")

# Add the semantic analysis library
add_library(utils ${UTILS_SOURCES}
        lexer_utils.h
        extension.h
        constants.h
        class_type.h
        class_type.cpp)

# Include directories for semantic analysis
target_include_directories(utils PUBLIC ${CMAKE_SOURCE_DIR}/include)
