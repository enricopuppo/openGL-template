find_package(OpenGL REQUIRED)

add_executable(project Main.cpp ArcBall.cpp Camera.cpp DisplayManager.cpp Entity.cpp EntityRenderer.cpp Light.cpp MasterRenderer.cpp MatrixGenerator.cpp Model.cpp ModelLoader.cpp ObjectShader.cpp OBJLoader.cpp Player.cpp ProcModelGenerator.cpp Quaternion.cpp ShaderProgram.cpp Terrain.cpp TerrainRenderer.cpp TerrainShader.cpp TerrainTexture.cpp TerrainTexturePack.cpp Texture.cpp TexturedModel.cpp WaterFrameBuffers.cpp WaterRenderer.cpp WaterShader.cpp WaterTile.cpp)

set_target_properties(project  PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES)
target_include_directories(project PRIVATE ${CMAKE_SOURCE_DIR}/exts)
target_include_directories(project PRIVATE ${CMAKE_SOURCE_DIR}/exts/glfw/include)

target_link_libraries(project PRIVATE glad glfw imgui)
