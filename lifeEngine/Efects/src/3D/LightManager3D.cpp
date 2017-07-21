#include "..\..\3D\LightManager3D.h"

//-------------------------------------------------------------------------//

le::LightManager3D::LightManager3D( le::Camera& PlayerCamera )
{
	this->PlayerCamera = &PlayerCamera;

	ShaderLight.loadFromFile( "vertexShader.vs", "fragmentShader.fs" );
}

//-------------------------------------------------------------------------//

void le::LightManager3D::UpdateLights()
{
//	ShaderLight.setUniform( "transformationMatrix" )
}

//-------------------------------------------------------------------------//

void le::LightManager3D::StartApplyLights()
{
//	Shader::bind( &ShaderLight );
}

//-------------------------------------------------------------------------//

void le::LightManager3D::StopApplyLights()
{
	//Shader::bind( NULL );
}

//-------------------------------------------------------------------------//

void le::LightManager3D::CreateLight( string NameLight, le::Light3D Light3D )
{}

//-------------------------------------------------------------------------//

void le::LightManager3D::DestroyLight( string NameLight )
{}

//-------------------------------------------------------------------------//

void le::LightManager3D::DestroyAllLights()
{
	mLights.clear();
}

//-------------------------------------------------------------------------//

void le::LightManager3D::SetPlayerCamera( le::Camera& PlayerCamera )
{
	this->PlayerCamera = &PlayerCamera;
}

//-------------------------------------------------------------------------//

le::Light3D* le::LightManager3D::GetLight( string NameLight )
{
	return NULL;
}

//-------------------------------------------------------------------------//