#version 330 core

#define MAX_LIGHT 4

uniform struct PointLight
{
  vec3 position;
  vec4 ambient;
  vec4 diffuse;
  vec4 specular;
  vec3 attenuation;
} light[MAX_LIGHT];

uniform struct Material
{
  sampler2D texture;

  vec4  ambient;
  vec4  diffuse;
  vec4  specular;
  vec4  emission;
  float shininess;
} material;

in Vertex {
  vec2  texcoord;
  vec3  normal;
  vec3  viewDir;
  
  vec3 lightDir[MAX_LIGHT];
  float distance[MAX_LIGHT];
  
} vertex;

out vec4 color;

uniform int UsingLights;

void main()
{	
  // нормализуем полученные данные для коррекции интерполяции
  vec3 normal   = normalize(vertex.normal);
  vec3 viewDir  = normalize(vertex.viewDir);
 
  
    // добавим собственное свечение материала
  color = material.emission;
  
  	for ( int i = 0; i < UsingLights; i++ )
	{
	 vec3 lightDir =normalize( vertex.lightDir[i] ); 
  // коэффициент затухания
 float attenuation = 1.0 / (light[i].attenuation.x +
    light[i].attenuation.y * vertex.distance[i] +
    light[i].attenuation.z * vertex.distance[i] * vertex.distance[i]);
	
  // добавим фоновое освещение
  color += material.ambient * light[i].ambient * attenuation;

  // добавим рассеянный свет
  float NdotL = max(dot(normal, lightDir), 0.0);
  color += material.diffuse * light[i].diffuse * NdotL * attenuation;

  // добавим отраженный свет
  float RdotVpow = max(pow(dot(reflect(-lightDir, normal), viewDir), material.shininess), 0.0);
  color += material.specular * light[i].specular * RdotVpow * attenuation;
}
  // вычислим итоговый цвет пикселя на экране с учетом текстуры
  color *= texture(material.texture, vertex.texcoord);
}