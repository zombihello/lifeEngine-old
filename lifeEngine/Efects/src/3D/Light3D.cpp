#include "..\..\3D\Light3D.h"

//-------------------------------------------------------------------------//

le::Light3D::Light3D()
{
	Specular.w = 1;
	fIntensivity = 1;
	Color = glm::vec4( 0.6f, 0.6f, 0.6f, 1.0f );

	fRadius = 25;
	LightSphere.InitSphere( fRadius, 15 );
	LightSphere.SetPosition( Position );
}

//-------------------------------------------------------------------------//

le::Light3D::Light3D( glm::vec3 Position, float Radius, float Intensivity, glm::vec4 Color, glm::vec4 Specular )
{
	this->Position = Position;
	fIntensivity = Intensivity;
	this->Color = Color / 255.f;
	this->Specular = Specular / 255.f;
	fRadius = Radius;

	LightSphere.InitSphere( fRadius, 15 );
	LightSphere.SetPosition( Position );
}

//-------------------------------------------------------------------------//

void le::Light3D::SetRadius( float Radius )
{
	fRadius = Radius;
	LightSphere.SetRadius( fRadius );
}

//-------------------------------------------------------------------------//

void le::Light3D::SetIntensivity( float Intensivity )
{
	fIntensivity = Intensivity;
}

//-------------------------------------------------------------------------//

void le::Light3D::SetPosition( glm::vec3 Position )
{
	this->Position = Position;
	LightSphere.SetPosition( Position );
}

//-------------------------------------------------------------------------//

void le::Light3D::SetColor( glm::vec4 Color )
{
	this->Color = Color / 255.f;
}

//-------------------------------------------------------------------------//

void le::Light3D::SetSpecular( glm::vec4 Specular )
{
	this->Specular = Specular / 255.f;
}

//-------------------------------------------------------------------------//

void le::Light3D::SetName( string NameLight )
{
	this->NameLight = NameLight;
}

//-------------------------------------------------------------------------//