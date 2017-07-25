#version 330 core

#define MAX_LIGHT 4

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

uniform struct Transform
{
  mat4 transformationMatrix;
  mat4 projectionMatrix;
  mat4 viewMatrix;
  vec3 viewPosition;
} transform;

uniform struct PointLight
{
  vec3 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec3 attenuation; 
} light[MAX_LIGHT];

out Vertex {
  vec2  texcoord;
  vec3  normal;
  vec3  viewDir;
  
  vec3 lightDir[MAX_LIGHT];
  float distance[MAX_LIGHT];
  
} vertex;

uniform int UsingLights;

void main()
{
	vec4 worldPosition = transform.transformationMatrix * vec4( position, 1.0f );	
 	
	vertex.texcoord = texCoord;
	vertex.normal   = (transform.transformationMatrix * vec4(normal,0.0f)).xyz;
	vertex.viewDir  = transform.viewPosition - vec3(worldPosition);

	for ( int i = 0; i < UsingLights; i++ )
	{
		vertex.lightDir[i] = ( vec4(light[i].position, 1.0f) - worldPosition).xyz;
		vertex.distance[i] = length( vertex.lightDir[i] );
	}

  gl_Position =  transform.projectionMatrix * transform.viewMatrix * worldPosition;
}