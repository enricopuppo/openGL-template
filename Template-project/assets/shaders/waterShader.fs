#version 330 core
precision highp float;

varying highp vec4 clipSpace;
varying highp vec3 toCameraVector;
varying highp vec3 fromLightVector;
varying vec2 textureCoords;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudvSampler;
uniform sampler2D normalSampler;
uniform vec3 lightColor;
uniform float moveFactor;

const float waveStrength = 0.008;   // regulates the amount of distortion that simulates waves
const float shineDamper = 20.0;
const float reflectivity = 0.6;

void main(){
    vec2 normDeviceSpaceCoords = clipSpace.xy/clipSpace.w;      // fragment coordinates in device space
    vec2 normScreenSpaceCoords = normDeviceSpaceCoords / 2.0 + 0.5;     // fragment coordinates in screen space
    
    vec2 refractTexCoords = vec2(normScreenSpaceCoords.x, normScreenSpaceCoords.y);
    vec2 reflectTexCoords = vec2(normScreenSpaceCoords.x, -normScreenSpaceCoords.y);    //negative y because flipped

    // dudv water crease with apparent motion 
    vec2 distortedTexCoords = texture(dudvSampler, vec2(textureCoords.x + moveFactor, textureCoords.y)).rg * 0.1;
    distortedTexCoords = textureCoords + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);
    vec2 distortion = (texture(dudvSampler, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength;

    refractTexCoords += distortion; // distortion applied to refraction
    refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);   // avoids wavy glitch at the bottom of the screen

    reflectTexCoords += distortion; // distortion applied to reflection
    reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001, 0.999);   // same as for the refraction, but considering the flip
    reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999, -0.001);
    
    vec4 reflectionColor = texture(reflectionTexture, reflectTexCoords);    // final sampling of reflection texture
    vec4 refractionColor = texture(refractionTexture, refractTexCoords);    // final sampling of refraction sampling

    // fresnel effect to make more or less reflective depending on the camera angle
    vec3 viewVector = normalize(toCameraVector);    
    float refractiveFactor = dot(viewVector, vec3(0.0, 1.0, 0.0));
    refractiveFactor = pow(refractiveFactor, 1.0);

    // use of normal map to add more realistic wave reflections
    vec4 normalSamplerColor = texture(normalSampler, distortedTexCoords);
    vec3 normal = vec3(normalSamplerColor.r * 2.0 - 1.0, normalSamplerColor.b, normalSamplerColor.g * 2.0 - 1.0);
    normal = normalize(normal);

    // highlights computation
    vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
    float specular = max(dot(reflectedLight, viewVector), 0.0);
    specular = pow(specular, shineDamper);
    vec3 specularHighlights = lightColor * specular * reflectivity;

    vec4 outColor = mix(reflectionColor, refractionColor, refractiveFactor);    // final assembly of all the factors
    gl_FragColor = mix(outColor, vec4(0.0, 0.3, 0.5, 1.0), 0.2) + vec4(specularHighlights, 0.0);    // add a fixed light blue tint to the computed color and the specular highlights
}