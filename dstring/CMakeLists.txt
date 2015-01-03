set(sources
    dstring.h
    dstring.cpp
)

include_directories(
    ${dataflow_SOURCE_DIR}/include
)

add_library(dstring SHARED ${sources})
set_target_properties(dstring PROPERTIES
    DEFINE_SYMBOL DSTRING_EXPORTS
)
