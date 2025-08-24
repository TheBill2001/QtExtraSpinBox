# SPDX-FileCopyrightText: 2025 Trần Nam Tuấn <tuantran1632001@gmail.com>
# SPDX-License-Identifier: GPL-3.0-only

function(QtExtraSpinBoxes_create_library arg_TARGET)
    string(TOUPPER ${arg_TARGET} ${arg_TARGET}_UPPERCASE)

    set(options STATIC SHARED EXPORT_PRIVATE)
    set(oneValueArgs VERSION VERSION_MAJOR VERSION_MINOR VERSION_PATCH)
    set(multiValueArgs
        SOURCES
        HEADERS
        INCLUDE_DIRECTORIES
        COMPILE_DEFINITIONS
        COMPILE_OPTIONS
        LINK_LIBRARIES
    )

    cmake_parse_arguments(PARSE_ARGV 1 arg
        "${options}" "${oneValueArgs}" "${multiValueArgs}"
    )

    cmake_parse_arguments(arg_HEADERS "" "" "PRIVATE;PUBLIC" ${arg_HEADERS})

    cmake_parse_arguments(arg_INCLUDE_DIRECTORIES "" "" "PRIVATE;PUBLIC" ${arg_INCLUDE_DIRECTORIES})

    cmake_parse_arguments(arg_COMPILE_DEFINITIONS "" "" "PRIVATE;PUBLIC" ${arg_COMPILE_DEFINITIONS})

    cmake_parse_arguments(arg_COMPILE_OPTIONS "" "" "PRIVATE;PUBLIC" ${arg_COMPILE_OPTIONS})

    if(arg_VERSION)
        string(REPLACE "." ";" VERSION_LIST ${arg_VERSION})
        list(GET VERSION_LIST 0 arg_VERSION_MAJOR)
        list(GET VERSION_LIST 1 arg_VERSION_MINOR)
        list(GET VERSION_LIST 2 arg_VERSION_PATCH)
    endif()

    if(NOT DEFINED arg_VERSION_MAJOR OR NOT arg_VERSION_MAJOR)
        set(arg_VERSION_MAJOR 0)
    endif()

    if(NOT DEFINED arg_VERSION_MINOR OR NOT arg_VERSION_MINOR)
        set(arg_VERSION_MINOR 0)
    endif()

    if(NOT DEFINED arg_VERSION_PATCH OR NOT arg_VERSION_PATCH)
        set(arg_VERSION_PATCH 0)
    endif()

    set(arg_VERSION "${arg_VERSION_MAJOR}.${arg_VERSION_MINOR}.${arg_VERSION_PATCH}")

    list(APPEND arg_COMPILE_DEFINITIONS_PRIVATE
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
        list(APPEND arg_COMPILE_DEFINITIONS_PRIVATE QT_STRICT_ITERATORS)
    endif()

    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        list(APPEND arg_COMPILE_OPTIONS_PRIVATE
            -pedantic
            -Wzero-as-null-pointer-constant
            -Werror=undef
        )
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        list(APPEND arg_COMPILE_OPTIONS_PRIVATE -Wmissing-include-dirs)
    endif()

    if(arg_SHARED)
        qt_add_library(${arg_TARGET} SHARED
            ${arg_SOURCES}
            ${arg_HEADERS_PUBLIC}
            ${arg_HEADERS_PRIVATE}
        )

        add_library(${arg_TARGET}::${arg_TARGET} ALIAS ${arg_TARGET})

        if(NOT CYGWIN)
            set_target_properties(${arg_TARGET} PROPERTIES
                OUTPUT_NAME ${arg_TARGET}
                SOVERSION ${arg_VERSION_MAJOR}
                VERSION ${arg_VERSION}
            )
        endif()

        target_include_directories(${arg_TARGET}
            PUBLIC ${arg_INCLUDE_DIRECTORIES_PUBLIC}
            PRIVATE ${arg_INCLUDE_DIRECTORIES_PRIVATE}
        )

        target_compile_definitions(${arg_TARGET}
            PUBLIC
                ${arg_COMPILE_DEFINITIONS_PUBLIC}
            PRIVATE
                ${arg_TARGET}_LIBRARY
                ${arg_COMPILE_DEFINITIONS_PRIVATE}
        )

        if(arg_EXPORT_PRIVATE)
            target_compile_definitions(${arg_TARGET}
                PUBLIC ${arg_TARGET}_EXPORT_PRIVATE
            )
        endif()

        target_compile_options(${arg_TARGET}
            PUBLIC ${arg_COMPILE_OPTIONS_PUBLIC}
            PRIVATE ${arg_COMPILE_OPTIONS_PRIVATE}
        )

        target_link_libraries(${arg_TARGET}
            ${arg_LINK_LIBRARIES}
        )
    endif()
endfunction()
