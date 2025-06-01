# 2023/07/04 15:47:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
project (Linia2)

#set(CMAKE_UNITY_BUILD OFF)

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

        /wd4100
        /wd4002
        /wd4061
        /wd4068
        /wd4127
        /wd4141
        /wd4244
        /wd4255
        /wd4431
        /wd4456
        /wd4459
        /wd4701
        /wd4777
        /wd4996
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
file(GLOB CONTROLS                      src/Controls/*.*)
file(GLOB DATABASE                      src/DataBase/*.*)
file(GLOB DATABASE_SERVER               src/DataBase/Server/*.*)
file(GLOB PANELS                        src/Panels/*.*)
file(GLOB PANELS_DIALOGS                src/Panels/Dialogs/*.*)
file(GLOB PANELS_NOTEBOOK               src/Panels/Notebook/*.*)
file(GLOB PANELS_NOTEBOOK_PAGEMASTER    src/Panels/Notebook/PageMaster/*.*)
file(GLOB PANELS_NOTEBOOK_PAGEUSER      src/Panels/Notebook/PageUser/*.*)
file(GLOB READER                        src/Reader/*.*)
file(GLOB SETTINGS                      src/Settings/*.*)
file(GLOB UTILS                         src/Utils/*.*)
file(GLOB THIRDPARTY_LIBMPG123          src/ThirdParty/libmpg123/src/libmpg123/libmpg123.c
                                        src/ThirdParty/libmpg123/src/libmpg123/compat.c
                                        src/ThirdParty/libmpg123/src/libmpg123/dct64.c
                                        src/ThirdParty/libmpg123/src/libmpg123/equalizer.c
                                        src/ThirdParty/libmpg123/src/libmpg123/format.c
                                        src/ThirdParty/libmpg123/src/libmpg123/frame.c
                                        src/ThirdParty/libmpg123/src/libmpg123/icy.c
                                        src/ThirdParty/libmpg123/src/libmpg123/icy2utf8.c
                                        src/ThirdParty/libmpg123/src/libmpg123/index.c
                                        src/ThirdParty/libmpg123/src/libmpg123/id3.c
                                        src/ThirdParty/libmpg123/src/libmpg123/layer1.c
                                        src/ThirdParty/libmpg123/src/libmpg123/layer2.c
                                        src/ThirdParty/libmpg123/src/libmpg123/layer3.c
                                        src/ThirdParty/libmpg123/src/libmpg123/optimize.c
                                        src/ThirdParty/libmpg123/src/libmpg123/ntom.c
                                        src/ThirdParty/libmpg123/src/libmpg123/parse.c
                                        src/ThirdParty/libmpg123/src/libmpg123/readers.c
                                        src/ThirdParty/libmpg123/src/libmpg123/stringbuf.c
                                        src/ThirdParty/libmpg123/src/libmpg123/synth.c
                                        src/ThirdParty/libmpg123/src/libmpg123/synth_8bit.c
                                        src/ThirdParty/libmpg123/src/libmpg123/synth_real.c
                                        src/ThirdParty/libmpg123/src/libmpg123/synth_s32.c
                                        src/ThirdParty/libmpg123/src/libmpg123/tabinit.c)

source_group(doc                        FILES ${DOC})
source_group(Resources                  FILES ${RESOURCES})
source_group(src                        FILES ${SRC})
source_group(Communicator               FILES ${COMMUNICATOR})
source_group(Controls                   FILES ${CONTROLS})
source_group(DataBase                   FILES ${DATABASE})
source_group(DataBase/Server            FILES ${DATABASE_SERVER})
source_group(Panels                     FILES ${PANELS})
source_group(Panels/Dialogs             FILES ${PANELS_DIALOGS})
source_group(Panels/Notebook            FILES ${PANELS_NOTEBOOK})
source_group(Panels/Notebook/PageMaster FILES ${PANELS_NOTEBOOK_PAGEMASTER})
source_group(Panels/Notebook/PageUser   FILES ${PANELS_NOTEBOOK_PAGEUSER})
source_group(Reader                     FILES ${READER})
source_group(Settings                   FILES ${SETTINGS})
source_group(Utils                      FILES ${UTILS})
source_group(ThirdParty/libmpg123       FILES ${THIRDPARTY_LIBMPG123})

add_executable(${PROJECT_NAME}
    ${DOC}
    ${RESOURCES}
    ${SRC}
    ${COMMUNICATOR}
    ${CONTROLS}
    ${DATABASE}
    ${DATABASE_SERVER}
    ${PANELS}
    ${PANELS_DIALOGS}
    ${PANELS_NOTEBOOK}
    ${PANELS_NOTEBOOK_PAGEMASTER}
    ${PANELS_NOTEBOOK_PAGEUSER}
    ${READER}
    ${SETTINGS}
    ${UTILS}
    ${THIRDPARTY_LIBMPG123}
)


if(WIN32)
    target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_lib/wxbase32u)
    target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_lib/wxbase32u_net)
    target_link_libraries(${PROJECT_NAME} ../../../ThirdParty/wxWidgets/generated/lib/vc_lib/wxmsw32u_core)
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
