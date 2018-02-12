#add_custom_target(CALLGRAPH ALL)
message("\nCall graph files to generate:")
foreach (SOURCE_FILE ${SOURCE_FILES})
    string(REGEX MATCH "cpp$" IS_CPP_FILE ${SOURCE_FILE})
    if(IS_CPP_FILE)
        message("\t" ${SOURCE_FILE})
        #string(APPEND SOURCES_LIST " " ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE})
        #add_dependencies(CALLGRAPH ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE})
    endif()
endforeach()


add_custom_target(CALLGRAPH ALL
        COMMAND clang++ -std=c++14 -fsyntax-only -S -emit-llvm ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE} -o - | opt -analyze -dot-callgraph
        SOURCES ${SOURCE_FILE}
        COMMENT "Creating call graph " ${SOURCE_GRAPH})


