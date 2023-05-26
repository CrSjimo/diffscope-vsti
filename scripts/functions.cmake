macro(_ck_list_add_flatly _list)
    set(_temp_list)
    file(GLOB _temp_list ${ARGN})
    list(APPEND ${_list} ${_temp_list})
    unset(_temp_list)
endmacro()

macro(_ck_list_add_recursively _list)
    set(_temp_list)
    file(GLOB_RECURSE _temp_list ${ARGN})
    list(APPEND ${_list} ${_temp_list})
    unset(_temp_list)
endmacro()

function(ck_get_subdirs _var)
    set(options ABSOLUTE)
    set(oneValueArgs DIRECTORY RELATIVE)
    set(multiValueArgs EXCLUDE REGEX_EXLCUDE)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(FUNC_DIRECTORY)
        get_filename_component(_dir ${FUNC_DIRECTORY} ABSOLUTE)
    else()
        set(_dir ${CMAKE_CURRENT_SOURCE_DIR})
    endif()

    file(GLOB _subdirs LIST_DIRECTORIES true RELATIVE ${_dir} "${_dir}/*")

    if(FUNC_EXCLUDE)
        foreach(_exclude_dir ${FUNC_EXCLUDE})
            list(REMOVE_ITEM _subdirs ${_exclude_dir})
        endforeach()
    endif()

    if(FUNC_REGEX_INCLUDE)
        foreach(_exp ${FUNC_REGEX_INCLUDE})
            list(FILTER _subdirs INCLUDE REGEX ${_exp})
        endforeach()
    endif()

    if(FUNC_REGEX_EXCLUDE)
        foreach(_exp ${FUNC_REGEX_EXCLUDE})
            list(FILTER _subdirs EXCLUDE REGEX ${_exp})
        endforeach()
    endif()

    set(_res)

    if(FUNC_RELATIVE)
        get_filename_component(_relative ${FUNC_RELATIVE} ABSOLUTE)
    else()
        set(_relative)
    endif()

    foreach(_sub ${_subdirs})
        if(IS_DIRECTORY ${_dir}/${_sub})
            if(FUNC_ABSOLUTE)
                list(APPEND _res ${_dir}/${_sub})
            elseif(_relative)
                file(RELATIVE_PATH _rel_path ${_relative} ${_dir}/${_sub})
                list(APPEND _res ${_rel_path})
            else()
                list(APPEND _res ${_sub})
            endif()
        endif()
    endforeach()

    set(${_var} ${_res} PARENT_SCOPE)
endfunction()

function(ck_add_files _var)
    set(options CURRENT SUBDIRS ALLDIRS)
    set(oneValueArgs)
    set(multiValueArgs PATTERNS DIRS)
    cmake_parse_arguments(FUNC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT FUNC_PATTERNS)
        message(FATAL_ERROR "ck_add_files: PATTERNS not specified!")
    endif()

    set(_src)

    # Add current dir
    if(FUNC_CURRENT OR FUNC_ALLDIRS)
        set(_tmp_patterns)

        foreach(_pat ${FUNC_PATTERNS})
            list(APPEND _tmp_patterns ${CMAKE_CURRENT_SOURCE_DIR}/${_pat})
        endforeach()

        _ck_list_add_flatly(_src ${_tmp_patterns})
        unset(_tmp_patterns)
    endif()

    # Add sub dirs
    if(FUNC_SUBDIRS OR FUNC_ALLDIRS)
        ck_get_subdirs(_subdirs ABSOLUTE)
        set(_tmp_patterns)

        foreach(_subdir ${_subdirs})
            foreach(_pat ${FUNC_PATTERNS})
                list(APPEND _tmp_patterns ${_subdir}/${_pat})
            endforeach()
        endforeach()

        _ck_list_add_recursively(_src ${_tmp_patterns})
        unset(_tmp_patterns)
    endif()

    # Add other dirs recursively
    foreach(_dir ${FUNC_DIRS})
        set(_tmp_patterns)

        foreach(_pat ${FUNC_PATTERNS})
            list(APPEND _tmp_patterns ${_dir}/${_pat})
        endforeach()

        _ck_list_add_recursively(_src ${_tmp_patterns})
        unset(_tmp_patterns)
    endforeach()

    if(FUNC_FILTER_PLATFORM)
        if(WIN32)
            list(FILTER _src EXCLUDE REGEX ".*_(Unix|unix|Mac|mac|Linux|linux)\\.+")
        elseif(APPLE)
            list(FILTER _src EXCLUDE REGEX ".*_(Win|win|Windows|windows|Linux|linux)\\.+")
        else()
            list(FILTER _src EXCLUDE REGEX ".*_(Win|win|Windows|windows|Mac|mac)\\.+")
        endif()
    endif()

    set(${_var} ${_src} PARENT_SCOPE)
endfunction()