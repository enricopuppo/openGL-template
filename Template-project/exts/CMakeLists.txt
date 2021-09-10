add_subdirectory(glad)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL " " FORCE)
set(GLFW_INSTALL OFF CACHE BOOL " " FORCE)

add_subdirectory(glfw)

add_subdirectory(imgui)

#  target_include_directories(imgui PRIVATE .)
#  target_include_directories(imgui PUBLIC ${CMAKE_SOURCE_DIR}/libs)
#  target_compile_definitions(imgui PRIVATE -DIMGUI_IMPL_OPENGL_LOADER_GLAD)
#  target_compile_definitions(imgui PUBLIC -DYOCTO_OPENGL)
#  set_target_properties(imgui PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)

