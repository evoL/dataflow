set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets REQUIRED)

add_executable(dataflow WIN32
    Arrow.h
    Arrow.cpp
    DiagramModuleItem.h
    DiagramModuleItem.cpp
    DiagramScene.h
    DiagramScene.cpp
    DiagramScene.qrc
    MainWindow.h
    MainWindow.cpp
    Module.h
    Module.cpp
    ModulesListModel.h
    ModulesListModel.cpp
    main.cpp
)

target_link_libraries(dataflow Qt5::Widgets)

source_group("Resources" "\\.qrc$")
