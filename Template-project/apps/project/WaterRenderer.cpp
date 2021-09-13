#include "WaterRenderer.hpp"
#include "WaterShader.hpp"
#include "WaterTile.hpp"
#include "WaterFrameBuffers.hpp"
#include "ProcModelGenerator.hpp"
#include "MatrixGenerator.hpp"
#include "Model.hpp"

WaterRenderer::WaterRenderer(ModelLoader &loader, WaterShader &shader, glm::mat4 projectionMatrix, WaterFrameBuffers fbos, DisplayManager *dm){
	this->shader = shader;
	this->fbos = fbos;
	this->waveSpeed = 0.03;
	this->moveFactor = 0.0;
	this->dm = dm;
	this->dudvTexture = loader.loadTexture(dudvMapPath, GL_RGB);
	this->normalTexture = loader.loadTexture(normalMapPath, GL_RGB);
	shader.start();
	shader.connectTextureUnits();
	shader.loadProjectionMatrix(projectionMatrix);
	shader.stop();
	setUpVAO(loader);
}

void WaterRenderer::render(std::vector<WaterTile> &water, Camera &camera, Light &sun){
	// renders all the water tiles
	MatrixGenerator mg = MatrixGenerator();
	prepareRenderer(camera, sun);
	for(auto tile : water){
		glm::mat4 modelMatrix = mg.createTransformationMatrix(glm::vec3(tile.getX(), tile.getY(), tile.getZ()), glm::vec3(0.0, 0.0, 0.0), glm::vec3(tile.getTileSize()*3.0, 1.0, tile.getTileSize()*2.0));
		shader.loadModelMatrix(modelMatrix);
		glDisable(GL_CULL_FACE);
		glDrawArrays(GL_TRIANGLES, 0, quad.getVertexCount());
		glEnable(GL_CULL_FACE);
	}
	unbind();
}

void WaterRenderer::prepareRenderer(Camera &camera, Light &sun){
	// activates all the generated textures needed for reflection, refraction and waves
	shader.start();
	shader.loadViewMatrix(camera);
	moveFactor += waveSpeed  * (*dm).getDeltaTime();
	moveFactor = fmod(moveFactor, 1.0);
	shader.loadMoveFactor(moveFactor);
	shader.loadLight(sun);
	glBindVertexArray(quad.getVaoID());
	glEnableVertexAttribArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fbos.getReflectionTexture());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, fbos.getRefractionTexture());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dudvTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normalTexture);
}

void WaterRenderer::unbind(){
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	shader.stop();
}

void WaterRenderer::setUpVAO(ModelLoader &loader){
	// generates a quad for the water tile
	std::vector<GLfloat> vertices = {-1.0, -1.0,
									 -1.0,  1.0,
									  1.0, -1.0,
									  1.0, -1.0,
									 -1.0,  1.0,
									  1.0,  1.0};
	quad = loader.loadToVAO(vertices, 2);
}