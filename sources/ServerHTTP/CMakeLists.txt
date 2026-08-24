cmake_minimum_required(VERSION 3.10)
project(HttpLogServer)

# Стандарт C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Опции сборки
option(BUILD_TESTS "Build tests" OFF)
option(ENABLE_DEBUG "Enable debug output" OFF)

# Проверка платформы
if(WIN32)
    message(FATAL_ERROR "This server is designed for Linux only. Use the client for cross-platform support.")
endif()

# Поиск необходимых библиотек
find_package(Threads REQUIRED)

# Настройка компилятора
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Предупреждения
    add_compile_options(-Wall -Wextra -Wpedantic)
    
    # Оптимизация
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O3)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
    endif()
    
    # Дополнительные флаги для отладки
    if(ENABLE_DEBUG)
        add_compile_definitions(DEBUG_MODE)
    endif()
endif()

# Исполняемый файл сервера
add_executable(log_server
    ServerHTTP.cpp
)

# Подключение библиотек
target_link_libraries(log_server
    PRIVATE
    Threads::Threads
)

# Установка свойств
set_target_properties(log_server PROPERTIES
    OUTPUT_NAME "log_server"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Установка (опционально)
install(TARGETS log_server
    RUNTIME DESTINATION bin
)

# Создание директории для логов при установке
install(DIRECTORY DESTINATION /var/log
    DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ
)

# Добавление тестов (если включены)
if(BUILD_TESTS)
    enable_testing()
    
    # Тест запуска сервера
    add_test(NAME server_starts
        COMMAND log_server -p 9999 -l /tmp/test_log.txt
    )
    set_tests_properties(server_starts PROPERTIES
        TIMEOUT 2
        PASS_REGULAR_EXPRESSION "Server started on port: 9999"
    )
endif()

# Вывод информации о сборке
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Output directory: ${CMAKE_BINARY_DIR}/bin")
