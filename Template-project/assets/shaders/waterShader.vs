#version 330 core
precision highp float;

attribute vec2 position;

varying vec2 textureCoords;
varying vec4 clipSpace;
varying vec3 toCameraVector;
varying vec3 fromLightVector;

uniform mat4 modelMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 cameraPosition; 
uniform vec3 lightPosition;

const float tiling = 2.0;

void main(){
    vec4 worldPosition = modelMatrix * vec4(position.x, 0.0, position.y, 1.0);
    clipSpace = projectionMatrix  * viewMatrix * worldPosition;     // needed for projective texture mapping of reflection and refraction textures
    textureCoords = vec2(position.x/2.0 + 0.5, position.y/2.0 + 0.5) * tiling;
    toCameraVector = cameraPosition - worldPosition.xyz;
    fromLightVector = worldPosition.xyz - lightPosition;

    gl_Position = clipSpace;
}