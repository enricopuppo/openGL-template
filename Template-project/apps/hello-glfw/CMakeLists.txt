add_executable(hello-glfw  hello-glfw.cpp)

set_target_properties(hello-glfw  PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)
target_include_directories(hello-glfw  PRIVATE ${CMAKE_SOURCE_DIR}/exts)
target_include_directories(hello PRIVATE ${CMAKE_SOURCE_DIR}/exts/glfw/include)

target_link_libraries(hello-glfw PRIVATE glfw)
