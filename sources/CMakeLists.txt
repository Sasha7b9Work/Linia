cmake_minimum_required(VERSION 3.15)

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

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")         # Установка флага линковки /MT

if(WIN32)
else()
    add_definitions(
        -Wall                   # много стандартных предупреждений
        -Wextra                 # дополнительные предупреждения
        -pedantic               # строгое соотвествие стандарту C++
        -Werror                 # все предупреждения считать ошибками
        -fmax-errors=5
        -Wno-unknown-pragmas
        -pipe                   # увеличение скорости компляции вследствие избавления от промежуточных файлов  
        -std=c++23
        -march=native           # генерирует код, оптимизированный под процессор, на котором копилируется
        -fno-stack-protector    # отключение защиты стека

#  Без оптимизации - лучше для отладки
#      -g3           # максимальная отладочная информация
#     -O0           # оптимизация отключена

# Максимальная оптимизация - релизная версия
       -O3         # максимальный уровень оптимизации
       -flto       #  включает Link-Time Optimization (оптимизацию на этапе линковки). Уменьшает время компиляции
       -s          # -strip-all удалить все таблицы символов из финального исполняемого файла
       -g0         # не создавать отладочную информацию (по умолчанию)
       -flto=8     # link time optimization
       
    )
endif()


#add_subdirectory(Linia)
add_subdirectory(Linia2)
add_subdirectory(ServerHTTP)

if(WIN32)
    add_subdirectory(Device)
else()
endif()

