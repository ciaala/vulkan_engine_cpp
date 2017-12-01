SET(DOC_SOURCES
        modules.rst
        modules/ResourceManager.rst
        modules/audio.rst
        startup.rst
        free_form.rst
        vulkan/command_buffer.rst
        vulkan/pipeline.rst
        )
find_program(RST_TOOL rst2html5)
add_custom_target(DOCUMENTATION ALL)

foreach (DOC_SOURCE ${DOC_SOURCES})
    set(DOC_SOURCE_FILE_OUTPUT ${DOC_SOURCE}.html)
    string(REPLACE "/" "_" DOC_SOURCE_OUTPUT ${DOC_SOURCE_FILE_OUTPUT})
    set(DOC_FILE ${CMAKE_CURRENT_BINARY_DIR}/${DOC_SOURCE_FILE_OUTPUT})
    get_filename_component(PARENT_DIR ${DOC_FILE} DIRECTORY)
    file(MAKE_DIRECTORY ${PARENT_DIR})
    add_custom_target(
            ${DOC_SOURCE_OUTPUT} ALL
            COMMAND ${RST_TOOL} ${CMAKE_CURRENT_SOURCE_DIR}/${DOC_SOURCE} ${DOC_FILE}
            DEPENDS ${DOC_SOURCE}
            SOURCES ${DOC_SOURCE}
            COMMENT "Creating documentation " ${DOC_SOURCE_OUTPUT})
    #SET(DOCUMENTATION_DEPENDS ${DOC_SOURCE_OUTPUT})
    message("Documentation file: " ${DOC_SOURCE})
    add_dependencies(DOCUMENTATION ${DOC_SOURCE_OUTPUT})
endforeach ()

#add_custom_target(DOCUMENTATION ALL
#        DEPENDS ${DOCUMENTATION_DEPENDS}
#        SOURCE ${DOC_SOURCES})
