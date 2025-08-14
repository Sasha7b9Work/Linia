# 2023/07/04 15:47:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (Linia2)

set(CMAKE_UNITY_BUILD OFF)

if(WIN32)

    include_directories(
        src
        ../../ThirdParty/wxWidgets/include
        ../../ThirdParty/wxWidgets/generated/lib/vc_lib/mswud
        src/ThirdParty/libmpg123/src/libmpg123/
        src/ThirdParty/libmpg123/config/win/x64/
    )

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall -MT -std:c++20")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /STACK:10000000")

    add_compile_options(
        # Обязательные
        /wd4265    # class has virtual functions, but its non-trivial destructor is not virtual
        /wd4514    # unreferenced inline function has been removed
        /wd4625    # copy constructor was implicitly defined as deleted
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
file(GLOB COMMUNICATOR_SPI              src/Communicator/SPI/*.*)
file(GLOB CONTROLS                      src/Controls/*.*)
file(GLOB DEVICE                        src/Device/*.*)
file(GLOB PANELS                        src/Panels/*.*)
file(GLOB PANELS_PANEL3                 src/Panels/Panel03_Config/*.*)
file(GLOB PANELS_PANEL3_CHAN_B          src/Panels/Panel03_Config/PanelChannelB/*.*)
file(GLOB PANELS_PANEL3_CHAN_C          src/Panels/Panel03_Config/PanelChannelC/*.*)
file(GLOB PANELS_PANEL3_SHCEME          src/Panels/Panel03_Config/PanelScheme/*.*)
file(GLOB PANELS_PANEL5                 src/Panels/Panel05_Graph/*.*)
file(GLOB PANELS_PANEL11                src/Panels/Panel11_Menu/*.*)
file(GLOB PANELS_PANEL11_SETUP          src/Panels/Panel11_Menu/Setup/*.*)
file(GLOB PANELS_PANEL12                src/Panels/Panel12_Debug/*.*)
file(GLOB SETTINGS                      src/Settings/*.*)
file(GLOB UTILS                         src/Utils/*.*)
file(GLOB WINDOWS                       src/Windows/*.*)

source_group(doc                                 FILES ${DOC})
source_group(Resources                           FILES ${RESOURCES})
source_group(src                                 FILES ${SRC})
source_group(Communicator                        FILES ${COMMUNICATOR})
source_group(Communicator/ComPort                FILES ${COMMUNICATOR_COMPORT})
source_group(Communicator/SPI                    FILES ${COMMUNICATOR_SPI})
source_group(Controls                            FILES ${CONTROLS})
source_group(Device                              FILES ${DEVICE})
source_group(Panels                              FILES ${PANELS})
source_group(Panels/Panel03_Config               FILES ${PANELS_PANEL3})
source_group(Panels/Panel03_Config/PanelChannelB FILES ${PANELS_PANEL3_CHAN_B})
source_group(Panels/Panel03_Config/PanelChannelC FILES ${PANELS_PANEL3_CHAN_C})
source_group(Panels/Panel03_Config/PanelScheme   FILES ${PANELS_PANEL3_SHCEME})
source_group(Panels/Panel05_Graph                FILES ${PANELS_PANEL5})
source_group(Panels/Panel11_Menu                 FILES ${PANELS_PANEL11})
source_group(Panels/Panel11_Menu/Setup           FILES ${PANELS_PANEL11_SETUP})
source_group(Panels/Panel12_Debug                FILES ${PANELS_PANEL12})
source_group(Settings                            FILES ${SETTINGS})
source_group(Utils                               FILES ${UTILS})
source_group(Windows                             FILES ${WINDOWS})

add_executable(${PROJECT_NAME}
    ${DOC}
    ${RESOURCES}
    ${SRC}
    ${COMMUNICATOR}
    ${COMMUNICATOR_COMPORT}
    ${COMMUNICATOR_SPI}
    ${CONTROLS}
    ${DEVICE}
    ${PANELS}
    ${PANELS_PANEL3}
    ${PANELS_PANEL3_CHAN_B}
    ${PANELS_PANEL3_CHAN_C}
    ${PANELS_PANEL3_SHCEME}
    ${PANELS_PANEL5}
    ${PANELS_PANEL11}
    ${PANELS_PANEL11_SETUP}
    ${PANELS_PANEL12}
    ${SETTINGS}
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
    target_link_libraries(${PROJECT_NAME}
        wx_gtk3u_core-3.2
        gtk-3
        gdk-3
        pangocairo-1.0
        pango-1.0
        pthread
        cairo
        /usr/lib/x86_64-linux-gnu/libX11.so
        /usr/lib/x86_64-linux-gnu/libxkbcommon.so
        /usr/lib/x86_64-linux-gnu/libpng.so
        gdk_pixbuf-2.0
        wx_baseu-3.2
        gio-2.0
        gobject-2.0
        glib-2.0
        dl 
        )
endif()
