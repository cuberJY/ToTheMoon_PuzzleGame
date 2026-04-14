cmake_minimum_required(VERSION 3.16)

project(ToTheMoonPuzzleGame VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

qt_add_executable(ToTheMoonPuzzleGame
    core/src/main.cpp
    gui/src/main_gui.cpp
    core/src/ModeManager.cpp
    core/src/PuzzleBoard.cpp
    gui/src/MainWindow.cpp
    gui/src/board.cpp
    gui/src/choosediff.cpp
    gui/include/MainWindow.h
    gui/include/board.h
    gui/include/choosediff.h
    core/include/ModeManager.h
    core/include/PuzzleBoard.h
)

qt_wrap_ui(UI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/src/board.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/src/choosediff.ui
)

target_sources(ToTheMoonPuzzleGame PRIVATE
    ${UI_SOURCES}
)

set(UI_FILES_FOR_QTCREATOR
    gui/src/board.ui
    gui/src/choosediff.ui
)

target_sources(ToTheMoonPuzzleGame PRIVATE
    ${UI_FILES_FOR_QTCREATOR}
)

qt_add_resources(ToTheMoonPuzzleGame resources)

target_include_directories(ToTheMoonPuzzleGame PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(ToTheMoonPuzzleGame PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
)

set_target_properties(ToTheMoonPuzzleGame PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.github.ToTheMoonPuzzleGame
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)

install(TARGETS ToTheMoonPuzzleGame
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
