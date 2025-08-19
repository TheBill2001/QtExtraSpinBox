# SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: GPL-3.0-only

cmake_minimum_required(VERSION 3.23)

project(QtExtraSpinBox
    LANGUAGES CXX
    VERSION 1.0.0
    DESCRIPTION "Extra Spin Box for Qt Widgets"
    HOMEPAGE_URL "https://github.com/TheBill2001/mease"
)

option(${PROJECT_NAME}_BUILD_SHARED  "Enable building QtExtraSpinBox shared library" ON)
option(${PROJECT_NAME}_BUILD_STATIC  "Enable building QtExtraSpinBox static library" ON)
option(${PROJECT_NAME}_DEVEL         "Enable QtExtraSpinBox development mode" ${PROJECT_IS_TOP_LEVEL})
option(${PROJECT_NAME}_BUILD_EXAMPLE "Enable building QtExtraSpinBox tests" ${PROJECT_IS_TOP_LEVEL})

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT IS_MULTI)
    message(STATUS "No CMAKE_BUILD_TYPE set -- using Release")
    set(CMAKE_BUILD_TYPE Release)
endif()

set(KF_MIN_VERSION 6.14.0)
set(QT_MIN_VERSION 6.8.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CheckTypeSize)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

if(${PROJECT_NAME}_DEVEL)
    find_package(ECM ${KF_MIN_VERSION} REQUIRED)
    list(APPEND CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

    include(KDEClangFormat)
    include(KDEGitCommitHooks)
endif()

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS
    Core
    Gui
    Widgets
)

set(${PROJECT_NAME}_COMPILER_FLAGS)
set(${PROJECT_NAME}_COMPILER_PUBLIC_DEFS)
set(${PROJECT_NAME}_COMPILER_PRIVATE_DEFS
    QT_DISABLE_DEPRECATED_UP_TO=0x060800
    QT_NO_CAST_TO_ASCII
    QT_NO_CAST_FROM_ASCII
    QT_NO_URL_CAST_FROM_STRING
    QT_NO_CAST_FROM_BYTEARRAY
    QT_USE_QSTRINGBUILDER
    QT_NO_NARROWING_CONVERSIONS_IN_CONNECT
    QT_NO_KEYWORDS
    QT_NO_FOREACH
)

if(NOT WIN32)
    list(APPEND ${PROJECT_NAME}_COMPILER_DEFS QT_STRICT_ITERATORS)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    list(APPEND ${PROJECT_NAME}_COMPILER_FLAGS
        -pedantic
        -Wzero-as-null-pointer-constant
        -Werror=undef
    )
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list(APPEND ${PROJECT_NAME}_COMPILER_FLAGS -Wmissing-include-dirs)
endif()

check_type_size("int" SIZEOF_INT BUILTIN_TYPES_ONLY LANGUAGE CXX)
if(SIZEOF_INT EQUAL 32)
    list(APPEND ${PROJECT_NAME}_COMPILER_PUBLIC_DEFS ${PROJECT_NAME}_32_INT)
endif()

set(${PROJECT_NAME}_PUBLIC_HEADERS
)

set(${PROJECT_NAME}_PRIVATE_HEADERS
)

set(${PROJECT_NAME}_SOURCES
)

if(${PROJECT_NAME}_BUILD_SHARED)
    qt_add_library(${PROJECT_NAME} SHARED
        ${${PROJECT_NAME}_PUBLIC_HEADERS}
        ${${PROJECT_NAME}_PRIVATE_HEADERS}
        ${${PROJECT_NAME}_SOURCES}
    )

    add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

    target_include_directories(${PROJECT_NAME}
        PRIVATE
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
        PUBLIC
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    )

    if(NOT CYGWIN)
        set_target_properties(${PROJECT_NAME} PROPERTIES
                SOVERSION ${${PROJECT_NAME}_VERSION_MAJOR}
                VERSION ${${PROJECT_NAME}_VERSION}
        )
    endif()

    target_compile_definitions(${PROJECT_NAME}
        PUBLIC
            ${${PROJECT_NAME}_COMPILER_PUBLIC_DEFS}
        PRIVATE
            QTEXTRASPINBOX_LIBRARY
            ${${PROJECT_NAME}_COMPILER_PRIVATE_DEFS}
    )

    target_compile_options(${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_COMPILER_FLAGS})

    target_link_libraries(${PROJECT_NAME} PUBLIC Qt6::Widgets)
endif()

if(${PROJECT_NAME}_BUILD_STATIC)
    qt_add_library(${PROJECT_NAME}Static STATIC
        ${${PROJECT_NAME}_SOURCES}
        ${${PROJECT_NAME}_PRIVATE_HEADERS}
        ${${PROJECT_NAME}_PUBLIC_HEADERS}
    )

    add_library(${PROJECT_NAME}::${PROJECT_NAME}Static ALIAS ${PROJECT_NAME}Static)

    target_include_directories(${PROJECT_NAME}Static
        PRIVATE
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
        PUBLIC
            "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    )

    set_target_properties(${PROJECT_NAME}Static PROPERTIES
        EXPORT_NAME ${PROJECT_NAME}Static
        OUTPUT_NAME ${PROJECT_NAME}Static
    )

    target_compile_definitions(${PROJECT_NAME}Static
        PUBLIC
            QTEXTRASPINBOX_STATIC_DEFINE
            ${${PROJECT_NAME}_COMPILER_PUBLIC_DEFS}
        PRIVATE
            ${${PROJECT_NAME}_COMPILER_PRIVATE_DEFS}
        )

    target_compile_options(${PROJECT_NAME}Static PRIVATE ${${PROJECT_NAME}_COMPILER_FLAGS})

    target_link_libraries(${PROJECT_NAME}Static PUBLIC Qt6::Widgets)
endif()

if(${PROJECT_NAME}_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

if(PROJECT_IS_TOP_LEVEL)
    if(${PROJECT_NAME}_BUILD_SHARED)
        install(
            TARGETS ${PROJECT_NAME}
            COMPONENT Runtime
            EXPORT ${PROJECT_NAME}SharedExport
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )

        install(
            EXPORT ${PROJECT_NAME}SharedExport
            FILE ${PROJECT_NAME}-shared.cmake
            NAMESPACE ${PROJECT_NAME}::
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
        )

        if(MSVC)
            install(
                FILES $<TARGET_PDB_FILE:${PROJECT_NAME}>
                COMPONENT Runtime
                DESTINATION ${CMAKE_INSTALL_BINDIR}
                CONFIGURATIONS Debug OR RelWithDebInfo
                OPTIONAL
            )
        endif(MSVC)
    endif()

    if(${PROJECT_NAME}_BUILD_STATIC)
        install(
            TARGETS ${PROJECT_NAME}Static
            COMPONENT Development
            EXPORT ${PROJECT_NAME}StaticExport
            RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
            ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
            LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        )

        install(
            EXPORT ${PROJECT_NAME}StaticExport
            FILE ${PROJECT_NAME}-static.cmake
            NAMESPACE ${PROJECT_NAME}::
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
        )
    endif()

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        VERSION "${PROJECT_VERSION}"
        COMPATIBILITY AnyNewerVersion
    )

    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    )

    install(
        FILES ${${PROJECT_NAME}_PUBLIC_HEADERS}
        COMPONENT Development
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    )
endif()

if(${PROJECT_NAME}_DEVEL)
    file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES
        include/*.hpp
        src/*.cpp
        src/*.hpp
        example/*.hpp
        example/*.cpp
    )
    kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
    kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
endif()
