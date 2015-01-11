set(sources
    ppm.h
    ppm.cpp
)

include_directories(
    ${dataflow_SOURCE_DIR}/include
)

add_library(ppm SHARED ${sources})
set_target_properties(ppm PROPERTIES
    DEFINE_SYMBOL PPM_EXPORTS
)

install(TARGETS ppm DESTINATION ${DATAFLOW_OUTPUT_DIR})
