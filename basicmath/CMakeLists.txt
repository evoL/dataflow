set(sources
    basicmath.h
    basicmath.cpp
)

include_directories(${dataflow_SOURCE_DIR}/include)

add_library(basicmath SHARED ${sources})
set_target_properties(basicmath PROPERTIES
    DEFINE_SYMBOL BASIC_MATH_EXPORTS
)
