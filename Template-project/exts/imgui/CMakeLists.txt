find_package(OpenGL REQUIRED)

add_library(imgui
   imgui.cpp 
   imgui_draw.cpp 
   imgui_widgets.cpp
   imgui_tables.cpp 
   imgui_impl_glfw.cpp 
   imgui_impl_opengl3.cpp)

target_link_libraries(imgui PUBLIC glfw glad ${OPENGL_gl_LIBRARY})
target_compile_definitions(imgui PUBLIC -DIMGUI_IMPL_OPENGL_LOADER_GLAD)
target_include_directories(imgui PUBLIC ${CMAKE_SOURCE_DIR}/exts ${CMAKE_SOURCE_DIR}/exts/glfw/include)

 if(UNIX AND NOT APPLE)
   target_link_libraries(imgui PUBLIC dl)
 endif(UNIX AND NOT APPLE)
