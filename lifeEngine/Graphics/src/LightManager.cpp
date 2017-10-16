#include <Graphics\Scene.h>
#include "..\LightManager.h"

//-------------------------------------------------------------------------//

le::LightManager::LightManager() :
	Scene( NULL )
{}

//-------------------------------------------------------------------------//

le::LightManager::~LightManager()
{}

//-------------------------------------------------------------------------//

void le::LightManager::AddLightsToScene( le::Scene& Scene )
{
	Scene.AddLightManagerToScene( this );
}

//-------------------------------------------------------------------------//

void le::LightManager::AddPointLight( const string& NameLight, float Radius, float Intensivity, const glm::vec3& Position, const glm::vec4& Color, const glm::vec4& Specular )
{
	PointLights.push_back( PointLight( Radius, Intensivity, Position, Color, Specular ) );
	PointLights[ PointLights.size() - 1 ].NameLight = NameLight;
}

//-------------------------------------------------------------------------//

void le::LightManager::AddPointLight( const string& NameLight, const PointLight& PointLight )
{
	PointLights.push_back( PointLight );
	PointLights[ PointLights.size() - 1 ].NameLight = NameLight;
}

//-------------------------------------------------------------------------//

void le::LightManager::DestroyPointLight( const string& NameLight )
{
	for ( size_t i = 0; i < PointLights.size(); i++ )
		if ( PointLights[ i ].NameLight == NameLight )
		{
			PointLights.erase( i + PointLights.begin() );
			break;
		}
}

//-------------------------------------------------------------------------//

void le::LightManager::DestroyAllPointLights()
{
	PointLights.clear();
}

//-------------------------------------------------------------------------//

void le::LightManager::DestroyAllLights()
{
	PointLights.clear();
}

//-------------------------------------------------------------------------//

void le::LightManager::SetScene( le::Scene* Scene )
{
	this->Scene = Scene;
}

//-------------------------------------------------------------------------//

le::PointLight* le::LightManager::GetPointLight( const string& NameLight )
{
	for ( size_t i = 0; i < PointLights.size(); i++ )
		if ( PointLights[ i ].NameLight == NameLight )
			return &PointLights[ i ];

	return NULL;
}

//-------------------------------------------------------------------------//

vector<le::PointLight*> le::LightManager::GetPointLights( const string& NameLight )
{
	vector<PointLight*> FindLights;

	for ( size_t i = 0; i < PointLights.size(); i++ )
		if ( PointLights[ i ].NameLight == NameLight )
			FindLights.push_back( &PointLights[i] );

	return FindLights;
}

//-------------------------------------------------------------------------//

vector<le::PointLight>* le::LightManager::GetAllPointLights()
{
	return &PointLights;
}

//-------------------------------------------------------------------------//
