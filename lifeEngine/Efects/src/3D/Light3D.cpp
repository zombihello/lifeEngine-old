#include "..\..\3D\Light3D.h"

//-------------------------------------------------------------------------//

le::Light3D::Light3D()
{
	Position = glm::vec3( 0.0f, 0.0f, 0.0f );
	Ambient = glm::vec4( 0.2f, 0.2f, 0.2f, 1.0f );
	Diffuse = glm::vec4( 0.6f, 0.6f, 0.6f, 1.0f );
	Specular = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	Attenuation = glm::vec3( 0.5f, 0.0f, 0.0f );

	LightSphere.InitSphere( 150, 15 );
	LightSphere.SetPosition( Position );
}

//-------------------------------------------------------------------------//

le::Light3D::Light3D( float Radius, glm::vec3 Position, glm::vec4 Ambient, glm::vec4 Diffuse, glm::vec4 Specular, glm::vec3 Attenuation )
{
	this->Position = Position;
	this->Ambient = Ambient;
	this->Diffuse = Diffuse;
	this->Specular = Specular;
	this->Attenuation = Attenuation;

	LightSphere.InitSphere( Radius, 15 );
	LightSphere.SetPosition( Position );
}

//-------------------------------------------------------------------------//