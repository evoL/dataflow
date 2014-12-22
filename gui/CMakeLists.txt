set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

qt5_add_resources(resources
    DiagramScene.qrc
)

add_executable(dataflow_gui WIN32
    ${resources}
    Arrow.h
    Arrow.cpp
    DiagramBlock.h
    DiagramBlock.cpp
	DiagramOperation.h
	DiagramOperation.cpp
	DiagramConstructor.h
	DiagramConstructor.cpp
    DiagramScene.h
    DiagramScene.cpp
    MainWindow.h
    MainWindow.cpp
    Module.h
    Module.cpp
    BlockDescription.h
    BlockDescription.cpp
    BlockIn.h
    BlockIn.cpp
    BlockOut.h
    BlockOut.cpp
	ModulesPanelItem.h
	ModulesPanelItem.cpp
	ModulesPanelModel.h
	ModulesPanelModel.cpp
    main.cpp
)

include_directories(
    "${PROJECT_SOURCE_DIR}/interpreter"
    "${PROJECT_BINARY_DIR}/interpreter"
    ${LIBXML2_INCLUDE_DIR}
    ${ICONV_INCLUDE_DIR}
)

add_dependencies(dataflow_gui dataflow)
target_link_libraries(dataflow_gui dataflow Qt5::Widgets)

source_group("Resources" "\\.qrc$")
