cmake_minimum_required(VERSION 3.26)
project(automation1)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)

set(CMAKE_CXX_STANDARD 20)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/TangoCMakeModules)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Tango REQUIRED)

include_directories(${TANGO_INCLUDE_DIRS})
include_directories(external/automation1/include)
link_directories(external/automation1/lib)

include_directories("include")

add_executable(automation1 main.cpp
        src/Axis.cpp
        src/AxisClass.cpp
        src/Controller.cpp
        src/ControllerClass.cpp
        src/MultiClassesFactory.cpp
        src/BissEncoder.cpp
        src/BissEncoderClass.cpp
)

target_link_libraries(automation1 Tango::Tango automation1c)


install(TARGETS automation1 DESTINATION bin)
install(FILES external/automation1/lib/libautomation1c.so external/automation1/lib/libautomation1compiler.so DESTINATION lib)

# Generate the package
include(Packing)