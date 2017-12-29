SET(DOC_SOURCES
        RunningEnvironment.rst
        DebuggingSetup.rst
        index.rst
        engine/modules/ResourceManager.rst
        engine/modules/audio.rst

        engine/vulkan/command_buffer.rst
        engine/vulkan/pipeline.rst

        engine/modules.rst
        engine/world_updater.rst
        engine/render_loop.rst
        engine/startup.rst
        engine/free_form.rst
        )
find_program(RST_TOOL rst2html5)
add_custom_target(DOCUMENTATION ALL)
message("\nDocumentation file to generate:")
foreach (DOC_SOURCE ${DOC_SOURCES})
    set(DOC_SOURCE_FILE_OUTPUT ${DOC_SOURCE}.html)
    string(REPLACE "/" "_" DOC_SOURCE_OUTPUT ${DOC_SOURCE_FILE_OUTPUT})
    string(REPLACE ".rst." "." DOC_FILE ${CMAKE_CURRENT_BINARY_DIR}/generated/${DOC_SOURCE_FILE_OUTPUT})
    get_filename_component(PARENT_DIR ${DOC_FILE} DIRECTORY)
    file(MAKE_DIRECTORY ${PARENT_DIR})
    add_custom_target(
            ${DOC_SOURCE_OUTPUT} ALL
            COMMAND ${RST_TOOL} ${CMAKE_CURRENT_SOURCE_DIR}/${DOC_SOURCE} ${DOC_FILE}
            DEPENDS ${DOC_SOURCE}
            SOURCES ${DOC_SOURCE}
            COMMENT "Creating documentation " ${DOC_SOURCE_OUTPUT})
    #SET(DOCUMENTATION_DEPENDS ${DOC_SOURCE_OUTPUT})
    message("\t" ${DOC_SOURCE} " => " ${DOC_FILE})
    add_dependencies(DOCUMENTATION ${DOC_SOURCE_OUTPUT})
endforeach ()
message("\n")

#add_custom_target(DOCUMENTATION ALL
#        DEPENDS ${DOCUMENTATION_DEPENDS}
#        SOURCE ${DOC_SOURCES})
