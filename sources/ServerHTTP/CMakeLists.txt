cmake_minimum_required(VERSION 3.10)
project(HttpLogServer)

set(CMAKE_UNITY_BUILD OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определение платформы
if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0600)
    set(PLATFORM_LIBS ws2_32)
    message(STATUS "Building for Windows")
else()
    set(PLATFORM_LIBS pthread)
    message(STATUS "Building for Linux")
endif()

# Поиск потоков
find_package(Threads REQUIRED)

# Исполняемый файл сервера
add_executable(log_server
    main.cpp
    ServerHTTP.h
    ServerHTTP.cpp
)

# Подключение библиотек
target_link_libraries(log_server
    PRIVATE
    Threads::Threads
    ${PLATFORM_LIBS}
)

# Установка свойств
set_target_properties(log_server PROPERTIES
    OUTPUT_NAME "log_server"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Вывод информации
message(STATUS "Output: ${CMAKE_BINARY_DIR}/bin/log_server")
