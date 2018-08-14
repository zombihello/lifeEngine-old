#include <System\VAO.h>
#include <System\ResourcesManager.h>
#include <Graphics\Model.h>
#include <Graphics\Camera.h>
#include <Graphics\Level.h>
#include <Graphics\LightManager.h>
#include <Graphics\Skybox.h>
#include "..\Scene.h"

//-------------------------------------------------------------------------//

le::Scene::Scene() :
	Visible_PointLight( 0 ),
	Visible_SpotLight( 0 ),
	Visible_Models( 0 ),
	ViewMatrix( NULL ),
	Frustum( NULL ),
	Camera( NULL ),
	LightManager( NULL ),
	PointLights( NULL ),
	DirectionalLights( NULL ),
	SpotLights( NULL ),
	Skybox( NULL ),
	PositionCamera( NULL ),
	PointLightRender( NULL )
{
	ProjectionMatrix = &System::Configuration.ProjectionMatrix;
	PVMatrix = *ProjectionMatrix;

	string PointLight_FragmentShader;
	string SpotLight_FragmentShader;
	string DirectionalLight_FragmentShader;

	switch ( System::Configuration.QualityShadows )
	{
	case LightManager::None:
		PointLight_FragmentShader = "../shaders/light/PointLightRender_ShadowNone.fs";
		SpotLight_FragmentShader = "../shaders/light/SpotLightRender_ShadowNone.fs";
		DirectionalLight_FragmentShader = "../shaders/light/DirectionalLightRender_ShadowNone.fs";
		break;

	case LightManager::Low:
		PointLight_FragmentShader = "../shaders/light/PointLightRender_ShadowLow.fs";
		SpotLight_FragmentShader = "../shaders/light/SpotLightRender_ShadowLow.fs";
		DirectionalLight_FragmentShader = "../shaders/light/DirectionalLightRender_ShadowLow.fs";
		break;

	case LightManager::Medium:
		PointLight_FragmentShader = "../shaders/light/PointLightRender_ShadowMedium.fs";
		SpotLight_FragmentShader = "../shaders/light/SpotLightRender_ShadowMedium.fs";
		DirectionalLight_FragmentShader = "../shaders/light/DirectionalLightRender_ShadowMedium.fs";
		break;

	default:
	case LightManager::High:
		PointLight_FragmentShader = "../shaders/light/PointLightRender_ShadowHigh.fs";
		SpotLight_FragmentShader = "../shaders/light/SpotLightRender_ShadowHigh.fs";
		DirectionalLight_FragmentShader = "../shaders/light/DirectionalLightRender_ShadowHigh.fs";
		break;
	}

	ResourcesManager::LoadShader( "AnimationModels", "../shaders/geometry/AnimationModelsRender.vs", "../shaders/geometry/AnimationModelsRender.fs" );
	ResourcesManager::LoadShader( "StaticModels", "../shaders/geometry/StaticModelsRender.vs", "../shaders/geometry/StaticModelsRender.fs" );
	ResourcesManager::LoadShader( "Brushes", "../shaders/geometry/LevelRender.vs", "../shaders/geometry/LevelRender.fs" );
	ResourcesManager::LoadShader( "TestRender", "../shaders/TestRender.vs", "../shaders/TestRender.fs" );
	ResourcesManager::LoadShader( "PointLight", "../shaders/light/PointLightRender.vs", PointLight_FragmentShader );
	ResourcesManager::LoadShader( "DirectionalLight", "../shaders/light/DirectionalLightRender.vs", DirectionalLight_FragmentShader );
	ResourcesManager::LoadShader( "SpotLight", "../shaders/light/SpotLightRender.vs", SpotLight_FragmentShader );

	AnimationModelsRender = ResourcesManager::GetShader( "AnimationModels" );
	StaticModelsRender = ResourcesManager::GetShader( "StaticModels" );
	LevelRender = ResourcesManager::GetShader( "Brushes" );
	TestRender = ResourcesManager::GetShader( "TestRender" );
	PointLightRender = ResourcesManager::GetShader( "PointLight" );
	DirectionalLightRender = ResourcesManager::GetShader( "DirectionalLight" );
	SpotLightRender = ResourcesManager::GetShader( "SpotLight" );

	if ( LevelRender != NULL )
	{
		LevelRender->setUniform( "ColorMap", 0 );
		LevelRender->setUniform( "LightMap", 1 );
	}

	if ( PointLightRender != NULL )
	{
		PointLightRender->setUniform( "ScreenSize", System::Configuration.WindowSize );
		PointLightRender->setUniform( "ColorMap", GBuffer::Textures );
		PointLightRender->setUniform( "PositionMap", GBuffer::Position );
		PointLightRender->setUniform( "NormalMap", GBuffer::Normal );
		PointLightRender->setUniform( "ShadowMap", 3 );
	}

	if ( SpotLightRender != NULL )
	{
		SpotLightRender->setUniform( "ScreenSize", System::Configuration.WindowSize );
		SpotLightRender->setUniform( "ColorMap", GBuffer::Textures );
		SpotLightRender->setUniform( "PositionMap", GBuffer::Position );
		SpotLightRender->setUniform( "NormalMap", GBuffer::Normal );
		SpotLightRender->setUniform( "ShadowMap", 3 );
	}

	if ( DirectionalLightRender != NULL )
	{
		DirectionalLightRender->setUniform( "ScreenSize", System::Configuration.WindowSize );
		DirectionalLightRender->setUniform( "ColorMap", GBuffer::Textures );
		DirectionalLightRender->setUniform( "PositionMap", GBuffer::Position );
		DirectionalLightRender->setUniform( "NormalMap", GBuffer::Normal );
		DirectionalLightRender->setUniform( "ShadowMap", 3 );
	}

	GBuffer.InitGBuffer( System::Configuration.WindowSize );
	LightQuad.InitQuad( 1.f );
}

//-------------------------------------------------------------------------//

le::Scene::~Scene()
{
	ResourcesManager::DeleteShader( "AnimationModels" );
	ResourcesManager::DeleteShader( "StaticModels" );
	ResourcesManager::DeleteShader( "Brushes" );
	ResourcesManager::DeleteShader( "TestRender" );
	ResourcesManager::DeleteShader( "PointLight" );
	ResourcesManager::DeleteShader( "DirectionalLight" );
	ResourcesManager::DeleteShader( "SpotLight" );
}

//-------------------------------------------------------------------------//

void le::Scene::AddModelToScene( Model* Model )
{
	map<GLuint, InfoMesh>* RenderMesh = &Model->GetRenderMesh();
	ModelsInScene.push_back( Model );

	if ( Model->IsNoSkeleton() )
	{
		for ( auto it = RenderMesh->begin(); it != RenderMesh->end(); it++ )
			RenderBuffer_StaticModel[ it->first ].push_back( &it->second );
	}
	else
	{
		for ( auto it = RenderMesh->begin(); it != RenderMesh->end(); it++ )
			RenderBuffer_AnimationModel[ it->first ].push_back( &it->second );
	}

	Model->SetScene( this );
}

//-------------------------------------------------------------------------//

void le::Scene::RemoveModelFromScene( Model* Model )
{
	map<GLuint, InfoMesh>* RenderMesh = &Model->GetRenderMesh();
	map<GLuint, vector<InfoMesh*>>* RenderBuffer_Models;

	if ( Model->IsNoSkeleton() )
		RenderBuffer_Models = &RenderBuffer_StaticModel;
	else
		RenderBuffer_Models = &RenderBuffer_AnimationModel;

	vector<InfoMesh*>* InfoMeshes;

	for ( auto it = RenderMesh->begin(); it != RenderMesh->end(); it++ )
	{
		InfoMeshes = &( *RenderBuffer_Models )[ it->first ];

		for ( size_t i = 0; i < InfoMeshes->size(); i++ )
			if ( ( *InfoMeshes )[ i ] == &it->second )
			{
				InfoMeshes->erase( InfoMeshes->begin() + i );
				break;
			}
	}

	for ( size_t i = 0; i < ModelsInScene.size(); i++ )
		if ( ModelsInScene[ i ] == Model )
		{
			ModelsInScene.erase( ModelsInScene.begin() + i );
			break;
		}

	Model->SetScene( NULL );
}

//-------------------------------------------------------------------------//

void le::Scene::AddLevelToScene( Level* Level )
{
	LevelInScene = Level;

	Skybox = &Level->GetSkybox();

	if ( Camera != NULL )
		Skybox->SetPlayerCamera( *Camera );

	Level->SetScene( this );
	RenderBuffer_Level = &Level->GetAllPlanes();
}

//-------------------------------------------------------------------------//

void le::Scene::RemoveLevelFromScene( Level* Level )
{
	Level->SetScene( NULL );
	RenderBuffer_Level = NULL;
	Skybox->RemovePlayerCamera();
	Skybox = NULL;
	LevelInScene = NULL;
}

//-------------------------------------------------------------------------//

void le::Scene::AddLightManagerToScene( le::LightManager* LightManager )
{
	PointLights = &LightManager->GetAllPointLight();
	DirectionalLights = &LightManager->GetAllDirectionalLight();
	SpotLights = &LightManager->GetAllSpotLight();

	LightManager->SetScene( this );
	this->LightManager = LightManager;
}

//-------------------------------------------------------------------------//

void le::Scene::RemoveLightManagerFromScene( le::LightManager* LightManager )
{
	PointLights = NULL;
	DirectionalLights = NULL;
	SpotLights = NULL;

	LightManager->SetScene( NULL );
	this->LightManager = NULL;
}

//-------------------------------------------------------------------------//

void le::Scene::RemoveCamera()
{
	ViewMatrix = NULL;
	Frustum = NULL;
	Camera = NULL;
	PositionCamera = NULL;

	if ( Skybox != NULL )
		Skybox->RemovePlayerCamera();
}

//-------------------------------------------------------------------------//

void le::Scene::RenderScene()
{
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );

	if ( Camera )
		PVMatrix = *ProjectionMatrix * ( *ViewMatrix );

	// ****************************
	// Проверка видимости геометрии
	// ****************************
	FrustumCulling();

	// ****************************
	// Рендер геометрии сцены
	// ****************************
	GeometryRender();

	// ****************************
	// Просчитывание освещения
	// ****************************
	//GBuffer.RenderFrame();

	if ( Configuration::IsWireframeRender || LightManager == NULL )
		GBuffer.RenderFrame( GBuffer::Textures );
	else
	{
		BuildShadowMaps();
		LightRender();
	}

	Shader::bind( NULL );
	VAO::UnbindVAO();
	GBuffer.Unbind();

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
}

//-------------------------------------------------------------------------//

void le::Scene::ClearScene()
{
	Logger::Log( Logger::Info, "Scene Is Clearing" );

	// TODO: [zombiHello] Сделать очистку сцены
	Logger::Log( Logger::Warning, "Method le::Scene::ClearScene() Is Empty" );

	Logger::Log( Logger::Info, "Scene Is Cleared" );
}

//-------------------------------------------------------------------------//

void le::Scene::SetCamera( le::Camera& Camera )
{
	ViewMatrix = &Camera.GetViewMatrix();
	Frustum = &Camera.GetFrustum();
	PositionCamera = &Camera.GetPosition();
	this->Camera = &Camera;

	if ( Skybox != NULL )
		Skybox->SetPlayerCamera( Camera );
}

//-------------------------------------------------------------------------//

le::GBuffer& le::Scene::GetGBuffer()
{
	return GBuffer;
}

//-------------------------------------------------------------------------//

size_t& le::Scene::GetVisible_PointLight()
{
	return Visible_PointLight;
}

//-------------------------------------------------------------------------//

size_t& le::Scene::GetVisible_SpotLight()
{
	return Visible_SpotLight;
}

//-------------------------------------------------------------------------//

size_t& le::Scene::GetVisible_Models()
{
	return Visible_Models;
}

//-------------------------------------------------------------------------//

map<GLuint, vector<le::Plane*>>& le::Scene::GetRenderBuffer_Level()
{
	return *RenderBuffer_Level;
}

//-------------------------------------------------------------------------//

vector<le::PointLight*>& le::Scene::GetLightBuffer_PointLight()
{
	return LightBuffer_PointLight;
}

//-------------------------------------------------------------------------//

vector<le::SpotLight*>& le::Scene::GetLightBuffer_SpotLight()
{
	return LightBuffer_SpotLight;
}

//-------------------------------------------------------------------------//

le::Level* le::Scene::GetLevel()
{
	return LevelInScene;
}

//-------------------------------------------------------------------------//

map<GLuint, vector<le::Scene::InfoMesh*>>& le::Scene::GetRenderBuffer_AnimationModel()
{
	return RenderBuffer_AnimationModel;
}

//-------------------------------------------------------------------------//

map<GLuint, vector<le::Scene::InfoMesh*>>& le::Scene::GetRenderBuffer_StaticModel()
{
	return RenderBuffer_StaticModel;
}

//-------------------------------------------------------------------------//

void le::Scene::FrustumCulling()
{
	if ( TestRender == NULL || Frustum == NULL || Camera == NULL )
		return;

	if ( Configuration::IsWireframeRender ) // отключаем каркасный рендер если он включен
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	Shader::bind( TestRender );
	TestRender->setUniform( "PVTMatrix", PVMatrix );
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );

	Visible_Models = Visible_PointLight = Visible_SpotLight = 0;

	// ***************************************** //
	// Проверка статичных моделей на отсечение по фрустуму

	for ( auto it = RenderBuffer_StaticModel.begin(); it != RenderBuffer_StaticModel.end(); it++ )
	{
		Ptr_GeometryBuffer = &it->second;

		for ( size_t i = 0; i < Ptr_GeometryBuffer->size(); i++ )
		{
			Ptr_InfoMesh = ( *Ptr_GeometryBuffer )[ i ];

			if ( !Frustum->IsVisible( *Ptr_InfoMesh->BoundingBox ) )
				Ptr_InfoMesh->IsRender = false;
			else
			{
				Ptr_InfoMesh->DistanceToCamera = Camera->GetDistance( *Ptr_InfoMesh->Position );

				if ( Ptr_InfoMesh->DistanceToCamera > System::Configuration.RenderDistance )
					continue;

				Ptr_InfoMesh->IsRender = true;

				if ( Visible_Models >= GeometryBuffer_Models.size() )
					GeometryBuffer_Models.push_back( Ptr_InfoMesh );
				else
					GeometryBuffer_Models[ Visible_Models ] = Ptr_InfoMesh;

				Visible_Models++;
			}
		}
	}

	// ***************************************** //
	// Проверка анимируемых моделей на отсечение по фрустуму

	for ( auto it = RenderBuffer_AnimationModel.begin(); it != RenderBuffer_AnimationModel.end(); it++ )
	{
		Ptr_GeometryBuffer = &it->second;

		for ( size_t i = 0; i < Ptr_GeometryBuffer->size(); i++ )
		{
			Ptr_InfoMesh = ( *Ptr_GeometryBuffer )[ i ];

			if ( !Frustum->IsVisible( *Ptr_InfoMesh->BoundingBox ) )
				Ptr_InfoMesh->IsRender = false;
			else
			{
				Ptr_InfoMesh->DistanceToCamera = Camera->GetDistance( *Ptr_InfoMesh->Position );

				if ( Ptr_InfoMesh->DistanceToCamera > System::Configuration.RenderDistance )
					continue;

				Ptr_InfoMesh->IsRender = true;

				if ( Visible_Models >= GeometryBuffer_Models.size() )
					GeometryBuffer_Models.push_back( Ptr_InfoMesh );
				else
					GeometryBuffer_Models[ Visible_Models ] = Ptr_InfoMesh;

				Visible_Models++;
			}
		}
	}

	// ***************************************** //
	// Проверка точечного освещения на отсечение по фрустуму

	for ( auto it = PointLights->begin(); it != PointLights->end(); it++ )
		if ( Frustum->IsVisible( it->LightSphere ) )
		{
			if ( PositionCamera != NULL )
			{
				// TODO: [zombiHello] - Неправильная проверка видимости у источников света
				float Distance = distance( *PositionCamera, it->LightSphere.GetPosition() );
				it->IsVisible = Distance > -it->Radius && Distance < it->Radius;
			}

			if ( Visible_PointLight >= LightBuffer_PointLight.size() )
				LightBuffer_PointLight.push_back( &( *it ) );
			else
				LightBuffer_PointLight[ Visible_PointLight ] = &( *it );

			Visible_PointLight++;
		}

	// ***************************************** //
	// Проверка прожекторного освещения на отсечение по фрустуму

	bool IsCameraInBox = false;
	bool IsCameraInCircle = false;

	for ( auto it = SpotLights->begin(); it != SpotLights->end(); it++ )
		if ( Frustum->IsVisible( it->LightCone.BoundingBox ) )
		{
			if ( PositionCamera != NULL )
			{
				glm::vec3 *MaxVertexBox = &it->LightCone.BoundingBox.GetMaxVertex();
				glm::vec3 *MinVertexBox = &it->LightCone.BoundingBox.GetMinVertex();
				float Distance = PositionCamera->z - it->Position.z;

				if ( PositionCamera->x > MinVertexBox->x && PositionCamera->x < MaxVertexBox->x )
					if ( PositionCamera->y > MinVertexBox->y && PositionCamera->y < MaxVertexBox->y )
						if ( PositionCamera->z > MinVertexBox->z && PositionCamera->z < MaxVertexBox->z )
							IsCameraInBox = true;

				IsCameraInCircle = Distance > -it->Radius && Distance < it->Radius;

				it->IsVisible = IsCameraInBox && IsCameraInCircle;
			}

			if ( Visible_SpotLight >= LightBuffer_SpotLight.size() )
				LightBuffer_SpotLight.push_back( &( *it ) );
			else
				LightBuffer_SpotLight[ Visible_SpotLight ] = &( *it );

			Visible_SpotLight++;
		}

	// ***************************************** //
	// Рендер ограничивающих тел моделей

	glDepthMask( GL_FALSE );

	for ( size_t i = 0; i < Visible_Models; i++ )
	{
		InfoMesh* InfoMesh = GeometryBuffer_Models[ i ];

		TestRender->setUniform( "PVTMatrix", PVMatrix * InfoMesh->BoundingBox->GetTransformation() );
		InfoMesh->BoundingBox->QueryTest();
	}

	// ***************************************** //
	// Рендер ограничивающих тел точечных источников света

	glDisable( GL_CULL_FACE );

	for ( size_t i = 0; i < Visible_PointLight; i++ )
	{
		PointLight* PointLight = LightBuffer_PointLight[ i ];

		TestRender->setUniform( "PVTMatrix", PVMatrix * PointLight->LightSphere.GetTransformation() );
		PointLight->LightSphere.QueryTest();
	}

	// ***************************************** //
	// Рендер ограничивающих тел прожекторных источников света

	for ( size_t i = 0; i < Visible_SpotLight; i++ )
	{
		SpotLight* SpotLight = LightBuffer_SpotLight[ i ];

		TestRender->setUniform( "PVTMatrix", PVMatrix * SpotLight->LightCone.GetTransformation() );
		SpotLight->LightCone.QueryTest();
	}

	glDepthMask( GL_TRUE );
	glEnable( GL_CULL_FACE );
	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

	if ( Configuration::IsWireframeRender ) // включаем каркасный рендер обратно если он был включен
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}

//-------------------------------------------------------------------------//

void le::Scene::BuildShadowMaps()
{
	if ( Camera )
		for ( size_t i = 0; i < DirectionalLights->size(); i++ )
			( *DirectionalLights )[ i ].SetCenter( Camera->GetPosition() ); // Перемещаем направленные источники света за камерой игрока

	// ***************************************** //
	// Обновление карт теней

	LightManager->BuildShadowMaps( true, true, true );
}

//-------------------------------------------------------------------------//

void le::Scene::GeometryRender()
{
	GBuffer.ClearFrame();

	// ****************************
	// Рендер скайбокса
	// ****************************

	if ( Skybox != NULL )
	{
		GBuffer.Bind( GBuffer::RenderSkybox );
		glDisable( GL_DEPTH_TEST );

		Skybox->RenderSkybox();

		glEnable( GL_DEPTH_TEST );
	}

	// ***************************************** //
	// Рендер брашей уровня

	GBuffer.Bind( GBuffer::RenderBuffers );
	glCullFace( GL_FRONT );

	if ( LevelRender != NULL && !RenderBuffer_Level->empty() )
	{
		Shader::bind( LevelRender );
		LevelRender->setUniform( "PVMatrix", PVMatrix );

		VAO::BindVAO( LevelInScene->GetArrayBuffer() );

		for ( auto It = RenderBuffer_Level->begin(); It != RenderBuffer_Level->end(); It++ )
		{
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, It->first );
			le::Plane* Plane;

			for ( size_t i = 0; i < It->second.size(); i++ )
			{
				Plane = It->second[ i ];

				glActiveTexture( GL_TEXTURE1 );
				glBindTexture( GL_TEXTURE_2D, Plane->Lightmap );
			
				glDrawRangeElements( GL_TRIANGLES, 0, Plane->NumberIndices, Plane->NumberIndices, GL_UNSIGNED_INT, ( void* ) ( Plane->StartIndex * sizeof( unsigned int ) ) );
			}
		}
	}

	// ***************************************** //
	// Рендер анимируемых моделей

	if ( AnimationModelsRender != NULL && !RenderBuffer_AnimationModel.empty() )
	{
		vector<le::Skeleton::Bone>* Bones;

		Shader::bind( AnimationModelsRender );
		AnimationModelsRender->setUniform( "PVMatrix", PVMatrix );

		for ( auto it = RenderBuffer_AnimationModel.begin(); it != RenderBuffer_AnimationModel.end(); it++ )
		{
			glBindTexture( GL_TEXTURE_2D, it->first );

			Ptr_GeometryBuffer = &it->second;

			for ( size_t i = 0; i < Ptr_GeometryBuffer->size(); i++ )
			{
				Ptr_InfoMesh = ( *Ptr_GeometryBuffer )[ i ];

				if ( !Ptr_InfoMesh->IsRender || Ptr_InfoMesh->BoundingBox->Query.GetResult() == 0 )
					continue;

				Bones = Ptr_InfoMesh->Skeleton->GetAllBones();
				AnimationModelsRender->setUniform( "TransformMatrix", *Ptr_InfoMesh->MatrixTransformation );

				for ( size_t j = 0; j < Bones->size(); j++ )
					AnimationModelsRender->setUniform( "Bones[" + to_string( j ) + "]", ( *Bones )[ j ].TransformMatrix );

				VAO::BindVAO( Ptr_InfoMesh->VertexArray );
				glDrawElements( GL_TRIANGLES, Ptr_InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
			}
		}
	}

	// ***************************************** //
	// Рендер статичных моделей (те которые не анимируются)

	if ( StaticModelsRender != NULL && !RenderBuffer_StaticModel.empty() )
	{
		Shader::bind( StaticModelsRender );
		StaticModelsRender->setUniform( "PVMatrix", PVMatrix );

		for ( auto it = RenderBuffer_StaticModel.begin(); it != RenderBuffer_StaticModel.end(); it++ )
		{
			glBindTexture( GL_TEXTURE_2D, it->first );
			Ptr_GeometryBuffer = &it->second;

			for ( size_t i = 0; i < Ptr_GeometryBuffer->size(); i++ )
			{
				Ptr_InfoMesh = ( *Ptr_GeometryBuffer )[ i ];

				if ( !Ptr_InfoMesh->IsRender || Ptr_InfoMesh->BoundingBox->Query.GetResult() == 0 )
					continue;

				StaticModelsRender->setUniform( "TransformMatrix", *Ptr_InfoMesh->MatrixTransformation );

				VAO::BindVAO( Ptr_InfoMesh->VertexArray );
				glDrawElements( GL_TRIANGLES, Ptr_InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
			}
		}
	}
}

//-------------------------------------------------------------------------//

void le::Scene::LightRender()
{
	if ( LightManager == NULL )
		return;

	GBuffer.Bind( GBuffer::RenderLight );

	glDepthMask( GL_FALSE );
	glDisable( GL_CULL_FACE );

	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_ONE, GL_ONE );

	glStencilOpSeparate( GL_FRONT, GL_KEEP, GL_INCR_WRAP, GL_KEEP );
	glStencilOpSeparate( GL_BACK, GL_KEEP, GL_DECR_WRAP, GL_KEEP );

	// ***************************************** //
	// Просчитываем точечные источники света

	glEnable( GL_STENCIL_TEST );

	if ( PointLightRender != NULL && TestRender != NULL )
		for ( size_t Index = 0; Index < Visible_PointLight; Index++ )
			RenderPointLight( Index );

	// ***************************************** //
	// Просчитываем прожекторные источники света

	if ( SpotLightRender != NULL && TestRender != NULL )
		for ( size_t Index = 0; Index < Visible_SpotLight; Index++ )
			RenderSpotLight( Index );

	glDisable( GL_STENCIL_TEST );

	// ***************************************** //
	// Просчитываем направленные источники света

	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );

	if ( DirectionalLights != NULL && DirectionalLightRender != NULL )
	{
		Shader::bind( DirectionalLightRender );

		for ( size_t i = 0; i < DirectionalLights->size(); i++ )
			RenderDirectionalLight( i );
	}

	glDisable( GL_BLEND );
	glEnable( GL_CULL_FACE );
	glDepthMask( GL_TRUE );

	GBuffer.RenderFrame();
}

//-------------------------------------------------------------------------//

void le::Scene::RenderPointLight( const size_t& Index )
{
	PointLight* PointLight = LightBuffer_PointLight[ Index ];

	if ( PointLight->LightSphere.Query.GetResult() > 0 || PositionCamera != NULL && PointLight->IsVisible )
	{
		glActiveTexture( GL_TEXTURE3 );
		glBindTexture( GL_TEXTURE_2D, PointLight->ShadowMap );

		glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
		Shader::bind( TestRender );

		glEnable( GL_DEPTH_TEST );
		glClear( GL_STENCIL_BUFFER_BIT );
		glStencilFunc( GL_ALWAYS, 0, 0 );

		TestRender->setUniform( "PVTMatrix", PVMatrix * PointLight->LightSphere.GetTransformation() );
		PointLightRender->setUniform( "light.Position", PointLight->Position );
		PointLightRender->setUniform( "light.Color", PointLight->Color );
		PointLightRender->setUniform( "light.Radius", PointLight->Radius );
		PointLightRender->setUniform( "light.Intensivity", PointLight->Intensivity );
		PointLight->LightSphere.RenderSphere();

		glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

		Shader::bind( PointLightRender );

		glStencilFunc( GL_NOTEQUAL, 0, 0xFF );
		glDisable( GL_DEPTH_TEST );

		glEnable( GL_BLEND );
		LightQuad.RenderQuad();
		glDisable( GL_BLEND );
	}
}

//-------------------------------------------------------------------------//

void le::Scene::RenderSpotLight( const size_t& Index )
{
	SpotLight* SpotLight = LightBuffer_SpotLight[ Index ];

	if ( SpotLight->LightCone.Query.GetResult() > 0 || PositionCamera != NULL && SpotLight->IsVisible )
	{
		glActiveTexture( GL_TEXTURE3 );
		glBindTexture( GL_TEXTURE_2D, SpotLight->ShadowMap );

		glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
		Shader::bind( TestRender );

		glEnable( GL_DEPTH_TEST );
		glClear( GL_STENCIL_BUFFER_BIT );
		glStencilFunc( GL_ALWAYS, 0, 0 );

		TestRender->setUniform( "PVTMatrix", PVMatrix * SpotLight->LightCone.GetTransformation() );
		SpotLightRender->setUniform( "light.LightMatrix", SpotLight->LightTransforms[ 0 ] );
		SpotLightRender->setUniform( "light.Position", SpotLight->Position );
		SpotLightRender->setUniform( "light.Intensivity", SpotLight->Intensivity );
		SpotLightRender->setUniform( "light.Color", SpotLight->Color );
		SpotLightRender->setUniform( "light.SpotDirection", SpotLight->SpotDirection );
		SpotLightRender->setUniform( "light.Height", SpotLight->Height );
		SpotLightRender->setUniform( "light.SpotCutoff", SpotLight->SpotCutoff );
		SpotLight->LightCone.RenderCone();

		glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

		Shader::bind( SpotLightRender );

		glStencilFunc( GL_NOTEQUAL, 0, 0xFF );
		glDisable( GL_DEPTH_TEST );

		glEnable( GL_BLEND );
		LightQuad.RenderQuad();
		glDisable( GL_BLEND );
	}
}

//-------------------------------------------------------------------------//

void le::Scene::RenderDirectionalLight( const size_t& Index )
{
	DirectionalLight* DirectionalLight = &( *DirectionalLights )[ Index ];

	glActiveTexture( GL_TEXTURE3 );
	glBindTexture( GL_TEXTURE_2D, DirectionalLight->ShadowMap );

	DirectionalLightRender->setUniform( "light.LightMatrix", DirectionalLight->LightTransforms[ 0 ] );
	DirectionalLightRender->setUniform( "light.Position", DirectionalLight->Position );
	DirectionalLightRender->setUniform( "light.Color", DirectionalLight->Color );
	DirectionalLightRender->setUniform( "light.Intensivity", DirectionalLight->Intensivity );
	DirectionalLight->Quad.RenderQuad();
}

//-------------------------------------------------------------------------//

le::Scene::InfoMesh::InfoMesh() :
	Skeleton( NULL ),
	MatrixTransformation( NULL ),
	Position( NULL ),
	DistanceToCamera( 0 ),
	IsRender( true )
{}

//-------------------------------------------------------------------------//