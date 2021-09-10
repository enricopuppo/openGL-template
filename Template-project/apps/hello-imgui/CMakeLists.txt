add_executable(hello-imgui  hello-imgui.cpp)

set_target_properties(hello-imgui  PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)
target_include_directories(hello-imgui  PRIVATE ${CMAKE_SOURCE_DIR}/exts)
target_include_directories(hello-imgui PRIVATE ${CMAKE_SOURCE_DIR}/exts/glfw/include)

target_link_libraries(hello-imgui PRIVATE imgui imgui)
