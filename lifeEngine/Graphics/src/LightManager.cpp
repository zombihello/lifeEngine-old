#include <Graphics\Scene.h>
#include <System\ResourcesManager.h>
#include <System\VAO.h>
#include "..\LightManager.h"

//-------------------------------------------------------------------------//

le::LightManager::LightManager() :
	Scene( NULL ),
	ShadowMapRender( NULL )
{
	ResourcesManager::LoadShader( "ShadowMap", "../shaders/ShadowMapRender.vs", "../shaders/ShadowMapRender.fs" );

	ShadowMapRender = ResourcesManager::GetShader( "ShadowMap" );
}

//-------------------------------------------------------------------------//

le::LightManager::~LightManager()
{
	ResourcesManager::DeleteShader( "ShadowMap" );

	if ( Scene != NULL )
		Scene->RemoveLightManagerFromScene( this );
}

//-------------------------------------------------------------------------//

void le::LightManager::AddLightsToScene( le::Scene& Scene )
{
	Scene.AddLightManagerToScene( this );
}

//-------------------------------------------------------------------------//

void le::LightManager::BuildShadowMaps()
{
	if ( Scene != NULL )
		BuildShadowMaps( Scene->GetRenderBuffer_Level(), Scene->GetRenderBuffer_StaticModel(), Scene->GetRenderBuffer_AnimationModel() );	
}

//-------------------------------------------------------------------------//

void le::LightManager::BuildShadowMaps( map<GLuint, vector<le::Scene::InfoMesh*> >& GeometryLevel, map<GLuint, vector<le::Scene::InfoMesh*> >& GeometryStaticModels, map<GLuint, vector<le::Scene::InfoMesh*> >& GeometryAnimationModels )
{
	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );

	int								OffsetX = 0;
	int								OffsetY = 0;

	le::Scene::InfoMesh*			Ptr_InfoMesh;
	vector<le::Scene::InfoMesh*>*	Ptr_GeometryBuffer;

	Shader::bind( ShadowMapRender );

	// ***************************************** //
	// Строим карту теней для точечных источников

	for ( size_t i = 0; i < PointLights.size(); i++ )
		if ( PointLights[ i ].InitShadowMap( true ) )
		{
			PointLight* PointLight = &PointLights[ i ];

			glBindFramebuffer( GL_FRAMEBUFFER, PointLight->ShadowMap_FBO );
			glClear( GL_DEPTH_BUFFER_BIT );

			for ( int Face = 0; Face < 6; Face++ )
			{
				glViewport( OffsetX * SHADOWMAP_SIZE, OffsetY * SHADOWMAP_SIZE, SHADOWMAP_SIZE, SHADOWMAP_SIZE );

				ShadowMapRender->setUniform( "LightMatrices", PointLight->LightTransforms[ Face ] );
				ShadowMapRender->setUniform( "LightPosition", PointLight->Position );
				ShadowMapRender->setUniform( "FarPlane", PointLight->Radius );

				for ( auto it = GeometryLevel.begin(); it != GeometryLevel.end(); it++ )
				{
					Ptr_GeometryBuffer = &it->second;

					for ( size_t j = 0; j < Ptr_GeometryBuffer->size(); j++ )
					{
						Ptr_InfoMesh = ( *Ptr_GeometryBuffer )[ j ];

						VAO::BindVAO( Ptr_InfoMesh->VertexArray );
						glDrawElements( GL_TRIANGLES, Ptr_InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
					}
				}

				if ( OffsetX == 2 )
				{
					OffsetX = 0;
					OffsetY++;
				}
				else
					OffsetX++;
			}
	
			OffsetX = OffsetY = 0;
		}

	//TODO: [zombiHello] Добавить тени от других видов света
	//TODO: [zombiHello] Сделать построение тени для стат. моделей
	//TODO: [zombiHello] Сделать построение тени для дин. моделей

	Shader::bind( NULL );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_CULL_FACE );

	glViewport( 0, 0, ( GLsizei ) System::Configuration.WindowSize.x, ( GLsizei ) System::Configuration.WindowSize.y );
}

//-------------------------------------------------------------------------//

void le::LightManager::AddPointLight( const string& NameLight, float Radius, const glm::vec3& Position, const glm::vec4& Color, float Intensivity, const glm::vec4& Specular )
{
	PointLights.push_back( PointLight( Radius, Position, Color, Intensivity, Specular ) );
	PointLights[ PointLights.size() - 1 ].NameLight = NameLight;
}

//-------------------------------------------------------------------------//

void le::LightManager::AddPointLight( const string& NameLight, const PointLight& PointLight )
{
	PointLights.push_back( PointLight );
	PointLights[ PointLights.size() - 1 ].NameLight = NameLight;
}

//-------------------------------------------------------------------------//

void le::LightManager::AddDirectionalLight( const string& NameLight, const glm::vec3& Position, const glm::vec4& Color, float Intensivity, const glm::vec4& Specular )
{
	DirectionalLights.push_back( DirectionalLight( Position, Color, Intensivity, Specular ) );
	DirectionalLights[ DirectionalLights.size() - 1 ].NameLight = NameLight;
}

//-------------------------------------------------------------------------//

void le::LightManager::AddDirectionalLight( const string& NameLight, const le::DirectionalLight& DirectionalLight )
{
	DirectionalLights.push_back( DirectionalLight );
	DirectionalLights[ DirectionalLights.size() - 1 ].NameLight = NameLight;
}

//-------------------------------------------------------------------------//

void le::LightManager::AddSpotLight( const string& NameLight, float Radius, float Height, const glm::vec3& Rotation, const glm::vec3& Position, const glm::vec4& Color, float Intensivity, const glm::vec4& Specular )
{
	SpotLights.push_back( SpotLight( Radius, Height, Rotation, Position, Color, Intensivity, Specular ) );
	SpotLights[ SpotLights.size() - 1 ].NameLight = NameLight;
}

//-------------------------------------------------------------------------//

void le::LightManager::AddSpotLight( const string& NameLight, const SpotLight& SpotLight )
{
	SpotLights.push_back( SpotLight );
	SpotLights[ SpotLights.size() - 1 ].NameLight = NameLight;
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

void le::LightManager::DestroyDirectionalLight( const string& NameLight )
{
	for ( size_t i = 0; i < DirectionalLights.size(); i++ )
		if ( DirectionalLights[ i ].NameLight == NameLight )
		{
			DirectionalLights.erase( i + DirectionalLights.begin() );
			break;
		}
}

//-------------------------------------------------------------------------//

void le::LightManager::DestroySpotLight( const string& NameLight )
{
	for ( size_t i = 0; i < SpotLights.size(); i++ )
		if ( SpotLights[ i ].NameLight == NameLight )
		{
			SpotLights.erase( i + SpotLights.begin() );
			break;
		}
}

//-------------------------------------------------------------------------//

void le::LightManager::DestroyAllPointLight()
{
	PointLights.clear();
}

//-------------------------------------------------------------------------//

void le::LightManager::DestroyAllDirectionalLight()
{
	DirectionalLights.clear();
}

//-------------------------------------------------------------------------//

void le::LightManager::DestroyAllSpotLight()
{
	SpotLights.clear();
}

//-------------------------------------------------------------------------//

void le::LightManager::DestroyAllLight()
{
	PointLights.clear();
	DirectionalLights.clear();
	SpotLights.clear();
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

le::DirectionalLight* le::LightManager::GetDirectionalLight( const string& NameLight )
{
	for ( size_t i = 0; i < DirectionalLights.size(); i++ )
		if ( DirectionalLights[ i ].NameLight == NameLight )
			return &DirectionalLights[ i ];

	return NULL;
}

//-------------------------------------------------------------------------//

le::SpotLight* le::LightManager::GetSpotLight( const string& NameLight )
{
	for ( size_t i = 0; i < SpotLights.size(); i++ )
		if ( SpotLights[ i ].NameLight == NameLight )
			return &SpotLights[ i ];

	return NULL;
}

//-------------------------------------------------------------------------//

vector<le::PointLight*> le::LightManager::GetPointLights( const string& NameLight )
{
	vector<PointLight*> FindLights;

	for ( size_t i = 0; i < PointLights.size(); i++ )
		if ( PointLights[ i ].NameLight == NameLight )
			FindLights.push_back( &PointLights[ i ] );

	return FindLights;
}

//-------------------------------------------------------------------------//

vector<le::DirectionalLight*> le::LightManager::GetDirectionalLights( const string& NameLight )
{
	vector<DirectionalLight*> FindLights;

	for ( size_t i = 0; i < DirectionalLights.size(); i++ )
		if ( DirectionalLights[ i ].NameLight == NameLight )
			FindLights.push_back( &DirectionalLights[ i ] );

	return FindLights;
}

//-------------------------------------------------------------------------//

vector<le::SpotLight*> le::LightManager::GetSpotLights( const string & NameLight )
{
	vector<SpotLight*> FindLights;

	for ( size_t i = 0; i < SpotLights.size(); i++ )
		if ( SpotLights[ i ].NameLight == NameLight )
			FindLights.push_back( &SpotLights[ i ] );

	return FindLights;
}

//-------------------------------------------------------------------------//

vector<le::SpotLight>& le::LightManager::GetAllSpotLight()
{
	return SpotLights;
}

//-------------------------------------------------------------------------//

vector<le::PointLight>& le::LightManager::GetAllPointLight()
{
	return PointLights;
}

//-------------------------------------------------------------------------//

vector<le::DirectionalLight>& le::LightManager::GetAllDirectionalLight()
{
	return DirectionalLights;
}

//-------------------------------------------------------------------------//
