#version 330 core
precision highp float;

attribute vec3 position;
attribute vec2 textureCoords;
attribute vec3 normal;
uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPosition[4];
uniform vec4 clipPlane;

varying vec2 passTextureCoords;
varying vec3 surfaceNormal;
varying vec3 toLight[4];
varying vec3 toCamera;

void main(){
    vec4 worldPosition = transformationMatrix * vec4(position, 1.0);    // position from model coords to world coords
    surfaceNormal = (transformationMatrix * vec4(normal, 0.0)).xyz; // normals in world coords
    passTextureCoords = textureCoords;  // pass texture coords to the fragment shader
    for(int i=0; i<4; i++)  // for each light source
        toLight[i] = lightPosition[i] - worldPosition.xyz;    // vector from the vertex to the light source
    toCamera = (inverse(viewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;    // vector from the vertex to the camera
    
    gl_ClipDistance[0] = dot(worldPosition, clipPlane); // distance of the vertex from the clipping plane
    gl_Position = projectionMatrix * viewMatrix * worldPosition;    // position updated depenting on view and projection
}