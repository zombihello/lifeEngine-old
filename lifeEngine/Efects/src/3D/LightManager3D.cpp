#include "..\..\3D\LightManager3D.h"

//-------------------------------------------------------------------------//

le::LightManager3D::LightManager3D() : Scene( NULL )
{}

//-------------------------------------------------------------------------//

void le::LightManager3D::AddLightsToScene( Scene3D & Scene )
{
	this->Scene = &Scene;
}

//-------------------------------------------------------------------------//

void le::LightManager3D::UpdateLights()
{
	if ( Scene != NULL )
		for ( int i = 0; i < vLights.size(); i++ )
			Scene->AddLightToScene( vLights[ i ] );
}

//-------------------------------------------------------------------------//

void le::LightManager3D::CreateLight( le::Light3D Light3D )
{
	vLights.push_back( Light3D );
}

//-------------------------------------------------------------------------//

void le::LightManager3D::DestroyLight( string NameLight )
{
	for ( int i = 0; i < vLights.size(); i++ )
		if ( vLights[ i ].NameLight == NameLight )
			vLights.erase( i + vLights.begin() );
}

//-------------------------------------------------------------------------//

void le::LightManager3D::DestroyAllLights()
{
	vLights.clear();
}

//-------------------------------------------------------------------------//

le::Light3D* le::LightManager3D::GetLight( string NameLight )
{
	for ( int i = 0; i < vLights.size(); i++ )
		if ( vLights[ i ].NameLight == NameLight )
			return &vLights[ i ];

	return NULL;
}

//-------------------------------------------------------------------------//
