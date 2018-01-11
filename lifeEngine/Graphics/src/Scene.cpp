﻿#include <System\VAO.h>
#include <Graphics\Model.h>
#include <Graphics\Camera.h>
#include <Graphics\Level.h>
#include <Graphics\LightManager.h>
#include <Graphics\Skybox.h>
#include "..\Scene.h"

//-------------------------------------------------------------------------//

inline bool sortFUNCTION( le::Scene::InfoMesh* InfoMesh_1, le::Scene::InfoMesh* InfoMesh_2 )
{
	return InfoMesh_1->DistanceToCamera < InfoMesh_2->DistanceToCamera;
}

//-------------------------------------------------------------------------//

le::Scene::Scene( System& System ) :
	Visible_PointLight( 0 ),
	Visible_SpotLight( 0 ),
	Visible_Models( 0 ),
	Visible_Brushes( 0 ),
	ViewMatrix( NULL ),
	Frustum( NULL ),
	Camera( NULL ),
	LightManager( NULL ),
	PointLights( NULL ),
	DirectionalLights( NULL ),
	SpotLights( NULL ),
	Skybox( NULL ),
	PositionCamera( NULL )
{
	ProjectionMatrix = &System.GetConfiguration().ProjectionMatrix;
	PVMatrix = *ProjectionMatrix;

	ResourcesManager::LoadShader( "AnimationModels", "../shaders/AnimationModelsRender.vs", "../shaders/AnimationModelsRender.fs" );
	ResourcesManager::LoadShader( "StaticModels", "../shaders/StaticModelsRender.vs", "../shaders/StaticModelsRender.fs" );
	ResourcesManager::LoadShader( "Brushes", "../shaders/LevelRender.vs", "../shaders/LevelRender.fs" );
	ResourcesManager::LoadShader( "TestRender", "../shaders/TestRender.vs", "../shaders/TestRender.fs" );
	ResourcesManager::LoadShader( "PointLight", "../shaders/PointLight.vs", "../shaders/PointLight.fs" );
	ResourcesManager::LoadShader( "DirectionalLight", "../shaders/DirectionalLightRender.vs", "../shaders/DirectionalLightRender.fs" );
	ResourcesManager::LoadShader( "SpotLight", "../shaders/SpotLightRender.vs", "../shaders/SpotLightRender.fs" );

	AnimationModelsRender = ResourcesManager::GetShader( "AnimationModels" );
	StaticModelsRender = ResourcesManager::GetShader( "StaticModels" );
	LevelRender = ResourcesManager::GetShader( "Brushes" );
	TestRender = ResourcesManager::GetShader( "TestRender" );
	PointLightRender = ResourcesManager::GetShader( "PointLight" );
	DirectionalLightRender = ResourcesManager::GetShader( "DirectionalLight" );
	SpotLightRender = ResourcesManager::GetShader( "SpotLight" );

	PointLightRender->setUniform( "ScreenSize", System.GetConfiguration().WindowSize );
	PointLightRender->setUniform( "ColorMap", GBuffer::Textures );
	PointLightRender->setUniform( "PositionMap", GBuffer::Position );
	PointLightRender->setUniform( "NormalMap", GBuffer::Normal );

	SpotLightRender->setUniform( "ScreenSize", System.GetConfiguration().WindowSize );
	SpotLightRender->setUniform( "ColorMap", GBuffer::Textures );
	SpotLightRender->setUniform( "PositionMap", GBuffer::Position );
	SpotLightRender->setUniform( "NormalMap", GBuffer::Normal );

	DirectionalLightRender->setUniform( "ScreenSize", System.GetConfiguration().WindowSize );
	DirectionalLightRender->setUniform( "ColorMap", GBuffer::Textures );
	DirectionalLightRender->setUniform( "NormalMap", GBuffer::Normal );

	GBuffer.InitGBuffer( System.GetConfiguration().WindowSize );
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
	vector<Level::Brush*>* Brushes = &Level->GetAllBrushes();
	map<GLuint, InfoMesh>* RenderMesh;

	for ( size_t i = 0; i < Brushes->size(); i++ )
	{
		Level::Brush* Brush = ( *Brushes )[ i ];
		RenderMesh = &Brush->GetRenderMesh();

		for ( auto it = RenderMesh->begin(); it != RenderMesh->end(); it++ )
			RenderBuffer_Level[ it->first ].push_back( &it->second );
	}

	Skybox = &Level->GetSkybox();

	if ( Camera != NULL )
		Skybox->SetPlayerCamera( *Camera );

	Level->SetScene( this );
}

//-------------------------------------------------------------------------//

void le::Scene::RemoveLevelFromScene( Level* Level )
{
	vector<Level::Brush*>* Brushes = &Level->GetAllBrushes();
	map<GLuint, InfoMesh>* RenderMesh;
	vector<InfoMesh*>* InfoMeshes;

	for ( size_t i = 0; i < Brushes->size(); i++ )
	{
		Level::Brush* Brush = ( *Brushes )[ i ];
		RenderMesh = &Brush->GetRenderMesh();

		for ( auto it = RenderMesh->begin(); it != RenderMesh->end(); it++ )
		{
			InfoMeshes = &RenderBuffer_Level[ it->first ];

			for ( size_t j = 0; j < InfoMeshes->size(); j++ )
				if ( ( *InfoMeshes )[ j ] == &it->second )
				{
					InfoMeshes->erase( InfoMeshes->begin() + j );
					break;
				}
		}
	}

	Level->SetScene( NULL );
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
	if ( Configuration::IsWireframeRender || LightManager == NULL )
		GBuffer.RenderFrame( GBuffer::Textures );
	else
		LightRender();

	Shader::bind( NULL );
	VAO::UnbindVAO();

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
}

//-------------------------------------------------------------------------//

void le::Scene::ClearScene()
{
	// TODO: [zombiHello] Сделать очистку сцены
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

void le::Scene::FrustumCulling()
{
	if ( TestRender == NULL || Frustum == NULL || Camera == NULL )
		return;

	if ( Configuration::IsWireframeRender ) // отключаем каркасный рендер если он включен
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	Shader::bind( TestRender );
	TestRender->setUniform( "PVTMatrix", PVMatrix );
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );

	Visible_Brushes = Visible_Models = Visible_PointLight = Visible_SpotLight = 0;

	// ***************************************** //
	// Проверка брашей на отсечение по фрустуму

	for ( auto it = RenderBuffer_Level.begin(); it != RenderBuffer_Level.end(); it++ )
	{
		Ptr_GeometryBuffer = &it->second;

		for ( size_t i = 0; i < Ptr_GeometryBuffer->size(); i++ )
		{
			Ptr_InfoMesh = ( *Ptr_GeometryBuffer )[ i ];

			if ( !Frustum->IsVisible( *Ptr_InfoMesh->BoundingBox ) )
				Ptr_InfoMesh->IsRender = false;
			else
			{
				Ptr_InfoMesh->IsRender = true;
				Ptr_InfoMesh->DistanceToCamera = Camera->GetDistance( *Ptr_InfoMesh->Position );

				if ( Visible_Brushes >= GeometryBuffer_Level.size() )
					GeometryBuffer_Level.push_back( Ptr_InfoMesh );
				else
					GeometryBuffer_Level[ Visible_Brushes ] = Ptr_InfoMesh;

				Visible_Brushes++;
			}
		}
	}

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
				Ptr_InfoMesh->IsRender = true;
				Ptr_InfoMesh->DistanceToCamera = Camera->GetDistance( *Ptr_InfoMesh->Position );

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
				Ptr_InfoMesh->IsRender = true;
				Ptr_InfoMesh->DistanceToCamera = Camera->GetDistance( *Ptr_InfoMesh->Position );

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
			if ( Visible_PointLight >= LightBuffer_PointLight.size() )
				LightBuffer_PointLight.push_back( &( *it ) );
			else
				LightBuffer_PointLight[ Visible_PointLight ] = &( *it );

			Visible_PointLight++;
		}

	// ***************************************** //
	// Проверка прожекторного освещения на отсечение по фрустуму

	for ( auto it = SpotLights->begin(); it != SpotLights->end(); it++ )
		if ( Frustum->IsVisible( it->LightCone.BoundingBox ) )
		{
			if ( Visible_SpotLight >= LightBuffer_SpotLight.size() )
				LightBuffer_SpotLight.push_back( &( *it ) );
			else
				LightBuffer_SpotLight[ Visible_SpotLight ] = &( *it );

			Visible_SpotLight++;
		}

	// ***************************************** //
	// Сортируем геометрию уровня по удалению от камеры

	sort( GeometryBuffer_Level.begin(), GeometryBuffer_Level.begin() + Visible_Brushes, sortFUNCTION );

	// ***************************************** //
	// Рендер ограничивающих тел брашей

	for ( size_t i = 0; i < Visible_Brushes; i++ )
		GeometryBuffer_Level[ i ]->BoundingBox->QueryTest();

	// ***************************************** //
	// Рендер ограничивающих тел моделей

	glDepthMask( GL_FALSE );

	for ( size_t i = 0; i < Visible_Models; i++ )
		GeometryBuffer_Models[ i ]->BoundingBox->QueryTest();

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

void le::Scene::GeometryRender()
{
	GBuffer.ClearFrame();
	GBuffer.Bind( GBuffer::RenderBuffers );

	// ***************************************** //
	// Рендер брашей уровня

	if ( LevelRender != NULL && !RenderBuffer_Level.empty() )
	{
		Shader::bind( LevelRender );
		LevelRender->setUniform( "PVMatrix", PVMatrix );

		for ( auto it = RenderBuffer_Level.begin(); it != RenderBuffer_Level.end(); it++ )
		{
			glBindTexture( GL_TEXTURE_2D, it->first );
			Ptr_GeometryBuffer = &it->second;

			for ( size_t i = 0; i < Ptr_GeometryBuffer->size(); i++ )
			{
				Ptr_InfoMesh = ( *Ptr_GeometryBuffer )[ i ];

				if ( !Ptr_InfoMesh->IsRender || Ptr_InfoMesh->BoundingBox->Query.GetResult() == 0 )
					continue;

				VAO::BindVAO( Ptr_InfoMesh->VertexArray );
				glDrawElements( GL_TRIANGLES, Ptr_InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
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

	// ****************************
	// Рендер скайбокса
	// ****************************

	if ( Skybox != NULL )
	{
		GBuffer.Bind( GBuffer::RenderSkybox );
		Skybox->RenderSkybox();
	}
}

//-------------------------------------------------------------------------//

void le::Scene::LightRender()
{
	if ( LightManager == NULL )
		return;

	glDepthMask( GL_FALSE );
	glEnable( GL_STENCIL_TEST );
	glDisable( GL_CULL_FACE );

	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_ONE, GL_ONE );

	glStencilOpSeparate( GL_FRONT, GL_KEEP, GL_INCR_WRAP, GL_KEEP );
	glStencilOpSeparate( GL_BACK, GL_KEEP, GL_DECR_WRAP, GL_KEEP );

	GBuffer.Bind( GBuffer::RenderLight );
	size_t CountPass;

	// ***************************************** //
	// Просчитываем точечные источники света

	CountPass = ( size_t ) ceil( Visible_PointLight / 10 ) + 1;

	if ( PointLightRender != NULL && TestRender != NULL )
		for ( size_t Pass = 0, Index = 0; Pass < CountPass; Pass++, Index += 10 )
			RenderPointLights( Index, Index + 10 );

	// ***************************************** //
	// Просчитываем прожекторные источники света

	CountPass = ( size_t ) ceil( Visible_SpotLight / 10 ) + 1;

	if ( SpotLightRender != NULL && TestRender != NULL )
		for ( size_t Pass = 0, Index = 0; Pass < CountPass; Pass++, Index += 10 )
			RenderSpotLights( Index, Index + 10 );

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

void le::Scene::RenderPointLights( const size_t& StartIndex, const size_t& EndIndex )
{
	bool IsCameraInCircle = false;

	int CountLights = 0;
	float Distance = 0;

	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	Shader::bind( TestRender );

	glEnable( GL_DEPTH_TEST );
	glClear( GL_STENCIL_BUFFER_BIT );
	glStencilFunc( GL_ALWAYS, 0, 0 );

	for ( size_t i = StartIndex; i < Visible_PointLight && i < EndIndex; i++ )
	{
		PointLight* PointLight = LightBuffer_PointLight[ i ];

		if ( PositionCamera != NULL )
		{
			Distance = distance( *PositionCamera, PointLight->LightSphere.GetPosition() );

			IsCameraInCircle = Distance > -PointLight->Radius && Distance < PointLight->Radius;
		}

		if ( PointLight->LightSphere.Query.GetResult() > 0 || PositionCamera != NULL && IsCameraInCircle )
		{
			TestRender->setUniform( "PVTMatrix", PVMatrix * PointLight->LightSphere.GetTransformation() );
			PointLightRender->setUniform( "light[" + to_string( CountLights ) + "].Position", PointLight->Position );
			PointLightRender->setUniform( "light[" + to_string( CountLights ) + "].Color", PointLight->Color );
			PointLightRender->setUniform( "light[" + to_string( CountLights ) + "].Radius", PointLight->Radius );
			PointLightRender->setUniform( "light[" + to_string( CountLights ) + "].Intensivity", PointLight->Intensivity );
			PointLight->LightSphere.RenderSphere();

			CountLights++;
		}
	}

	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

	if ( CountLights > 0 )
	{
		Shader::bind( PointLightRender );
		PointLightRender->setUniform( "CountLights", CountLights );

		glStencilFunc( GL_NOTEQUAL, 0, 0xFF );
		glDisable( GL_DEPTH_TEST );

		glEnable( GL_BLEND );
		LightQuad.RenderQuad();
		glDisable( GL_BLEND );
	}
}

//-------------------------------------------------------------------------//

void le::Scene::RenderSpotLights( const size_t& StartIndex, const size_t& EndIndex )
{
	bool IsCameraInBox = false;
	bool IsCameraInCircle = false;

	int CountLights = 0;
	float Distance = 0;
	glm::vec3 *MinVertexBox, *MaxVertexBox;

	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	Shader::bind( TestRender );

	glEnable( GL_DEPTH_TEST );
	glClear( GL_STENCIL_BUFFER_BIT );
	glStencilFunc( GL_ALWAYS, 0, 0 );

	for ( size_t i = StartIndex; i < Visible_SpotLight && i < EndIndex; i++ )
	{
		SpotLight* SpotLight = LightBuffer_SpotLight[ i ];

		if ( PositionCamera != NULL )
		{
			MaxVertexBox = &SpotLight->LightCone.BoundingBox.GetMaxVertex();
			MinVertexBox = &SpotLight->LightCone.BoundingBox.GetMinVertex();
			Distance = PositionCamera->z - SpotLight->Position.z;

			if ( PositionCamera->x > MinVertexBox->x && PositionCamera->x < MaxVertexBox->x )
				if ( PositionCamera->y > MinVertexBox->y && PositionCamera->y < MaxVertexBox->y )
					if ( PositionCamera->z > MinVertexBox->z && PositionCamera->z < MaxVertexBox->z )
						IsCameraInBox = true;

			IsCameraInCircle = Distance > -SpotLight->Radius && Distance < SpotLight->Radius;
		}

		if ( SpotLight->LightCone.Query.GetResult() > 0 || IsCameraInBox && IsCameraInCircle )
		{
			TestRender->setUniform( "PVTMatrix", PVMatrix * SpotLight->LightCone.GetTransformation() );
			SpotLightRender->setUniform( "light[" + to_string( CountLights ) + "].Position", SpotLight->Position );
			SpotLightRender->setUniform( "light[" + to_string( CountLights ) + "].Intensivity", SpotLight->Intensivity );
			SpotLightRender->setUniform( "light[" + to_string( CountLights ) + "].Color", SpotLight->Color );
			SpotLightRender->setUniform( "light[" + to_string( CountLights ) + "].SpotDirection", SpotLight->SpotDirection );
			SpotLightRender->setUniform( "light[" + to_string( CountLights ) + "].Height", SpotLight->LightCone.GetHeight() );
			SpotLightRender->setUniform( "light[" + to_string( CountLights ) + "].SpotCutoff", SpotLight->SpotCutoff );
			SpotLight->LightCone.RenderCone();

			CountLights++;
		}
	}

	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

	if ( CountLights > 0 )
	{
		Shader::bind( SpotLightRender );
		SpotLightRender->setUniform( "CountLights", CountLights );

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
