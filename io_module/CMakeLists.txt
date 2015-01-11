set(sources
    io_module.h
    io_module.cpp
)

include_directories(
    ${dataflow_SOURCE_DIR}/include
    ${dataflow_SOURCE_DIR}/basicmath
    ${dataflow_SOURCE_DIR}/dstring
)

add_library(io_module SHARED ${sources})
set_target_properties(io_module PROPERTIES
    DEFINE_SYMBOL BASIC_IO_EXPORTS
)

install(TARGETS io_module DESTINATION ${DATAFLOW_OUTPUT_DIR})
