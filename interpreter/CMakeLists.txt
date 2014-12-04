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
    xmlparser_example.cpp
)

target_link_libraries(xmlparser_example ${LIBXML2_LIBRARIES})

if(UNIX)
    target_link_libraries(libraryloader_example dl)
endif()

add_dependencies(libraryloader_example basicmath)
