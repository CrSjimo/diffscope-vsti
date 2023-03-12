cmake_minimum_required(VERSION 3.14.0)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "")

# set(vst3sdk_SOURCE_DIR "C:/Users/Crs_1/VST_SDK/vst3sdk")
if(NOT vst3sdk_SOURCE_DIR)
    message(FATAL_ERROR "Path to VST3 SDK is empty!")
endif()

project(Diffscope_VSTi
    # This is your plug-in version number. Change it here only.
    # Version number symbols usable in C++ can be found in
    # source/version.h and ${PROJECT_BINARY_DIR}/projectversion.h.
    VERSION 1.0.0.0 
    DESCRIPTION "Diffscope_VSTi VST 3 Plug-in"
)

# Add libraries directory
add_subdirectory(libraries)

set(SMTG_VSTGUI_ROOT "${vst3sdk_SOURCE_DIR}")

add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(Diffscope_VSTi
    source/version.h
    source/myplugincids.h
    source/mypluginprocessor.h
    source/mypluginprocessor.cpp
    source/myplugincontroller.h
    source/myplugincontroller.cpp
    source/mypluginentry.cpp
    source/libraryloader.h
    source/libraryloader.cpp
)

target_link_libraries(Diffscope_VSTi PRIVATE loadso)

#- VSTGUI Wanted ----
if(SMTG_ADD_VSTGUI)
    target_sources(Diffscope_VSTi
        PRIVATE
            resource/myplugineditor.uidesc
    )
    target_link_libraries(Diffscope_VSTi
        PRIVATE
            vstgui_support
    )
    smtg_target_add_plugin_resources(Diffscope_VSTi
        RESOURCES
            "resource/myplugineditor.uidesc"
    )
endif(SMTG_ADD_VSTGUI)
# -------------------

smtg_target_add_plugin_snapshots (Diffscope_VSTi
    RESOURCES
        resource/A87D935EAD9951139D73465028C89294_snapshot.png
        resource/A87D935EAD9951139D73465028C89294_snapshot_2.0x.png
)

target_link_libraries(Diffscope_VSTi
    PRIVATE
        sdk
)

smtg_target_configure_version_file(Diffscope_VSTi)

if(SMTG_MAC)
    smtg_target_set_bundle(Diffscope_VSTi
        BUNDLE_IDENTIFIER dev.sjimo.diffscope.vsti
        COMPANY_NAME "OpenVpi Team"
    )
    smtg_target_set_debug_executable(Diffscope_VSTi
        "/Applications/VST3PluginTestHost.app"
        "--pluginfolder;$(BUILT_PRODUCTS_DIR)"
    )
elseif(SMTG_WIN)
    target_sources(Diffscope_VSTi PRIVATE 
        resource/win32resource.rc
    )
    if(MSVC)
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Diffscope_VSTi)

        smtg_target_set_debug_executable(Diffscope_VSTi
            "$(ProgramW6432)/Steinberg/VST3PluginTestHost/VST3PluginTestHost.exe"
            "--pluginfolder \"$(OutDir)/\""
        )
    endif()
endif(SMTG_MAC)
