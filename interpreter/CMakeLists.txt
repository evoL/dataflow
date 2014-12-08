add_executable(datastore_example
    ManagedMemoryChunk.h
    ManagedMemoryChunk.cpp
    Datastore.h
    Datastore.cpp
    datastore_example.cpp
)

add_executable(libraryloader_example
    library_loading.h
    library_loading.cpp
    Library.h
    Library.cpp
    LibraryLoader.h
    LibraryLoader.cpp
    libraryloader_example.cpp
)

find_package(LibXml2 REQUIRED)
include_directories(${LIBXML2_INCLUDE_DIR})

add_executable(xmlparser_example
    DataflowModel.h
    library_loading.h
    library_loading.cpp
    Library.h
    Library.cpp
    LibraryLoader.h
    LibraryLoader.cpp
    XMLParser.h
    XMLParser.cpp
    ModelManipulator.h
    ModelManipulator.cpp
    xmlparser_example.cpp
)

add_executable(interpreter_example
    Interpreter.h
    Interpreter.cpp
    DataflowModel.h
    Library.h
    Library.cpp
    LibraryLoader.h
    LibraryLoader.cpp
    library_loading.h
    library_loading.cpp
    XMLParser.h
    XMLParser.cpp
    ModelManipulator.h
    ModelManipulator.cpp
    ManagedMemoryChunk.h
    ManagedMemoryChunk.cpp
    Datastore.h
    Datastore.cpp
    BlockVisitor.h
    interpreter_example.cpp
)

set(LIBRARIES ${LIBXML2_LIBRARIES})

# Add Iconv for Visual C++
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    find_package(Iconv REQUIRED)
    include_directories(${ICONV_INCLUDE_DIR})
    list(APPEND ${LIBRARIES} ${ICONV_LIBRARIES})
endif()

target_link_libraries(xmlparser_example ${LIBRARIES})
target_link_libraries(interpreter_example ${LIBRARIES})

if(UNIX)
    target_link_libraries(libraryloader_example dl)
endif()

add_dependencies(libraryloader_example basicmath)
