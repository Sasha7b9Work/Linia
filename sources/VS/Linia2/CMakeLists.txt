# 2023/07/04 15:47:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (Linia2)

#set(CMAKE_UNITY_BUILD OFF)

if(WIN32)

    include_directories(
        src
        arm64_plugs
        linux_plugs
        ../../ThirdParty/wxWidgets/include
        ../../ThirdParty/wxWidgets/generated/lib/vc_lib/mswud
    )

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall -MT -std:c++20")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /STACK:10000000")

    add_compile_options(
        # Обязательные
        /wd4265    # class has virtual functions, but its non-trivial destructor is not virtual
        /wd4514    # unreferenced inline function has been removed
        /wd4625    # copy constructor was implicitly defined as deleted
        /wd4623    # default constructor was implicitly defined as deleted
        /wd4626    # assignment operator was implicitly defined as deleted
        /wd4710    # function not inlined
        /wd4711    # function xxx selected for automatic inline expansion
        /wd4820    # 'N' bytes padding added after data member
        /wd5026    # move constructor was implicitly defined as deleted
        /wd5027    # move assignment operator was implicitly defined as deleted
        /wd5045    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
        /wd5039    # pointer or reference to potentially throwing function passed to extern C function under -EHc. Undefined behavior may occur if this function throws an exception
    )

else()

    include_directories(
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    else()
        arm64_plugs
    endif()
        ./src
        ../../ThirdParty/wxWidgets/generated/lib/wx/include/gtk3-unicode-static-3.2
        ../../ThirdParty/wxWidgets/include
    )

    link_directories(
        ../../ThirdParty/wxWidgets/generated/lib
    )

    add_definitions(-D__WXGTK__ -D__WXGTK3__ -D_FILE_OFFSET_BITS=64)

endif()

add_definitions(-DOPT_GENERIC)

file(GLOB RESOURCES                     resources/*.*)
file(GLOB DOC                           doc/*.*)
file(GLOB SRC                           src/*.*)
file(GLOB COMMUNICATOR                  src/Communicator/*.*)
file(GLOB COMMUNICATOR_COMPORT          src/Communicator/ComPort/*.*)
file(GLOB COMMUNICATOR_GPIO             src/Communicator/GPIO/*.*)
file(GLOB COMMUNICATOR_SPI              src/Communicator/SPI/*.*)
file(GLOB COMMUNICATOR_UART             src/Communicator/UART/*.*)
file(GLOB CONTROLS                      src/Controls/*.*)
file(GLOB DEVICE                        src/Device/*.*)
file(GLOB DEVICETEST                    src/DeviceTest/*.*)
file(GLOB PANELS                        src/Panels/*.*)
file(GLOB PANELS_PANEL3                 src/Panels/Panel03_Config/*.*)
file(GLOB PANELS_PANEL3_SHCEME          src/Panels/Panel03_Config/PanelScheme/*.*)
file(GLOB PANELS_PANEL5                 src/Panels/Panel05_Graph/*.*)
file(GLOB PANELS_PANEL11                src/Panels/Panel11_Menu/*.*)
file(GLOB PANELS_PANEL11_SETUP          src/Panels/Panel11_Menu/Setup/*.*)
file(GLOB PANELS_PANEL12                src/Panels/Panel12_Debug/*.*)
file(GLOB PANELS_PANEL12_PAGESCHIPS     src/Panels/Panel12_Debug/PagesChips/*.*)
file(GLOB PANELS_PANEL12_OLD            src/Panels/Panel12_Debug/Old/*.*)
file(GLOB SETTINGS                      src/Settings/*.*)
file(GLOB STORAGE                       src/Storage/*.*)
file(GLOB TESTS                         src/Tests/*.*)
file(GLOB UTILS                         src/Utils/*.*)
file(GLOB WINDOWS                       src/Windows/*.*)

source_group(doc                                  FILES ${DOC})
source_group(Resources                            FILES ${RESOURCES})
source_group(src                                  FILES ${SRC})
source_group(Communicator                         FILES ${COMMUNICATOR})
source_group(Communicator/ComPort                 FILES ${COMMUNICATOR_COMPORT})
source_group(Communicator/GPIO                    FILES ${COMMUNICATOR_GPIO})
source_group(Communicator/SPI                     FILES ${COMMUNICATOR_SPI})
source_group(Communicator/UART                    FILES ${COMMUNICATOR_UART})
source_group(Controls                             FILES ${CONTROLS})
source_group(Device                               FILES ${DEVICE})
source_group(DeviceTest                           FILES ${DEVICETEST})
source_group(Panels                               FILES ${PANELS})
source_group(Panels/Panel03_Config                FILES ${PANELS_PANEL3})
source_group(Panels/Panel03_Config/PanelScheme    FILES ${PANELS_PANEL3_SHCEME})
source_group(Panels/Panel05_Graph                 FILES ${PANELS_PANEL5})
source_group(Panels/Panel11_Menu                  FILES ${PANELS_PANEL11})
source_group(Panels/Panel11_Menu/Setup            FILES ${PANELS_PANEL11_SETUP})
source_group(Panels/Panel12_Debug                 FILES ${PANELS_PANEL12})
source_group(Panels/Panel12_Debug/PagesChips      FILES ${PANELS_PANEL12_PAGESCHIPS})
source_group(Panels/Panel12_Debug/Old             FILES ${PANELS_PANEL12_OLD})
source_group(Settings                             FILES ${SETTINGS})
source_group(Storage                              FILES ${STORAGE})
source_group(Tests                                FILES ${TESTS})
source_group(Utils                                FILES ${UTILS})
source_group(Windows                              FILES ${WINDOWS})

add_executable(${PROJECT_NAME}
    ${DOC}
    ${RESOURCES}
    ${SRC}
    ${COMMUNICATOR}
    ${COMMUNICATOR_COMPORT}
    ${COMMUNICATOR_GPIO}
    ${COMMUNICATOR_SPI}
    ${COMMUNICATOR_UART}
    ${CONTROLS}
    ${DEVICE}
    ${DEVICETEST}
    ${PANELS}
    ${PANELS_PANEL3}
    ${PANELS_PANEL3_SHCEME}
    ${PANELS_PANEL5}
    ${PANELS_PANEL11}
    ${PANELS_PANEL11_SETUP}
    ${PANELS_PANEL12}
    ${PANELS_PANEL12_PAGESCHIPS}
    ${PANELS_PANEL12_OLD}
    ${SETTINGS}
    ${STORAGE}
    ${TESTS}
    ${UTILS}
    ${WINDOWS}
)


if(WIN32)
    target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_lib/wxbase32u)
    target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_lib/wxbase32u_net)
    target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_lib/wxmsw32u_core)
    target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_lib/wxmsw32u_aui)
    target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_lib/wxzlib)
    target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_lib/wxregexu)
    target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_lib/wxpng)

    target_link_libraries(${PROJECT_NAME} WinMM.lib)
    target_link_libraries(${PROJECT_NAME} Ws2_32.lib)
    target_link_libraries(${PROJECT_NAME} version.lib)
    target_link_libraries(${PROJECT_NAME} Imm32.Lib)
    target_link_libraries(${PROJECT_NAME} Setupapi.lib)
    target_link_libraries(${PROJECT_NAME} Comctl32.lib)
    target_link_libraries(${PROJECT_NAME} Rpcrt4.lib)

    # Копируем в рабочий каталог
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND copy $(OutputPath)\\$(PROJECT_NAME) . /Y)
else()
    # Поиск необходимых библиотек для Linux
    find_package(PkgConfig REQUIRED)
    
    # Поиск GTK3
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    
    # Поиск GSpell (для проверки орфографии в wxWidgets)
    pkg_check_modules(GSPELL gspell-1)
    
    # Поиск других библиотек
    find_library(X11_LIBRARY X11 REQUIRED)
    find_library(XKBCOMMON_LIBRARY xkbcommon REQUIRED)
    find_library(PNG_LIBRARY png REQUIRED)
    find_library(DL_LIBRARY dl REQUIRED)

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
        message(STATUS "ARM64 platform detected - libgpiod is required")
        pkg_check_modules(GPIOD REQUIRED libgpiod)
        
        if(NOT GPIOD_FOUND)
            message(FATAL_ERROR "libgpiod is required for ARM64 builds but was not found! Install with: sudo apt-get install libgpiod-dev")
        endif()
        
        target_compile_definitions(${PROJECT_NAME} PRIVATE ARM64)
        target_link_libraries(${PROJECT_NAME} ${GPIOD_LIBRARIES})
        target_include_directories(${PROJECT_NAME} PRIVATE ${GPIOD_INCLUDE_DIRS})
        target_compile_options(${PROJECT_NAME} PRIVATE ${GPIOD_CFLAGS_OTHER})
        message(STATUS "libgpiod library configured for ARM64: ${GPIOD_LIBRARIES}")
    else()
        message(STATUS "Non-ARM64 platform detected (${CMAKE_SYSTEM_PROCESSOR}) - skipping libgpiod search")
    endif()

    target_link_libraries(${PROJECT_NAME}
        wx_gtk3u_core-3.2
        ${GTK3_LIBRARIES}
        pthread
        ${X11_LIBRARY}
        ${XKBCOMMON_LIBRARY}
        ${PNG_LIBRARY}
        wx_baseu-3.2
        ${DL_LIBRARY}
    )

    # Добавляем GSpell если найдена
    if(GSPELL_FOUND)
        target_link_libraries(${PROJECT_NAME} ${GSPELL_LIBRARIES})
        target_include_directories(${PROJECT_NAME} PRIVATE ${GSPELL_INCLUDE_DIRS})
        target_compile_options(${PROJECT_NAME} PRIVATE ${GSPELL_CFLAGS_OTHER})
        message(STATUS "GSpell library found: ${GSPELL_LIBRARIES}")
    else()
        message(WARNING "GSpell library not found - spell checking may not work")
    endif()

    # Добавляем include директории GTK3
    target_include_directories(${PROJECT_NAME} PRIVATE ${GTK3_INCLUDE_DIRS})
    target_compile_options(${PROJECT_NAME} PRIVATE ${GTK3_CFLAGS_OTHER})
endif()
