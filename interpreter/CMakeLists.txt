find_package(LibXml2 REQUIRED)
find_package(Iconv REQUIRED)
include_directories(
    ${LIBXML2_INCLUDE_DIR}
    ${ICONV_INCLUDE_DIR}
)

add_library(dataflow SHARED
    BlockVisitor.h
    DataflowModel.h
    Datastore.cpp
    Datastore.h
    Interpreter.cpp
    Interpreter.h
    Library.cpp
    Library.h
    LibraryLoader.cpp
    LibraryLoader.h
    ManagedMemoryChunk.cpp
    ManagedMemoryChunk.h
    ModelManipulator.cpp
    ModelManipulator.h
    XMLParser.cpp
    XMLParser.h
    library_loading.cpp
    library_loading.h
)

target_link_libraries(dataflow
    ${LIBXML2_LIBRARIES}
    ${ICONV_LIBRARIES}
)

if(UNIX)
    target_link_libraries(dataflow dl)
endif()

add_executable(datastore_example datastore_example.cpp)
add_executable(libraryloader_example libraryloader_example.cpp)
add_executable(interpreter_example interpreter_example.cpp)

target_link_libraries(datastore_example dataflow)
target_link_libraries(libraryloader_example dataflow)
target_link_libraries(interpreter_example dataflow)
