#include "Model.hpp"
#include "WaterShader.hpp"
#include "ModelLoader.hpp"
#include "WaterTile.hpp"
#include "WaterFrameBuffers.hpp"
#include "Camera.hpp"
#include "DisplayManager.hpp"
#include "./glm/glm.hpp"
#include <vector>

class WaterRenderer
{
private:
	const std::string dudvMapPath = "../assets/textures/dudvmap.png";
	const std::string normalMapPath = "../assets/textures/water_normalmap.png";
	Model quad;
	WaterShader shader;
	WaterFrameBuffers fbos;
	GLuint dudvTexture;
	float waveSpeed;
	float moveFactor;
	DisplayManager *dm;
	GLuint normalTexture;

public:
	WaterRenderer(ModelLoader &loader, WaterShader &shader, glm::mat4 projectionMatrix, WaterFrameBuffers fbos, DisplayManager *dm);
	void render(std::vector<WaterTile> &water, Camera &camera, Light &sun);
	void prepareRenderer(Camera &camera, Light &sun);
	void unbind();
	void setUpVAO(ModelLoader &loader);
};