cmake_minimum_required(VERSION 3.1)

#set project name and language
project(xiqnet LANGUAGES CXX)

#Automoc and RCC ON. Needed in QT projects
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
#C++ 11 Flag
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


option(useSystemd "Using libsystemd" ON)

#Set project Version
set(xiqnet_VERSION_MAJOR "1")
set(xiqnet_VERSION_MINOR "3")
set(xiqnet_VERSION_PATCH "0")

set(PROJECT_VERSION "${xiqnet_VERSION_MAJOR}.${xiqnet_VERSION_MINOR}.${xiqnet_VERSION_PATCH}")

#Adding cmake functions like feature_summary()
include(FeatureSummary)
include(GNUInstallDirs)

#Find dependecies
find_package(Qt5 COMPONENTS Core Network CONFIG REQUIRED)
find_package(Protobuf REQUIRED)
find_package(zeraprotobuf REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(Systemd REQUIRED libsystemd)

#sum up project Files 
file(GLOB SOURCES 
    ${PROJECT_SOURCE_DIR}/*.cpp
    )

file(GLOB HEADER
    ${PROJECT_SOURCE_DIR}/*.h
    )

file(GLOB RESOURCES 
    ${PROJECT_SOURCE_DIR}/*.qrc
    )

#create library 
add_library(xiqnet SHARED
    ${SOURCES}
    ${HEADER}
    ${RESOURCES}
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


# announce headers - target perspective
target_include_directories(xiqnet
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/xiqnet>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

#set target Version
set_target_properties(xiqnet PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(xiqnet PROPERTIES SOVERSION ${xiqnet_VERSION_MAJOR})

message(STATUS "Project Version: " ${PROJECT_VERSION})

#generate export header
include(GenerateExportHeader)
generate_export_header(xiqnet)

#install export header
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/xiqnet_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xiqnet
    )



# install library
install(TARGETS xiqnet
    EXPORT xiqnetExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )

# configure *ConfigVersion.cmake
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    xiqnetConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMinorVersion
    )

# configure *Config.cmake
configure_file(xiqnetConfig.cmake.in xiqnetConfig.cmake @ONLY)

message(STATUS "BINDIR: " ${CMAKE_CURRENT_BINARY_DIR})

# install *Config(Version).cmake
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/xiqnetConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/xiqnetConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xiqnet
    )

# install targets cmake-files
install(EXPORT xiqnetExport
    FILE xiqnetTargets.cmake
    NAMESPACE VeinMeta::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/xiqnet
    )

# install public headers
install(
    FILES ${HEADER}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xiqnet
    )

# spawn out some info on configuration
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)



