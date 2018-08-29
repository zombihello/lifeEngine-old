#include <System\ResourcesManager.h>
#include <System\VAO.h>

#include <Graphics\Scene.h>
#include <Graphics\Level\Level.h>
#include <Graphics\Level\BSPInfo.h>
#include <Graphics\Light\LightManager.h>

//-------------------------------------------------------------------------//

le::LightManager::LightManager() :
	Scene( NULL ),
	ShadowMapRender( NULL )
{
	ShadowMapRender = ResourcesManager::GetShader( "ShadowMap" );
}

//-------------------------------------------------------------------------//

le::LightManager::~LightManager()
{
	if ( Scene != NULL )
		Scene->RemoveLightManager( this );
}

//-------------------------------------------------------------------------//

void le::LightManager::AddLightsToScene( le::Scene& Scene )
{
	Scene.AddLightManager( this );
}

//-------------------------------------------------------------------------//

void le::LightManager::BuildShadowMaps( Level& Level, map<GLuint, vector<le::Scene::InfoMesh*> >& GeometryStaticModels, map<GLuint, vector<le::Scene::InfoMesh*> >& GeometryAnimationModels )
{
	if ( ShadowMapRender == NULL )
		return;

	glCullFace( GL_FRONT );

	int									OffsetX = 0;
	int									OffsetY = 0;
	vector<InfoBSPPolygon>*				StaticLevel = &Level.GetVisablePlanes();
	vector<InfoBSPModel>*				DynamicLevel = &Level.GetVisableModels();

	Shader::bind( ShadowMapRender );
	ShadowMapRender->setUniform( "IsPointLight", true );

	// *****************************************
	// Строим карту теней для точечных источников

	for ( size_t IdPointLight = 0; IdPointLight < PointLights.size(); IdPointLight++ )
		if ( PointLights[ IdPointLight ].InitShadowMap( true ) )
		{
			PointLight* PointLight = &PointLights[ IdPointLight ];

			glBindFramebuffer( GL_FRAMEBUFFER, PointLight->ShadowMap_FBO );
			glClear( GL_DEPTH_BUFFER_BIT );

			ShadowMapRender->setUniform( "LightPosition", PointLight->Position );
			ShadowMapRender->setUniform( "FarPlane", PointLight->Radius );

			for ( int Face = 0; Face < 6; Face++ )
			{
				ShadowMapRender->setUniform( "LightMatrices", PointLight->LightTransforms[ Face ] );
				ShadowMapRender->setUniform( "Transformation", glm::mat4() );
				glViewport( OffsetX * Configuration::ShadowMapSize, OffsetY * Configuration::ShadowMapSize, Configuration::ShadowMapSize, Configuration::ShadowMapSize );

				VAO::BindVAO( Level.GetArrayBuffer() );

				// *****************************************
				// Рендерим статичную часть уровня

				for ( size_t IdInfoPolygon = 0; IdInfoPolygon < StaticLevel->size(); IdInfoPolygon++ )
				{
					InfoBSPPolygon*								Info = &StaticLevel->at( IdInfoPolygon );
					le::Plane*									Plane;

					for ( size_t IdPolygon = 0; IdPolygon < Info->RenderPlanes.size(); IdPolygon++ )
					{
						Plane = Info->RenderPlanes[ IdPolygon ];
						glDrawRangeElementsBaseVertex( GL_TRIANGLES, 0, Plane->NumberIndices, Plane->NumberIndices, GL_UNSIGNED_INT, ( void* ) ( Plane->StartIndex * sizeof( unsigned int ) ), Plane->StartVertex );
					}
				}

				// *****************************************
				// Рендерим динамическую часть уровня

				for ( size_t IdInfoModel = 0; IdInfoModel < DynamicLevel->size(); IdInfoModel++ )
				{
					InfoBSPModel*								Info = &DynamicLevel->at( IdInfoModel );
					map<glm::mat4*, vector<Plane*> >*			Planes = &Info->RenderPlanes;
					le::Plane*									Plane;

					for ( auto ItPlanes = Planes->begin(); ItPlanes != Planes->end(); ItPlanes++ )
					{
						ShadowMapRender->setUniform( "Transformation", *ItPlanes->first );

						for ( size_t IdPolygon = 0; IdPolygon < ItPlanes->second.size(); IdPolygon++ )
						{
							Plane = ItPlanes->second[ IdPolygon ];
							glDrawRangeElementsBaseVertex( GL_TRIANGLES, 0, Plane->NumberIndices, Plane->NumberIndices, GL_UNSIGNED_INT, ( void* ) ( Plane->StartIndex * sizeof( unsigned int ) ), Plane->StartVertex );
						}
					}
				}

				// *****************************************
				// Рендерим анимируемые модели
				// TODO: [zombiHello] Добавить анимацию моделям

				for ( auto It = GeometryAnimationModels.begin(); It != GeometryAnimationModels.end(); It++ )
					for ( size_t IdAnimationModel = 0; IdAnimationModel < It->second.size(); IdAnimationModel++ )
					{
						Scene::InfoMesh* InfoMesh = It->second[ IdAnimationModel ];

						if ( !*InfoMesh->IsRender ) continue;

						ShadowMapRender->setUniform( "Transformation", *InfoMesh->MatrixTransformation );
						VAO::BindVAO( InfoMesh->VertexArray );
						glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
					}

				// *****************************************
				// Рендерим статичные модели

				for ( auto It = GeometryStaticModels.begin(); It != GeometryStaticModels.end(); It++ )
					for ( size_t IdStaticModel = 0; IdStaticModel < It->second.size(); IdStaticModel++ )
					{
						Scene::InfoMesh* InfoMesh = It->second[ IdStaticModel ];

						if ( !*InfoMesh->IsRender ) continue;

						ShadowMapRender->setUniform( "Transformation", *InfoMesh->MatrixTransformation );
						VAO::BindVAO( InfoMesh->VertexArray );
						glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
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

	// *****************************************
	// Строим карту теней для прожекторных источников

	glViewport( 0, 0, Configuration::ShadowMapSize, Configuration::ShadowMapSize );
	ShadowMapRender->setUniform( "IsPointLight", false );

	for ( size_t IdSpotLight = 0; IdSpotLight < SpotLights.size(); IdSpotLight++ )
		if ( SpotLights[ IdSpotLight ].InitShadowMap() )
		{
			SpotLight* SpotLight = &SpotLights[ IdSpotLight ];

			glBindFramebuffer( GL_FRAMEBUFFER, SpotLight->ShadowMap_FBO );
			glClear( GL_DEPTH_BUFFER_BIT );

			ShadowMapRender->setUniform( "LightMatrices", SpotLight->LightTransforms[ 0 ] );
			ShadowMapRender->setUniform( "Transformation", glm::mat4() );

			VAO::BindVAO( Level.GetArrayBuffer() );

			// *****************************************
			// Рендерим статичную часть уровня

			for ( size_t IdInfoPolygon = 0; IdInfoPolygon < StaticLevel->size(); IdInfoPolygon++ )
			{
				InfoBSPPolygon*								Info = &StaticLevel->at( IdInfoPolygon );
				le::Plane*									Plane;

				for ( size_t IdPolygon = 0; IdPolygon < Info->RenderPlanes.size(); IdPolygon++ )
				{
					Plane = Info->RenderPlanes[ IdPolygon ];
					glDrawRangeElementsBaseVertex( GL_TRIANGLES, 0, Plane->NumberIndices, Plane->NumberIndices, GL_UNSIGNED_INT, ( void* ) ( Plane->StartIndex * sizeof( unsigned int ) ), Plane->StartVertex );
				}
			}

			// *****************************************
			// Рендерим динамическую часть уровня

			for ( size_t IdInfoModel = 0; IdInfoModel < DynamicLevel->size(); IdInfoModel++ )
			{
				InfoBSPModel*								Info = &DynamicLevel->at( IdInfoModel );
				map<glm::mat4*, vector<Plane*> >*			Planes = &Info->RenderPlanes;
				le::Plane*									Plane;

				for ( auto ItPlanes = Planes->begin(); ItPlanes != Planes->end(); ItPlanes++ )
				{
					ShadowMapRender->setUniform( "Transformation", *ItPlanes->first );

					for ( size_t IdPolygon = 0; IdPolygon < ItPlanes->second.size(); IdPolygon++ )
					{
						Plane = ItPlanes->second[ IdPolygon ];
						glDrawRangeElementsBaseVertex( GL_TRIANGLES, 0, Plane->NumberIndices, Plane->NumberIndices, GL_UNSIGNED_INT, ( void* ) ( Plane->StartIndex * sizeof( unsigned int ) ), Plane->StartVertex );
					}
				}
			}

			// *****************************************
			// Рендерим анимируемые модели
			// TODO: [zombiHello] Добавить анимацию моделям

			for ( auto It = GeometryAnimationModels.begin(); It != GeometryAnimationModels.end(); It++ )
				for ( size_t IdAnimationModel = 0; IdAnimationModel < It->second.size(); IdAnimationModel++ )
				{
					Scene::InfoMesh* InfoMesh = It->second[ IdAnimationModel ];

					if ( !InfoMesh->IsRender ) continue;

					ShadowMapRender->setUniform( "Transformation", *InfoMesh->MatrixTransformation );
					VAO::BindVAO( InfoMesh->VertexArray );
					glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
				}

			// *****************************************
			// Рендерим статичные модели

			for ( auto It = GeometryStaticModels.begin(); It != GeometryStaticModels.end(); It++ )
				for ( size_t IdStaticModel = 0; IdStaticModel < It->second.size(); IdStaticModel++ )
				{
					Scene::InfoMesh* InfoMesh = It->second[ IdStaticModel ];

					if ( !*InfoMesh->IsRender ) continue;

					ShadowMapRender->setUniform( "Transformation", *InfoMesh->MatrixTransformation );
					VAO::BindVAO( InfoMesh->VertexArray );
					glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
				}
		}

	// *****************************************
	// Строим карту теней для направленых источников

	for ( size_t IdDirectionalLight = 0; IdDirectionalLight < DirectionalLights.size(); IdDirectionalLight++ )
		if ( DirectionalLights[ IdDirectionalLight ].InitShadowMap() )
		{
			DirectionalLight* DirectionalLight = &DirectionalLights[ IdDirectionalLight ];

			glBindFramebuffer( GL_FRAMEBUFFER, DirectionalLight->ShadowMap_FBO );
			glClear( GL_DEPTH_BUFFER_BIT );

			ShadowMapRender->setUniform( "LightMatrices", DirectionalLight->LightTransforms[ 0 ] );
			ShadowMapRender->setUniform( "Transformation", glm::mat4() );

			VAO::BindVAO( Level.GetArrayBuffer() );

			// *****************************************
			// Рендерим статичную часть уровня

			for ( size_t IdInfoPolygon = 0; IdInfoPolygon < StaticLevel->size(); IdInfoPolygon++ )
			{
				InfoBSPPolygon*								Info = &StaticLevel->at( IdInfoPolygon );
				le::Plane*									Plane;

				for ( size_t IdPolygon = 0; IdPolygon < Info->RenderPlanes.size(); IdPolygon++ )
				{
					Plane = Info->RenderPlanes[ IdPolygon ];
					glDrawRangeElementsBaseVertex( GL_TRIANGLES, 0, Plane->NumberIndices, Plane->NumberIndices, GL_UNSIGNED_INT, ( void* ) ( Plane->StartIndex * sizeof( unsigned int ) ), Plane->StartVertex );
				}
			}

			// *****************************************
			// Рендерим динамическую часть уровня

			for ( size_t IdInfoModel = 0; IdInfoModel < DynamicLevel->size(); IdInfoModel++ )
			{
				InfoBSPModel*								Info = &DynamicLevel->at( IdInfoModel );
				map<glm::mat4*, vector<Plane*> >*			Planes = &Info->RenderPlanes;
				le::Plane*									Plane;

				for ( auto ItPlanes = Planes->begin(); ItPlanes != Planes->end(); ItPlanes++ )
				{
					ShadowMapRender->setUniform( "Transformation", *ItPlanes->first );

					for ( size_t IdPolygon = 0; IdPolygon < ItPlanes->second.size(); IdPolygon++ )
					{
						Plane = ItPlanes->second[ IdPolygon ];
						glDrawRangeElementsBaseVertex( GL_TRIANGLES, 0, Plane->NumberIndices, Plane->NumberIndices, GL_UNSIGNED_INT, ( void* ) ( Plane->StartIndex * sizeof( unsigned int ) ), Plane->StartVertex );
					}
				}
			}

			// *****************************************
			// Рендерим анимируемые модели
			// TODO: [zombiHello] Добавить анимацию моделям

			for ( auto It = GeometryAnimationModels.begin(); It != GeometryAnimationModels.end(); It++ )
				for ( size_t IdAnimationModel = 0; IdAnimationModel < It->second.size(); IdAnimationModel++ )
				{
					Scene::InfoMesh* InfoMesh = It->second[ IdAnimationModel ];

					if ( !InfoMesh->IsRender ) continue;

					ShadowMapRender->setUniform( "Transformation", *InfoMesh->MatrixTransformation );
					VAO::BindVAO( InfoMesh->VertexArray );
					glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
				}

			// *****************************************
			// Рендерим статичные модели

			for ( auto It = GeometryStaticModels.begin(); It != GeometryStaticModels.end(); It++ )
				for ( size_t IdStaticModel = 0; IdStaticModel < It->second.size(); IdStaticModel++ )
				{
					Scene::InfoMesh* InfoMesh = It->second[ IdStaticModel ];

					if ( !*InfoMesh->IsRender ) continue;

					ShadowMapRender->setUniform( "Transformation", *InfoMesh->MatrixTransformation );
					VAO::BindVAO( InfoMesh->VertexArray );
					glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
				}
		}

	Shader::bind( NULL );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
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
