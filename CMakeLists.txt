project("OctreeVisualization")

cmake_minimum_required(VERSION 3.15)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories($(CMAKE_CURRENT_SOURCE_DIR)/source)

find_package(Qt5 COMPONENTS Widgets DataVisualization 3DCore 3DExtras Core 3DRender Gui 3DInput REQUIRED)

add_executable(octreeVisualization
    source/scenemodifier.cpp
    source/main.cpp
)

target_link_libraries(octreeVisualization Qt5::Widgets Qt5::DataVisualization Qt5::3DCore Qt5::3DExtras Qt5::Core Qt5::3DRender Qt5::Gui Qt5::3DInput)