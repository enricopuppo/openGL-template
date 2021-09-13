#version 330 core
precision highp float;

varying highp vec2 passTextureCoords;
varying vec3 surfaceNormal;
varying vec3 toLight[4];
varying vec3 toCamera;

uniform sampler2D textureSampler;
uniform vec3 lightColor[4];
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 attenuation[4];

void main(){
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitToCamera = normalize(toCamera);

    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    for(int i=0; i<4; ++i){     // computes the lighting for each light
        float distanceFromLight = length(toLight[i]);
        float attenuationFactor = attenuation[i].x + (attenuation[i].y * distanceFromLight) + (attenuation[i].z * pow(distanceFromLight, 2));   // computes the attenuation using the parameters of each light
        
        vec3 unitToLight = normalize(toLight[i]);
        vec3 lightDirection = -unitToLight;
        float lightDot = dot(unitNormal, unitToLight); // dot product between normal and light vector
        float brightness = max(lightDot, 0.0); // negative thresholding the light 
        vec3 reflectedLightDirection = reflect(unitNormal, lightDirection); // compute the reflected light vector
        float specularFactor = max(dot(reflectedLightDirection, unitToCamera), 0.2);    // negative thresholding + constant minimum of the reflected light respect to camera
        float dampedFactor = pow(specularFactor, shineDamper);  // computes the shininess depending on the two texture parameters

        totalDiffuse = totalDiffuse + (brightness * lightColor[i]) / attenuationFactor; // compute the total diffuse light
        totalSpecular = totalSpecular + (dampedFactor * reflectivity * lightColor[i]) / attenuationFactor;  // computes the total specular light
    }

    totalDiffuse = max(totalDiffuse, 0.2);  // adds some constant brightness if needed

    vec4 textureColor = texture(textureSampler, passTextureCoords); // picks the color from the texture
    if(textureColor.a < 0.1)    // avoid to render transparent parts
        discard;

    gl_FragColor = vec4(totalDiffuse, 1.0) * textureColor + vec4(totalSpecular, 1.0);    // assigns to the fragment the final color as the combination of diffuse, specular light and texture color
}