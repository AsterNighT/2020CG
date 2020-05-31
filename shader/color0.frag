#version 460 core
#define PI 3.14159265f
#define MAX_LIGHT_COUNT

layout(location = 0) in VS_OUTPUT {
	vec4 Position;
	vec3 WorldPos;
	vec2 TextureCoordinate;
	vec3 WorldNormal;
	vec4 FragPosLightSpace[16];
}IN;

layout(location = 0) out vec4 finalColor;

layout (location = 0) uniform uint lightCount;
layout (location = 1) uniform vec3 lightPos[MAX_LIGHT_COUNT];
layout (location = 2) uniform vec3 lightColor[MAX_LIGHT_COUNT];
layout (location = 3) uniform vec3 cameraPos;
layout (location = 4) uniform float specPower;
layout (location = 5) uniform float ambientPower;

layout (location = 6) uniform sampler2D colorTexture;
layout (location = 7) uniform sampler2D shadowMap[MAX_LIGHT_COUNT];

/************* Pixel Shader *************/
float calcShadow(int index){
    vec3 projCoords = IN.FragPosLightSpace[index].xyz;
    projCoords = projCoords * 0.5 + 0.5;
	if(projCoords.z > 1.0) return 1.0;//No shadow
    float closestDepth = texture(shadowMap[index], projCoords.xy).r; 
    float currentDepth = projCoords.z;
	vec3 lightDir = lightPos[index]-IN.WorldPos.xyz;
	float bias = max(0.05 * (1.0 - dot(IN.WorldNormal, lightDir)), 0.005);
    float shadow = currentDepth - bias > closestDepth ? 0.0 : 1.0;
    return shadow;
}
vec3 calcPhongColor(int index){
	vec3 lightVec = lightPos[index] - IN.WorldPos.xyz;
	vec3 viewVec = cameraPos - IN.WorldPos.xyz;
	float strength = 1/length(lightVec);
	lightVec = normalize(lightVec);
	viewVec = normalize(viewVec);
	vec3 reflectVec = reflect(-lightVec, IN.WorldNormal);
	vec4 texColor = texture2D(colorTexture, IN.TextureCoordinate);
	vec4 specColor = texColor * vec4(lightColor[index], 1) * strength * pow(max(0,dot(reflectVec,viewVec)),specPower);
	vec4 diffuseColor = texColor * vec4(lightColor[index], 1) * strength * dot(lightVec,IN.WorldNormal);
	return vec3(specColor + diffuseColor);
}
vec3 calcAmbientColor(){
	vec4 texColor = texture2D(colorTexture, IN.TextureCoordinate);
	vec4 ambientColor = texColor * ambientPower;
	return vec3(ambientColor);
}
void main(){
	vec3 color = calcAmbientColor();
	for(int i=0;i<lightCount;i++){
		color += calcShadow(i)*calcPhongColor(i);
	}
    finalColor = vec4(color,1);
}
