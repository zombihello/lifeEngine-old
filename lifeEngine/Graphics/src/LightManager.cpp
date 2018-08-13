#include <Graphics\Scene.h>
#include <Graphics\Level.h>
#include <System\ResourcesManager.h>
#include <System\VAO.h>
#include "..\LightManager.h"

//-------------------------------------------------------------------------//

le::LightManager::LightManager() :
	Scene( NULL ),
	ShadowMapRender( NULL )
{
	ResourcesManager::LoadShader( "ShadowMap", "../shaders/light/ShadowMapRender.vs", "../shaders/light/ShadowMapRender.fs" );

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
	InfoShadows.InitInfoShadows( Scene );
}

//-------------------------------------------------------------------------//

void le::LightManager::BuildShadowMaps( bool ShadowMap_PointLight, bool ShadowMap_SpotLight, bool ShadowMap_DirectionalLight )
{
	if ( ShadowMapRender == NULL || InfoShadows.IsEmpty || InfoShadows.GeometryLevel == NULL )
		return;

	bool IsDisable_DepthTest = !glIsEnabled( GL_DEPTH_TEST );
	bool IsDisable_CullFace = !glIsEnabled( GL_CULL_FACE );

	if ( IsDisable_DepthTest ) // Если выключен тест глубины - включаем
		glEnable( GL_DEPTH_TEST );

	if ( IsDisable_CullFace ) // Если выключено отсичение сторон - включаем
		glEnable( GL_CULL_FACE );

	glCullFace( GL_BACK );

	Shader::bind( ShadowMapRender );

	// ***************************************** //
	// Строим карту теней для точечных источников

	if ( ShadowMap_PointLight )
	{
		int		OffsetX = 0;
		int		OffsetY = 0;

		ShadowMapRender->setUniform( "IsPointLight", true );

		for ( size_t Index = 0; Index < *InfoShadows.Visible_PointLight; Index++ )
		{
			PointLight* PointLight = ( *InfoShadows.LightBuffer_PointLight )[ Index ];

			if ( PointLight->IsVisible && PointLight->InitShadowMap( true ) )
			{
				glBindFramebuffer( GL_FRAMEBUFFER, PointLight->ShadowMap_FBO );
				glClear( GL_DEPTH_BUFFER_BIT );

				OffsetX = OffsetY = 0;
				ShadowMapRender->setUniform( "LightPosition", PointLight->Position );
				ShadowMapRender->setUniform( "FarPlane", PointLight->Radius );

				for ( int Face = 0; Face < 6; Face++ )
				{
					glViewport( OffsetX * SHADOWMAP_SIZE, OffsetY * SHADOWMAP_SIZE, SHADOWMAP_SIZE, SHADOWMAP_SIZE );

					ShadowMapRender->setUniform( "LightMatrices", PointLight->LightTransforms[ Face ] );

					for ( auto It = InfoShadows.GeometryLevel->begin(); It != InfoShadows.GeometryLevel->end(); It++ )
						for ( size_t i = 0; i < It->second.size(); i++ )
						{
							Plane* Plane = It->second[ i ];
							glDrawElements( GL_TRIANGLES, Plane->NumberIndices, GL_UNSIGNED_INT, ( void* ) ( Plane->StartIndex * sizeof( unsigned int ) ) );
						}

					if ( OffsetX == 2 )
					{
						OffsetX = 0;
						OffsetY++;
					}
					else
						OffsetX++;
				}
			}
		}
	}

	// ***************************************** //
	// Строим карту теней для прожекторных источников

	glViewport( 0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE );
	ShadowMapRender->setUniform( "IsPointLight", false );

	if ( ShadowMap_SpotLight )
	{
		for ( size_t Index = 0; Index < *InfoShadows.Visible_SpotLight; Index++ )
		{
			SpotLight* SpotLight = ( *InfoShadows.LightBuffer_SpotLight )[ Index ];

			if ( SpotLight->IsVisible && SpotLight->InitShadowMap() )
			{
				glBindFramebuffer( GL_FRAMEBUFFER, SpotLight->ShadowMap_FBO );
				glClear( GL_DEPTH_BUFFER_BIT );

				ShadowMapRender->setUniform( "LightMatrices", SpotLight->LightTransforms[ 0 ] );

				for ( auto It = InfoShadows.GeometryLevel->begin(); It != InfoShadows.GeometryLevel->end(); It++ )
					for ( size_t i = 0; i < It->second.size(); i++ )
					{
						Plane* Plane = It->second[ i ];
						glDrawElements( GL_TRIANGLES, Plane->NumberIndices, GL_UNSIGNED_INT, ( void* ) ( Plane->StartIndex * sizeof( unsigned int ) ) );
					}
			}
		}
	}

	// ***************************************** //
	// Строим карту теней для направленых источников

	if ( ShadowMap_DirectionalLight )
	{
		bool IsVisibleObject;

		for ( size_t i = 0; i < DirectionalLights.size(); i++ )
			if ( DirectionalLights[ i ].InitShadowMap() )
			{
				DirectionalLight* DirectionalLight = &DirectionalLights[ i ];

				glBindFramebuffer( GL_FRAMEBUFFER, DirectionalLight->ShadowMap_FBO );
				glClear( GL_DEPTH_BUFFER_BIT );

				ShadowMapRender->setUniform( "LightMatrices", DirectionalLight->LightTransforms[ 0 ] );

				for ( auto It = InfoShadows.GeometryLevel->begin(); It != InfoShadows.GeometryLevel->end(); It++ )
					for ( size_t i = 0; i < It->second.size(); i++ )
					{
						Plane* Plane = It->second[ i ];
						glDrawElements( GL_TRIANGLES, Plane->NumberIndices, GL_UNSIGNED_INT, ( void* ) ( Plane->StartIndex * sizeof( unsigned int ) ) );
					}
			}
	}

	Shader::bind( NULL );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	if ( IsDisable_DepthTest )
		glDisable( GL_DEPTH_TEST );

	if ( IsDisable_CullFace )
		glDisable( GL_CULL_FACE );

	glViewport( 0, 0, ( GLsizei ) System::Configuration.WindowSize.x, ( GLsizei ) System::Configuration.WindowSize.y );
}

//-------------------------------------------------------------------------//

void le::LightManager::BuildShadowMaps( Level& Level, map<GLuint, vector<le::Scene::InfoMesh*> >& GeometryStaticModels, map<GLuint, vector<le::Scene::InfoMesh*> >& GeometryAnimationModels )
{
	if ( ShadowMapRender == NULL )
		return;

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );

	int									OffsetX = 0;
	int									OffsetY = 0;
	map<GLuint, vector<Plane*> >*		Brushes = &Level.GetAllPlanes();

	Shader::bind( ShadowMapRender );
	ShadowMapRender->setUniform( "IsPointLight", true );

	// ***************************************** //
	// Строим карту теней для точечных источников

	for ( size_t i = 0; i < PointLights.size(); i++ )
		if ( PointLights[ i ].InitShadowMap( true ) )
		{
			PointLight* PointLight = &PointLights[ i ];

			glBindFramebuffer( GL_FRAMEBUFFER, PointLight->ShadowMap_FBO );
			glClear( GL_DEPTH_BUFFER_BIT );

			ShadowMapRender->setUniform( "LightPosition", PointLight->Position );
			ShadowMapRender->setUniform( "FarPlane", PointLight->Radius );

			for ( int Face = 0; Face < 6; Face++ )
			{
				glViewport( OffsetX * SHADOWMAP_SIZE, OffsetY * SHADOWMAP_SIZE, SHADOWMAP_SIZE, SHADOWMAP_SIZE );
				ShadowMapRender->setUniform( "LightMatrices", PointLight->LightTransforms[ Face ] );

				for ( auto It = Brushes->begin(); It != Brushes->end(); It++ )
					for ( size_t i = 0; i < It->second.size(); i++ )
					{
						Plane* Plane = It->second[ i ];
						glDrawElements( GL_TRIANGLES, Plane->NumberIndices, GL_UNSIGNED_INT, ( void* ) ( Plane->StartIndex * sizeof( unsigned int ) ) );
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

	// ***************************************** //
	// Строим карту теней для прожекторных источников

	glViewport( 0, 0, SHADOWMAP_SIZE, SHADOWMAP_SIZE );
	ShadowMapRender->setUniform( "IsPointLight", false );

	for ( size_t i = 0; i < SpotLights.size(); i++ )
		if ( SpotLights[ i ].InitShadowMap() )
		{
			SpotLight* SpotLight = &SpotLights[ i ];

			glBindFramebuffer( GL_FRAMEBUFFER, SpotLight->ShadowMap_FBO );
			glClear( GL_DEPTH_BUFFER_BIT );

			ShadowMapRender->setUniform( "LightMatrices", SpotLight->LightTransforms[ 0 ] );

			for ( auto It = Brushes->begin(); It != Brushes->end(); It++ )
				for ( size_t i = 0; i < It->second.size(); i++ )
				{
					Plane* Plane = It->second[ i ];
					glDrawElements( GL_TRIANGLES, Plane->NumberIndices, GL_UNSIGNED_INT, ( void* ) ( Plane->StartIndex * sizeof( unsigned int ) ) );
				}
		}

	// ***************************************** //
	// Строим карту теней для направленых источников

	for ( size_t i = 0; i < DirectionalLights.size(); i++ )
		if ( DirectionalLights[ i ].InitShadowMap() )
		{
			DirectionalLight* DirectionalLight = &DirectionalLights[ i ];

			glBindFramebuffer( GL_FRAMEBUFFER, DirectionalLight->ShadowMap_FBO );
			glClear( GL_DEPTH_BUFFER_BIT );

			ShadowMapRender->setUniform( "LightMatrices", DirectionalLight->LightTransforms[ 0 ] );

			for ( auto It = Brushes->begin(); It != Brushes->end(); It++ )
				for ( size_t i = 0; i < It->second.size(); i++ )
				{
					Plane* Plane = It->second[ i ];
					glDrawElements( GL_TRIANGLES, Plane->NumberIndices, GL_UNSIGNED_INT, ( void* ) ( Plane->StartIndex * sizeof( unsigned int ) ) );
				}
		}

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

	if ( Scene == NULL )
		InfoShadows.ClearInfoShadows();
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

///////////////////////////////////////////////
//			СТРУКТУРА ИНФОРМАЦИИ О ТЕНЯХ
//////////////////////////////////////////////

//-------------------------------------------------------------------------//

le::LightManager::InfoShadows::InfoShadows() :
	Visible_PointLight( NULL ),
	Visible_SpotLight( NULL ),
	LightBuffer_PointLight( NULL ),
	LightBuffer_SpotLight( NULL ),
	GeometryLevel( NULL ),
	GeometryStaticModels( NULL ),
	GeometryAnimationModels( NULL ),
	IsEmpty( true )
{}

//-------------------------------------------------------------------------//

void le::LightManager::InfoShadows::InitInfoShadows( le::Scene& Scene )
{
	Level* Level = Scene.GetLevel();

	Visible_PointLight = &Scene.GetVisible_PointLight();
	Visible_SpotLight = &Scene.GetVisible_SpotLight();
	LightBuffer_PointLight = &Scene.GetLightBuffer_PointLight();
	LightBuffer_SpotLight = &Scene.GetLightBuffer_SpotLight();
	GeometryStaticModels = &Scene.GetRenderBuffer_StaticModel();
	GeometryAnimationModels = &Scene.GetRenderBuffer_AnimationModel();

	if ( Level != NULL )
		GeometryLevel = &Level->GetAllPlanes();
	else
		GeometryLevel = NULL;

	IsEmpty = false;
}

//-------------------------------------------------------------------------//

void le::LightManager::InfoShadows::ClearInfoShadows()
{
	Visible_PointLight = NULL;
	Visible_SpotLight = NULL;
	LightBuffer_PointLight = NULL;
	LightBuffer_SpotLight = NULL;
	GeometryLevel = NULL;
	GeometryStaticModels = NULL;
	GeometryAnimationModels = NULL;

	IsEmpty = true;
}

//-------------------------------------------------------------------------//
