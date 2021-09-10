find_package(OpenGL REQUIRED)

add_library(glad glad.c)

target_link_libraries(glad PUBLIC ${OPENGL_gl_LIBRARY})

 if(UNIX AND NOT APPLE)
   target_link_libraries(glad PUBLIC dl)
 endif(UNIX AND NOT APPLE)
