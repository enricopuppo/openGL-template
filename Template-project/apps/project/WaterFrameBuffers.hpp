#ifndef WATERFRAMEBUFFERS_H
#define WATERFRAMEBUFFERS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class WaterFrameBuffers
{
private:
	int reflectionWidth = 640;
	int reflectionHeight = 360;
	int refractionWidth = 1280;
	int refractionHeight = 720;
	int screenWidth;
	int screenHeight;

	GLuint reflectionFrameBuffer;
	GLuint reflectionTexture;
	GLuint reflectionDepthBuffer;
	GLuint refractionFrameBuffer;
	GLuint refractionTexture;
	GLuint refractionDepthTexture;

	void initialiseReflectionFrameBuffer();
	void initialiseRefractionFrameBuffer();
	void bindFrameBuffer(int frameBuffer, int width, int height);
	GLuint createFrameBuffer();
	GLuint createTextureAttachment(int width, int height);
	GLuint createDepthTextureAttachment(int width, int height);
	GLuint createDepthBufferAttachment(int width, int height);

public:
	WaterFrameBuffers() = default;
	WaterFrameBuffers(int screenWidth, int screenHeight);
	void clean();
	void bindReflectionFB();
	void bindRefractionFB();
	void unbindCurrentFB();
	GLuint getReflectionTexture();
	GLuint getRefractionTexture();
};

#endif