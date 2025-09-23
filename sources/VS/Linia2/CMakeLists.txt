# 2023/07/04 15:47:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (Linia2)

#set(CMAKE_UNITY_BUILD OFF)

set(CMAKE_UNITY_BUILD_BATCH_SIZE 18)

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

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
        include_directories(
            ./src
            ../../ThirdParty/wxWidgets/generated/lib/wx/include/gtk3-unicode-static-3.2
            ../../ThirdParty/wxWidgets/include
        )
    else()
        include_directories(
            ./arm64_plugs
            ./src
            ../../ThirdParty/wxWidgets/generated/lib/wx/include/gtk3-unicode-static-3.2
            ../../ThirdParty/wxWidgets/include
        )
    endif()

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
file(GLOB DATA                          src/Data/*.*)
file(GLOB DISPLAY                       src/Display/*.*)
file(GLOB IPPP                          src/IPPP/*.*)
file(GLOB IPPP_DEVICE                   src/IPPP/Device/*.*)
file(GLOB IPPP_TESTS                    src/IPPP/Tests/*.*)
file(GLOB PANELS                        src/Panels/*.*)
file(GLOB PANELS_PANELCONFIG            src/Panels/PanelConfig/*.*)
file(GLOB PANELS_PANELCONFIG_SCHEME     src/Panels/PanelConfig/PanelScheme/*.*)
file(GLOB PANELS_PANELMENU              src/Panels/PanelMenu/*.*)
file(GLOB PANELS_PANELMENU_SETUP        src/Panels/PanelMenu/Setup/*.*)
file(GLOB PANELS_PANELDEBUG             src/Panels/PanelDebug/*.*)
file(GLOB PANELS_PANELDEBUG_PAGESCHIPS  src/Panels/PanelDebug/PagesChips/*.*)
file(GLOB PANELS_PANELDEBUG_OLD         src/Panels/PanelDebug/Old/*.*)
file(GLOB SETTINGS                      src/Settings/*.*)
file(GLOB SOFTTESTS                     src/SoftTests/*.*)
file(GLOB STORAGE                       src/Storage/*.*)
file(GLOB UTILS                         src/Utils/*.*)
file(GLOB WINDOWS                       src/Windows/*.*)

if(WIN32)                                                           # Эти заглушки включаем для Windows
    file(GLOB PLUGS_ARM64               arm64_plugs/*.*
                                        arm64_plugs/linux/spi/*.*)
    file(GLOB PLUGS_LINUX               linux_plugs/*.*
                                        linux_plugs/sys/*.*)
else()
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    else()                                                          # Эти заглушки включаем для Ubuntu x64
        file(GLOB PLUGS_ARM64           arm64_plugs/*.*
                                        arm64_plugs/linux/spi/*.*)
    endif()
endif()

source_group(doc                                  FILES ${DOC})
source_group(Resources                            FILES ${RESOURCES})
source_group(src                                  FILES ${SRC})
source_group(Communicator                         FILES ${COMMUNICATOR})
source_group(Communicator/ComPort                 FILES ${COMMUNICATOR_COMPORT})
source_group(Communicator/GPIO                    FILES ${COMMUNICATOR_GPIO})
source_group(Communicator/SPI                     FILES ${COMMUNICATOR_SPI})
source_group(Communicator/UART                    FILES ${COMMUNICATOR_UART})
source_group(Controls                             FILES ${CONTROLS})
source_group(Data                                 FILES ${DATA})
source_group(Display                              FILES ${DISPLAY})
source_group(IPPP                                 FILES ${IPPP})
source_group(IPPP/Device                          FILES ${IPPP_DEVICE})
source_group(IPPP/Tests                           FILES ${IPPP_TESTS})
source_group(Panels                               FILES ${PANELS})
source_group(Panels/PanelConfig                   FILES ${PANELS_PANELCONFIG})
source_group(Panels/PanelConfig/PanelScheme       FILES ${PANELS_PANELCONFIG_SCHEME})
source_group(Panels/PanelMenu                     FILES ${PANELS_PANELMENU})
source_group(Panels/PanelMenu/Setup               FILES ${PANELS_PANELMENU_SETUP})
source_group(Panels/PanelDebug                    FILES ${PANELS_PANELDEBUG})
source_group(Panels/PanelDebug/PagesChips         FILES ${PANELS_PANELDEBUG_PAGESCHIPS})
source_group(Panels/PanelDebug/Old                FILES ${PANELS_PANELDEBUG_OLD})
source_group(Settings                             FILES ${SETTINGS})
source_group(SoftTests                            FILES ${SOFTTESTS})
source_group(Storage                              FILES ${STORAGE})
source_group(Utils                                FILES ${UTILS})
source_group(Windows                              FILES ${WINDOWS})
source_group(Plugs/Arm64                          FILES ${PLUGS_ARM64})
source_group(Plugs/linux                          FILES ${PLUGS_LINUX})

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
    ${DATA}
    ${DISPLAY}
    ${IPPP}
    ${IPPP_DEVICE}
    ${IPPP_TESTS}
    ${PANELS}
    ${PANELS_PANELCONFIG}
    ${PANELS_PANELCONFIG_SCHEME}
    ${PANELS_PANELMENU}
    ${PANELS_PANELMENU_SETUP}
    ${PANELS_PANELDEBUG}
    ${PANELS_PANELDEBUG_PAGESCHIPS}
    ${PANELS_PANELDEBUG_OLD}
    ${SETTINGS}
    ${SOFTTESTS}
    ${STORAGE}
    ${UTILS}
    ${WINDOWS}
    ${PLUGS_ARM64}
    ${PLUGS_LINUX}
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

    # Добавляем include директории GTK3
    target_include_directories(${PROJECT_NAME} PRIVATE ${GTK3_INCLUDE_DIRS})
    target_compile_options(${PROJECT_NAME} PRIVATE ${GTK3_CFLAGS_OTHER})
endif()

