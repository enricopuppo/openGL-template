find_package(OpenGL REQUIRED)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL " " FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL " " FORCE)
set(GLFW_INSTALL OFF CACHE BOOL " " FORCE)
add_subdirectory(glfw)

#add_library(imgui/imgui.cpp imgui/imgui_demo.cpp 
#    imgui/imgui_draw.cpp 
#    imgui/imgui_widgets.cpp
#    imgui/imgui_tables.cpp 
#    imgui/imgui_impl_glfw.cpp 
#    imgui/imgui_impl_opengl3.cpp 
#    glad/glad.c)
  
#  target_include_directories(imgui PRIVATE .)
#  target_include_directories(imgui PUBLIC ${CMAKE_SOURCE_DIR}/libs)
#  target_compile_definitions(imgui PRIVATE -DIMGUI_IMPL_OPENGL_LOADER_GLAD)
#  target_compile_definitions(imgui PUBLIC -DYOCTO_OPENGL)
#  set_target_properties(imgui PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)

#  target_link_libraries(imgui PUBLIC yocto)

#  target_include_directories(imgui PRIVATE ${CMAKE_SOURCE_DIR}/exts/glfw/include)
#  target_link_libraries(imgui PUBLIC glfw ${OPENGL_gl_LIBRARY})

#  if(UNIX AND NOT APPLE)
#    target_link_libraries(imgui PUBLIC dl)
#  endif(UNIX AND NOT APPLE)

# warning flags
#if(APPLE)
#  target_compile_options(imgui PUBLIC -Wall -Wconversion -Wno-sign-conversion -Wno-implicit-float-conversion)
#endif(APPLE)
#if(MSVC)
#target_compile_options(imgui PUBLIC /D_CRT_SECURE_NO_WARNINGS /wd4018 /wd4244 /wd4305 /wd4800 /wd4267)
#target_compile_options(imgui PUBLIC /EHsc)
#endif(MSVC)


