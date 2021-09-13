#include "WaterFrameBuffers.hpp"
#include <GLFW/glfw3.h>
#include <stdio.h>

WaterFrameBuffers::WaterFrameBuffers(int screenWidth, int screenHeight)
{
    initialiseReflectionFrameBuffer();
    initialiseRefractionFrameBuffer();
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}

void WaterFrameBuffers::initialiseReflectionFrameBuffer()
{
    reflectionFrameBuffer = createFrameBuffer();
    reflectionTexture = createTextureAttachment(reflectionWidth, reflectionHeight);
    reflectionDepthBuffer = createDepthBufferAttachment(reflectionWidth, reflectionHeight);
    unbindCurrentFB();
}

void WaterFrameBuffers::initialiseRefractionFrameBuffer()
{
    refractionFrameBuffer = createFrameBuffer();
    refractionTexture = createTextureAttachment(refractionWidth, refractionHeight);
    refractionDepthTexture = createDepthBufferAttachment(refractionWidth, refractionHeight);
    unbindCurrentFB();
}

void WaterFrameBuffers::bindFrameBuffer(int frameBuffer, int width, int height)
{
    // binds the selected FBO to render in it
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, width, height);
}

GLuint WaterFrameBuffers::createFrameBuffer()
{
    // creates a new framebuffer setting the correspondent color attatchment
    GLuint frameBuffer;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    return frameBuffer;
}

GLuint WaterFrameBuffers::createTextureAttachment(int width, int height)
{
    // generates a texture and binds it with the color attachment 0 where we render
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
    return texture;
}

GLuint WaterFrameBuffers::createDepthTextureAttachment(int width, int height)
{
    // generates a new texture and binds it with the frame buffer as depht buffer
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, (void *)NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
    return texture;
}

GLuint WaterFrameBuffers::createDepthBufferAttachment(int width, int height)
{
    // generates a new renderbuffer for the depth
    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    return depthBuffer;
}

void WaterFrameBuffers::clean()
{
    glDeleteFramebuffers(1, &reflectionFrameBuffer);
    glDeleteFramebuffers(1, &refractionFrameBuffer);
    glDeleteTextures(1, &reflectionTexture);
    glDeleteTextures(1, &refractionFrameBuffer);
    glDeleteRenderbuffers(1, &reflectionDepthBuffer);
    glDeleteTextures(1, &refractionDepthTexture);
}

void WaterFrameBuffers::bindReflectionFB()
{
    bindFrameBuffer(reflectionFrameBuffer, reflectionWidth, reflectionHeight);
}

void WaterFrameBuffers::bindRefractionFB()
{
    bindFrameBuffer(refractionFrameBuffer, refractionWidth, refractionHeight);
}

void WaterFrameBuffers::unbindCurrentFB()
{
    // resets the default fbo
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint WaterFrameBuffers::getReflectionTexture()
{
    return reflectionTexture;
}

GLuint WaterFrameBuffers::getRefractionTexture()
{
    return refractionTexture;
}