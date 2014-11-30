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
    ModuleLoader.h
    LibraryLoader.h
    LibraryLoader.cpp
    libraryloader_example.cpp
)

if(UNIX)
    target_link_libraries(libraryloader_example dl)
endif()

add_dependencies(libraryloader_example basicmath)
