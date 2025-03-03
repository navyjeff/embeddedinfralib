macro(protocol_buffer_cpp input)

    if(NOT CCOLA_COMPONENT_SKIP)
        ccola_make_absolute_from_source(absolute_input ${input})
        ccola_sources("${absolute_input}")
        ccola_set_source_group("Source Files" "${absolute_input}")
        ccola_get_filename_component_we(source_base ${input})

        get_filename_component(generated_dir_cpp "${CMAKE_CURRENT_BINARY_DIR}/generated/proto_cpp" ABSOLUTE)

        set(protopath)
        foreach(dependency ${PROTOBUF_DEPENDENCIES})
            ccola_read_public_property("${dependency}" proto paths)
            foreach(path ${paths})
                list(APPEND protopath --proto_path ${path})
            endforeach()
        endforeach()

        foreach(dependency_path "${CMAKE_CURRENT_SOURCE_DIR}" "${CCOLA_PACK_DIR_protoc}/../../protobuf-3.3.0/src/google/protobuf" "${CCOLA_PACK_DIR_protoc}/../../../protobuf/echo_attributes")
            foreach(path ${dependency_path})
                list(APPEND protopath --proto_path ${path})
            endforeach()
        endforeach()

        if(CMAKE_HOST_WIN32)
            set(error_format "msvs")
        else()
            set(error_format "gcc")
        endif()

        if (CMAKE_HOST_WIN32)
            set(protoc_compiler_binary ${CCOLA_PACK_DIR_protoc}/../bin/protoc.exe)
        elseif (CMAKE_HOST_APPLE)
            set(protoc_compiler_binary ${CCOLA_PACK_DIR_protoc}/../bin/protoc-osx-x86_64)
        elseif (CMAKE_HOST_UNIX)
            set(protoc_compiler_binary ${CCOLA_PACK_DIR_protoc}/../bin/protoc-linux-x86_64)
        else ()
            message(FATAL_ERROR "No suitable proto compiler found for ${CMAKE_HOST_SYSTEM_NAME}")
        endif()

        add_custom_command(
            OUTPUT "${generated_dir_cpp}/${source_base}.pb.cc" "${generated_dir_cpp}/${source_base}.pb.h"
            COMMAND ${CMAKE_COMMAND} -E make_directory ${generated_dir_cpp}
            COMMAND ${protoc_compiler_binary} ${protopath} --error_format=${error_format} --cpp_out="${generated_dir_cpp}" ${absolute_input}
            DEPENDS "${absolute_input}"
        )

        ccola_sources("${generated_dir_cpp}/${source_base}.pb.cc" "${generated_dir_cpp}/${source_base}.pb.h")
        ccola_set_source_group("Generated Files" "${generated_dir_cpp}/${source_base}.pb.cc" "${generated_dir_cpp}/${source_base}.pb.h")
        ccola_inheritable_include_directories("${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/generated/proto_cpp")

        ccola_dependencies(
            protobuf.libprotobuf
        )
    endif()

endmacro()

macro(protocol_buffer_csharp input)

    if(NOT CCOLA_COMPONENT_SKIP)
        ccola_make_absolute_from_source(absolute_input ${input})
        ccola_sources("${absolute_input}")
        ccola_set_source_group("Source Files" "${absolute_input}")
        ccola_get_filename_component_we(source_base ${input})

        get_filename_component(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated" ABSOLUTE)

        set(protopath)
        foreach(dependency ${PROTOBUF_DEPENDENCIES})
            ccola_read_public_property("${dependency}" proto paths)
            foreach(path ${paths})
                list(APPEND protopath --proto_path ${path})
            endforeach()
        endforeach()

        foreach(dependency_path "${CMAKE_CURRENT_SOURCE_DIR}" "${CCOLA_PACK_DIR_protoc}/../../protobuf-3.3.0/src/google/protobuf" "${CCOLA_PACK_DIR_protoc}/../../../protobuf/echo_attributes")
            foreach(path ${dependency_path})
                list(APPEND protopath --proto_path ${path})
            endforeach()
        endforeach()

        if(CMAKE_HOST_WIN32)
            set(error_format "msvs")
        else()
            set(error_format "gcc")
        endif()

        if (CMAKE_HOST_WIN32)
            set(protoc_compiler_binary ${CCOLA_PACK_DIR_protoc}/../bin/protoc.exe)
        elseif (CMAKE_HOST_APPLE)
            set(protoc_compiler_binary ${CCOLA_PACK_DIR_protoc}/../bin/protoc-osx-x86_64)
        elseif (CMAKE_HOST_UNIX)
            set(protoc_compiler_binary ${CCOLA_PACK_DIR_protoc}/../bin/protoc-linux-x86_64)
        else ()
            message(FATAL_ERROR "No suitable proto compiler found for ${CMAKE_HOST_SYSTEM_NAME}")
        endif()

        add_custom_command(
            OUTPUT "${generated_dir}/${source_base}.cs"
            COMMAND ${CMAKE_COMMAND} -E make_directory ${generated_dir}
            COMMAND ${protoc_compiler_binary} ${protopath} --error_format=${error_format} --csharp_out="${generated_dir}" ${absolute_input}
            DEPENDS "${absolute_input}"
        )

        ccola_sources("${generated_dir}/${source_base}.cs")
        ccola_set_source_group("Generated Files" "${generated_dir}/${source_base}.cs")
    endif()

endmacro()

macro(protocol_buffer_java input)

    if(NOT CCOLA_COMPONENT_SKIP)
        ccola_make_absolute_from_source(absolute_input ${input})
        ccola_sources("${absolute_input}")
        ccola_set_source_group("Source Files" "${absolute_input}")
        ccola_get_filename_component_we(source_base ${input})

        get_filename_component(java_dir "${CMAKE_CURRENT_BINARY_DIR}/generated" ABSOLUTE)
        get_filename_component(generated_dir "${java_dir}/com/philips/cococo/protobufEcho" ABSOLUTE)

        set(protopath)
        foreach(dependency ${PROTOBUF_DEPENDENCIES})
            ccola_read_public_property("${dependency}" proto paths)
            foreach(path ${paths})
                list(APPEND protopath --proto_path ${path})
            endforeach()
        endforeach()

        foreach(dependency_path "${CMAKE_CURRENT_SOURCE_DIR}" "${CCOLA_PACK_DIR_protoc}/../../protobuf-3.3.0/src/google/protobuf" "${CCOLA_PACK_DIR_protoc}/../../../protobuf/echo_attributes")
            foreach(path ${dependency_path})
                list(APPEND protopath --proto_path ${path})
            endforeach()
        endforeach()

        if(CMAKE_HOST_WIN32)
            set(error_format "msvs")
        else()
            set(error_format "gcc")
        endif()

        if (CMAKE_HOST_WIN32)
            set(protoc_compiler_binary ${CCOLA_PACK_DIR_protoc}/../bin/protoc.exe)
        elseif (CMAKE_HOST_APPLE)
            set(protoc_compiler_binary ${CCOLA_PACK_DIR_protoc}/../bin/protoc-osx-x86_64)
        elseif (CMAKE_HOST_UNIX)
            set(protoc_compiler_binary ${CCOLA_PACK_DIR_protoc}/../bin/protoc-linux-x86_64)
        else ()
            message(FATAL_ERROR "No suitable proto compiler found for ${CMAKE_HOST_SYSTEM_NAME}")
        endif()

        add_custom_command(
            OUTPUT "${generated_dir}/${source_base}Proto.java"
            COMMAND ${CMAKE_COMMAND} -E make_directory ${generated_dir}
            COMMAND ${protoc_compiler_binary} ${protopath} --error_format=${error_format} --java_out="${java_dir}" ${absolute_input}
            DEPENDS "${absolute_input}"
        )

        ccola_sources("${generated_dir}/${source_base}Proto.java")
        ccola_set_source_group("Generated Files" "${generated_dir}/${source_base}Proto.java")
    endif()

endmacro()

macro(protocol_buffer_dependencies)

    if(NOT CCOLA_COMPONENT_SKIP)
        ccola_assert(CCOLA_COMPONENT_NAME "This command may only be used in a component")

        set(dependencies "${ARGV}")
        ccola_parse_selection_list(dependencies)

        foreach(item ${dependencies})
            ccola_make_full_component_name(item)
            ccola_dependencies(${item})
            set(PROTOBUF_DEPENDENCIES ${PROTOBUF_DEPENDENCIES} ${item})
        endforeach()
    endif()

endmacro()
