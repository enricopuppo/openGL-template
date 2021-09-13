#version 330 core
precision highp float;

varying highp vec2 passTextureCoords;
varying vec3 surfaceNormal;
varying vec3 toLight[4];
varying vec3 toCamera;

uniform sampler2D rSampler;
uniform sampler2D gSampler;
uniform sampler2D bSampler;
uniform sampler2D blendMapSampler;

uniform vec3 lightColor[4];
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 attenuation[4];

void main(){
    vec3 unitNormal = normalize(surfaceNormal);
    vec3 unitToCamera = normalize(toCamera);

    vec4 blendMapColor = texture(blendMapSampler, passTextureCoords);   // picks the fragment color from the blend map
    vec2 tiledCoords = passTextureCoords * 10.0;    // enlarges the uv coords to tile the textures
    vec4 rTextureColor = texture(rSampler, tiledCoords) * blendMapColor.r;  // picks the grass color proportionally to the blend map value for red
    vec4 gTextureColor = texture(gSampler, tiledCoords) * blendMapColor.g;  // picks the plank color proportionally to the blend map value for green
    vec4 bTextureColor = texture(bSampler, tiledCoords) * blendMapColor.b;  // picks the dirt color proportionally to the blend map value for blue
    vec4 totalColor = rTextureColor + gTextureColor + bTextureColor;    // sums the colors picked from all the textures

    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);

    for(int i=0; i<4; ++i){
        float distanceFromLight = length(toLight[i]);
        float attenuationFactor = attenuation[i].x + (attenuation[i].y * distanceFromLight) + (attenuation[i].z * pow(distanceFromLight, 2));   // computes the attenuation using the parameters of each light
    
        vec3 unitToLight = normalize(toLight[i]);
        vec3 lightDirection = -unitToLight; // direction from light to object
        float lightDot = dot(unitNormal, unitToLight);  // dot product between face normal and toLight vector
        float brightness = max(lightDot, 0.0);  // negative thresholding the light + constant minimum brightness
        vec3 reflectedLightDirection = reflect(unitNormal, lightDirection); // compute the reflected light vector
        float specularFactor = max(dot(reflectedLightDirection, unitToCamera), 0.2);    // negative thresholding + constant minimum of the reflected light respect to camera
        float dampedFactor = pow(specularFactor, shineDamper);  // computes the shininess depending on the two texture parameters
        totalDiffuse = totalDiffuse + (brightness * lightColor[i]) / attenuationFactor; // compute the total diffuse light
        totalSpecular = totalSpecular + (dampedFactor * reflectivity * lightColor[i]) / attenuationFactor;  // computes the total specular light
    }

    totalDiffuse = max(totalDiffuse, 0.2);  // adds some constant brightness if needed
    if(totalColor.a < 0.1)  // avoid to render transparent parts
        discard;

    gl_FragColor = vec4(totalDiffuse, 1.0) * totalColor + vec4(totalSpecular, 1.0);  // assigns to the fragment the final color as the combination of diffuse, specular light and texture color
}