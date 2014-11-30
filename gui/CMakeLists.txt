set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

qt5_add_resources(resources
    DiagramScene.qrc
)

add_executable(dataflow WIN32
    ${resources}
    Arrow.h
    Arrow.cpp
    DiagramModuleItem.h
    DiagramModuleItem.cpp
    DiagramScene.h
    DiagramScene.cpp
    MainWindow.h
    MainWindow.cpp
    Module.h
    Module.cpp
    ModuleDescription.h
    ModuleDescription.cpp
    ModuleIn.h
    ModuleIn.cpp
    ModuleOut.h
    ModuleOut.cpp
    ModulesListModel.h
    ModulesListModel.cpp
    main.cpp
)

target_link_libraries(dataflow Qt5::Widgets)

source_group("Resources" "\\.qrc$")
