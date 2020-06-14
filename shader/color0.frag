#version 440 core
#define PI 3.14159265f
#define MAX_LIGHT_COUNT

layout(location = 0) in VS_OUTPUT {
	vec4 Position;
	vec4 WorldPos;
	vec2 TextureCoordinate;
	vec3 WorldNormal;
	vec4 FragPosLightSpace;
}IN;

layout(location = 0) out vec4 finalColor;

layout (location = 2) uniform mat4 lightSpaceMatrix;
layout (location = 3) uniform vec3 lightPos;
layout (location = 4) uniform vec3 lightColor;
layout (location = 5) uniform float lightStrength;
layout (location = 6) uniform vec3 cameraPos;
layout (location = 7) uniform float specPower;
layout (location = 8) uniform float ambientPower;

layout (location = 9) uniform sampler2D colorTexture;
layout (location = 10) uniform sampler2D shadowMap;

/************* Pixel Shader *************/
float calcShadow(){
    vec3 projCoords = IN.FragPosLightSpace.xyz/IN.FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + vec3(0.5,0.5,0.5);
	if(projCoords.x>1||projCoords.x<0) return 0.0;
	if(projCoords.y>1||projCoords.y<0) return 0.0;
	if(projCoords.z > 1.0) return 0.0;//No shadow
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;
	float bias = 0.005;
    float shadow = currentDepth - bias > closestDepth ? 0.0 : 1.0;
    return shadow;
	//float shadow = 0.0;
	//vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	//for(int x = -1; x <= 1; ++x){
	//    for(int y = -1; y <= 1; ++y)		{
	//		float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
	//		shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
	//	}    
	//} 
	//shadow /= 9.0;
	//return 1-shadow;

}
vec3 calcPhongColor(){
	vec3 lightVec = lightPos - IN.WorldPos.xyz;
	vec3 viewVec = cameraPos - IN.WorldPos.xyz;
	float strength = min(1,lightStrength/length(lightVec));///length(lightVec);
	lightVec = normalize(lightVec);
	viewVec = normalize(viewVec);
	vec3 reflectVec = reflect(-lightVec, IN.WorldNormal);
	vec4 texColor = IN.TextureCoordinate.x >= 0 ? texture2D(colorTexture, IN.TextureCoordinate) : vec4(0.7,0.7,0.7,0.7);
	vec4 specColor = texColor * vec4(lightColor, 1) * strength * pow(max(0,dot(reflectVec,viewVec)),specPower);
	vec4 diffuseColor = texColor * vec4(lightColor, 1) * strength * max(0,dot(lightVec,IN.WorldNormal));
	return vec3(specColor+diffuseColor);
}
vec3 calcAmbientColor(){
	vec4 texColor = texture2D(colorTexture, IN.TextureCoordinate);
	vec4 ambientColor = texColor * ambientPower;
	return vec3(ambientColor);
}
void main(){
	vec3 color = calcAmbientColor();
	color += calcShadow()*calcPhongColor();
    finalColor = vec4(color,1);
}
