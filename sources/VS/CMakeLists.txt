cmake_minimum_required(VERSION 3.10)

# Set CMake policy required by UrhoCommon module
if (COMMAND cmake_policy)
    # Libraries linked via full path no longer produce linker search paths
    cmake_policy (SET CMP0003 NEW)
    # INTERFACE_LINK_LIBRARIES defines the link interface
    cmake_policy (SET CMP0022 NEW)
#    # Disallow use of the LOCATION target property - so we set to OLD as we still need it
#    cmake_policy (SET CMP0026 OLD)
    # MACOSX_RPATH is enabled by default
    cmake_policy (SET CMP0042 NEW)
#    # Honor the visibility properties for SHARED target types only
#    cmake_policy (SET CMP0063 OLD)
endif ()

# Set project name
project (Linia2-GUI)

set (TARGET_NAME Linia2-GUI)

set(CMAKE_UNITY_BUILD ON)
#set(CMAKE_UNITY_BUILD_BATCH_SIZE 32)

# Older cmake versions don't support string(APPEND ...) provide a workaround
macro(wx_string_append var str)
    set(${var} ${${var}}${str})
endmacro()

#if(BUILD_USE_STATIC_RUNTIME)
    set(MSVC_LIB_USE "/MT")
    set(MSVC_LIB_REPLACE "/MDd")
    set(MSVC_LIB_REPLACE_2 "/MD")
 
    # Set MSVC runtime flags for all configurations
    foreach(cfg "" ${CMAKE_CONFIGURATION_TYPES})
        set(c_flag_var CMAKE_C_FLAGS)
        set(cxx_flag_var CMAKE_CXX_FLAGS)
        if(cfg)
            string(TOUPPER ${cfg} cfg_upper)
            wx_string_append(c_flag_var "_${cfg_upper}")
            wx_string_append(cxx_flag_var "_${cfg_upper}")
        endif()
        if(${c_flag_var} MATCHES ${MSVC_LIB_REPLACE})
            string(REPLACE ${MSVC_LIB_REPLACE} ${MSVC_LIB_USE} ${c_flag_var} "${${c_flag_var}}")
            set(${c_flag_var} ${${c_flag_var}} CACHE STRING
            "Flags used by the C compiler during ${cfg_upper} builds." FORCE)
        endif()
        if(${cxx_flag_var} MATCHES ${MSVC_LIB_REPLACE})
            string(REPLACE ${MSVC_LIB_REPLACE} ${MSVC_LIB_USE} ${cxx_flag_var} "${${cxx_flag_var}}")
            set(${cxx_flag_var} ${${cxx_flag_var}} CACHE STRING
            "Flags used by the CXX compiler during ${cfg_upper} builds." FORCE)
        endif()
        if(${c_flag_var} MATCHES ${MSVC_LIB_REPLACE_2})
            string(REPLACE ${MSVC_LIB_REPLACE_2} ${MSVC_LIB_USE} ${c_flag_var} "${${c_flag_var}}")
            set(${c_flag_var} ${${c_flag_var}} CACHE STRING
            "Flags used by the C compiler during ${cfg_upper} builds." FORCE)
        endif()
        if(${cxx_flag_var} MATCHES ${MSVC_LIB_REPLACE_2})
            string(REPLACE ${MSVC_LIB_REPLACE_2} ${MSVC_LIB_USE} ${cxx_flag_var} "${${cxx_flag_var}}")
            set(${cxx_flag_var} ${${cxx_flag_var}} CACHE STRING
            "Flags used by the CXX compiler during ${cfg_upper} builds." FORCE)
        endif()

    endforeach()
#endif()


if(WIN32)
else()
    add_definitions(
        -Wall
        -Werror
        -fmax-errors=1
        -pthread
        -Wno-unknown-pragmas
        -Wno-write-strings
#        -g3                       # Без этого Code Blocks не останавливается на точках останова
        -Ofast
    )
endif()


if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    add_definitions(
        -march=armv8-a          # Для получения оптимального когда на Orange Pi 5 Plus (ARM64)
    )
endif()


add_subdirectory(Linia2)
