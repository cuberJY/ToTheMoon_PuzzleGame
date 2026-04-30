cmake_minimum_required(VERSION 3.16)

project(ToTheMoonPuzzleGame VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Multimedia)

# 启用 AUTOMOC、AUTOUIC 和 AUTORCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 手动指定源文件
set(CORE_SOURCES
    core/src/LevelMode.cpp
    core/src/PlayerData.cpp
    core/src/PuzzleBoard.cpp
    core/src/RandomMode.cpp
)

set(GUI_SOURCES
    gui/src/BoardController.cpp
    gui/src/BoardView.cpp
    gui/src/ChooseDiff.cpp
    gui/src/ChooseLevel.cpp
    gui/src/Home.cpp
    gui/src/MainWindow.cpp
    gui/src/Tool.cpp
    gui/src/main_gui.cpp
)

# 收集头文件
set(CORE_HEADERS
    core/include/GameMode.h
    core/include/LevelMode.h
    core/include/PlayerData.h
    core/include/PuzzleBoard.h
    core/include/RandomMode.h
)

set(GUI_HEADERS
    gui/include/BoardController.h
    gui/include/BoardView.h
    gui/include/ChooseDiff.h
    gui/include/ChooseLevel.h
    gui/include/Home.h
    gui/include/MainWindow.h
    gui/include/Tool.h
)

# 收集UI文件
set(UI_FILES
    gui/src/BoardView.ui
    gui/src/ChooseDiff.ui
    gui/src/ChooseLevel.ui
    gui/src/Home.ui
)

# 收集资源文件
set(RESOURCE_FILES
    resources/resources.qrc
)

# 创建可执行文件
qt_add_executable(ToTheMoonPuzzleGame
    ${CORE_SOURCES}
    ${GUI_SOURCES}
    ${CORE_HEADERS}
    ${GUI_HEADERS}
    ${UI_FILES}
    ${RESOURCE_FILES}
)

if(WIN32)
    set_target_properties(ToTheMoonPuzzleGame PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

# 添加编译选项
if(MSVC)
    target_compile_options(ToTheMoonPuzzleGame PRIVATE /W4)
else()
    target_compile_options(ToTheMoonPuzzleGame PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_include_directories(ToTheMoonPuzzleGame PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(ToTheMoonPuzzleGame PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Multimedia
)