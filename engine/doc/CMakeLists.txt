SET(DOC_SOURCES
        modules.rst
        startup.rst
        )
find_program(RST_TOOL rst2html5)
if (RST_TOOL)
add_custom_target(DOCUMENTATION ALL)
foreach (DOC_SOURCE ${DOC_SOURCES})
    set(DOC_SOURCE_OUTPUT ${DOC_SOURCE}.html)
    add_custom_target(
            ${DOC_SOURCE_OUTPUT} ALL
            COMMAND ${RST_TOOL} ${CMAKE_CURRENT_SOURCE_DIR}/${DOC_SOURCE} ${CMAKE_CURRENT_BINARY_DIR}/${DOC_SOURCE_OUTPUT}
            DEPENDS ${DOC_SOURCE}
            SOURCES ${DOC_SOURCE}
            COMMENT "Creating documentation " ${DOC_SOURCE_OUTPUT})
    #SET(DOCUMENTATION_DEPENDS ${DOC_SOURCE_OUTPUT})
    message("Documentation file: " ${DOC_SOURCE})
    add_dependencies(DOCUMENTATION ${DOC_SOURCE_OUTPUT})
endforeach ()
    else()
    message("Unable to compile documentation without tool")
        endif()
#add_custom_target(DOCUMENTATION ALL
#        DEPENDS ${DOCUMENTATION_DEPENDS}
#        SOURCE ${DOC_SOURCES})
