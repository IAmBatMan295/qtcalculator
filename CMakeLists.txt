cmake_minimum_required(VERSION 3.16)

project(QtCalculator C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets QUIET)
if(Qt6_FOUND)
    message(STATUS "Found Qt6 Widgets")
    set(QT_LIB Qt6::Widgets)

    get_target_property(_opts Qt6::Core INTERFACE_COMPILE_OPTIONS)
    if(_opts)
        list(REMOVE_ITEM _opts "-mno-direct-extern-access")
        set_target_properties(Qt6::Core PROPERTIES INTERFACE_COMPILE_OPTIONS "${_opts}")
    endif()
else()
    find_package(Qt5 COMPONENTS Widgets REQUIRED)
    message(STATUS "Found Qt5 Widgets")
    set(QT_LIB Qt5::Widgets)
endif()

string(REPLACE "-mno-direct-extern-access" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REPLACE "-mno-direct-extern-access" "" CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}")

set(BUILD_MODE "MERGED" CACHE STRING "INDIVIDUAL = one exe per .c/.cpp | MERGED = single exe")
set_property(CACHE BUILD_MODE PROPERTY STRINGS INDIVIDUAL MERGED)

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/src")
    message(FATAL_ERROR "'src/' directory not found in project root.")
endif()

file(GLOB_RECURSE ALL_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.c"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

if(NOT ALL_SOURCES)
    message(FATAL_ERROR "No .c / .cpp files found under src/.")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(COMMON_WARN_FLAGS -Wall -Wextra -Wpedantic)

if(BUILD_MODE STREQUAL "INDIVIDUAL")
    message(STATUS "Build mode: INDIVIDUAL — one executable per source file")

    foreach(SRC IN LISTS ALL_SOURCES)
        get_filename_component(TARGET_NAME "${SRC}" NAME_WE)

        add_executable(${TARGET_NAME} "${SRC}")

        target_compile_options(${TARGET_NAME} PRIVATE ${COMMON_WARN_FLAGS})

        target_include_directories(${TARGET_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/src")

        target_link_libraries(${TARGET_NAME} PRIVATE ${QT_LIB})

        message(STATUS "  -> ${TARGET_NAME}")
    endforeach()

elseif(BUILD_MODE STREQUAL "MERGED")
    message(STATUS "Build mode: MERGED — single executable from all sources")

    add_executable(main ${ALL_SOURCES})

    target_compile_options(main PRIVATE ${COMMON_WARN_FLAGS})

    target_include_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/src")

    target_link_libraries(main PRIVATE ${QT_LIB})

else()
    message(FATAL_ERROR "Unknown BUILD_MODE '${BUILD_MODE}'. Use INDIVIDUAL or MERGED.")
endif()
