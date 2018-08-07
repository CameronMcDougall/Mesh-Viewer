
# Source Files
file(GLOB_RECURSE sources *.c *.cpp *.h *.hpp)
#file(GLOB sources *.c *.cpp *.h *.hpp)
if ( 1 ) # debug
    foreach(line IN LISTS sources)
        string(APPEND multiline "${line}\n\t")
    endforeach() 
    message(STATUS "sources (verify that all source files were found. If some are missing, e.g. from the cgra subdirectory, then make sure the path doesn't contain spaces):\n\t${multiline}")    
endif()

#add_subdirectory(cgra)

add_executable(${CGRA_PROJECT} ${sources})

target_source_group_tree(${CGRA_PROJECT})

target_link_libraries(${CGRA_PROJECT} PRIVATE ${OPENGL_LIBRARY})
target_link_libraries(${CGRA_PROJECT} PRIVATE glfw ${GLFW_LIBRARIES})
target_link_libraries(${CGRA_PROJECT} PRIVATE glew glm imgui)

target_include_directories(${CGRA_PROJECT} PRIVATE "${PROJECT_SOURCE_DIR}/src")

# Set the source directory as a preprocessor define, used to make sure that the relative paths
# work correctly, regardless of where the project is run from (as long as it's run on the same
# machine it was built on).
target_compile_definitions(${CGRA_PROJECT} PRIVATE "-DCGRA_SRCDIR=\"${PROJECT_SOURCE_DIR}\"")