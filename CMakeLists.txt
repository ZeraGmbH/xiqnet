cmake_minimum_required(VERSION 3.1)

project(xiqnet LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

option(useSystemd "Using libsystemd" ON)

set(xiqnet_VERSION_MAJOR "1")
set(xiqnet_VERSION_MINOR "3")
set(xiqnet_VERSION_PATCH "0")
set(PROJECT_VERSION "${xiqnet_VERSION_MAJOR}.${xiqnet_VERSION_MINOR}.${xiqnet_VERSION_PATCH}")

include(FeatureSummary)
include(GNUInstallDirs)

find_package(Qt5 COMPONENTS Core Network CONFIG REQUIRED)
find_package(Protobuf REQUIRED)
find_package(zeraprotobuf REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Systemd REQUIRED libsystemd)

file(GLOB SOURCES 
    ${PROJECT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${PROJECT_SOURCE_DIR}/*.h
    )

add_library(xiqnet SHARED
    ${SOURCES}
    ${HEADER}
    )

target_link_libraries(xiqnet 
    PUBLIC
    Qt5::Core
    Qt5::Network
    ${Protobuf_LIBRARIES}   
    VeinMeta::zeraprotobuf
    )

if(${Systemd_FOUND})
    target_link_libraries(xiqnet 
        PRIVATE
        systemd
    )
endif()

target_include_directories(xiqnet
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/xiqnet>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

set_target_properties(xiqnet PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(xiqnet PROPERTIES SOVERSION ${xiqnet_VERSION_MAJOR})

include(GenerateExportHeader)
generate_export_header(xiqnet)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/xiqnet_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xiqnet
    )

install(TARGETS xiqnet
    EXPORT xiqnetExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    xiqnetConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMinorVersion
    )

configure_file(xiqnetConfig.cmake.in xiqnetConfig.cmake @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/xiqnetConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/xiqnetConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xiqnet
    )

install(EXPORT xiqnetExport
    FILE xiqnetTargets.cmake
    NAMESPACE VeinMeta::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xiqnet
    )

install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xiqnet
    )

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
