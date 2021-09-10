find_package(OpenGL REQUIRED)

add_executable(hello  hello.cpp)

set_target_properties(hello  PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)
target_include_directories(hello PRIVATE ${CMAKE_SOURCE_DIR}/exts)
target_include_directories(hello PRIVATE ${CMAKE_SOURCE_DIR}/exts/glfw/include)

target_link_libraries(hello PRIVATE glad glfw)
