#include <System\VAO.h>
#include <System\ResourcesManager.h>
#include <Graphics\Model.h>
#include <Graphics\Camera.h>
#include <Graphics\Level.h>
#include <Graphics\LightManager.h>
#include <Graphics\Skybox.h>
#include <Graphics\BSPInfo.h>
#include "..\Scene.h"

//-------------------------------------------------------------------------//

le::Scene::Scene() :
	ViewMatrix( NULL ),
	Frustum( NULL ),
	ActiveCamera( NULL ),
	LightManager( NULL ),
	PointLights( NULL ),
	DirectionalLights( NULL ),
	SpotLights( NULL ),
	Skybox( NULL ),
	PositionCamera( NULL )
{
	ProjectionMatrix = &System::Configuration.ProjectionMatrix;
	PVMatrix = *ProjectionMatrix;

	AnimationModelsRender = ResourcesManager::GetShader( "AnimationModels" );
	StaticModelsRender = ResourcesManager::GetShader( "StaticModels" );
	LevelRender = ResourcesManager::GetShader( "Brushes" );

	AnimationModelsRender_GBuffer = ResourcesManager::GetShader( "AnimationModels_GBuffer" );
	StaticModelsRender_GBuffer = ResourcesManager::GetShader( "StaticModels_GBuffer" );
	LevelRender_GBuffer = ResourcesManager::GetShader( "Brushes_GBuffer" );

	TestRender = ResourcesManager::GetShader( "TestRender" );
	PointLightRender = ResourcesManager::GetShader( "PointLight" );
	DirectionalLightRender = ResourcesManager::GetShader( "DirectionalLight" );
	SpotLightRender = ResourcesManager::GetShader( "SpotLight" );

	if ( LevelRender != NULL )
	{
		LevelRender->setUniform( "ColorMap", 0 );
		LevelRender->setUniform( "LightMap", 1 );

		LevelRender_GBuffer->setUniform( "ColorMap", 0 );
		LevelRender_GBuffer->setUniform( "LightMap", 1 );
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
	for ( size_t IdModel = 0; IdModel < Models.size(); IdModel++ )
		Models[ IdModel ]->SetScene( NULL );

	Level->SetScene( NULL );
	LightManager->SetScene( NULL );
}

//-------------------------------------------------------------------------//

void le::Scene::AddModel( Model* Model )
{
	map<GLuint, InfoMesh>* RenderMesh = &Model->GetRenderMesh();
	Models.push_back( Model );

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

void le::Scene::RemoveModel( Model* Model )
{
	map<GLuint, InfoMesh>*				RenderMesh = &Model->GetRenderMesh();
	map<GLuint, vector<InfoMesh*>>*		RenderBuffer_Models;
	vector<InfoMesh*>*					InfoMeshes;

	if ( Model->IsNoSkeleton() )
		RenderBuffer_Models = &RenderBuffer_StaticModel;
	else
		RenderBuffer_Models = &RenderBuffer_AnimationModel;

	for ( auto it = RenderMesh->begin(); it != RenderMesh->end(); it++ )
	{
		InfoMeshes = &RenderBuffer_Models->at( it->first );

		for ( size_t i = 0; i < InfoMeshes->size(); i++ )
			if ( InfoMeshes->at( i ) == &it->second )
			{
				InfoMeshes->erase( InfoMeshes->begin() + i );
				break;
			}
	}

	for ( size_t i = 0; i < Models.size(); i++ )
		if ( Models[ i ] == Model )
		{
			Models.erase( Models.begin() + i );
			break;
		}

	Model->SetScene( NULL );
}

//-------------------------------------------------------------------------//

void le::Scene::AddLevel( le::Level* Level )
{
	this->Level = Level;
	Skybox = &Level->GetSkybox();

	if ( ActiveCamera != NULL )
		Skybox->SetPlayerCamera( *ActiveCamera );

	Level->SetScene( this );
	RenderBuffer_Level = &Level->GetVisablePlanes();
}

//-------------------------------------------------------------------------//

void le::Scene::RemoveLevel()
{
	Level->SetScene( NULL );
	Skybox->RemovePlayerCamera();
	Skybox = NULL;
	Level = NULL;
	RenderBuffer_Level = NULL;
}

//-------------------------------------------------------------------------//

void le::Scene::AddLightManager( le::LightManager* LightManager )
{
	PointLights = &LightManager->GetAllPointLight();
	DirectionalLights = &LightManager->GetAllDirectionalLight();
	SpotLights = &LightManager->GetAllSpotLight();

	LightManager->SetScene( this );
	this->LightManager = LightManager;
}

//-------------------------------------------------------------------------//

void le::Scene::RemoveLightManager( le::LightManager* LightManager )
{
	PointLights = NULL;
	DirectionalLights = NULL;
	SpotLights = NULL;

	LightManager->SetScene( NULL );
	this->LightManager = NULL;
}

//-------------------------------------------------------------------------//

void le::Scene::RemoveCamera( const string& NameCamera )
{
	if ( CamerasInScene.find( NameCamera ) == CamerasInScene.end() ) return;

	if ( CamerasInScene[ NameCamera ] == ActiveCamera )
	{
		ViewMatrix = NULL;
		Frustum = NULL;
		ActiveCamera = NULL;
		PositionCamera = NULL;

		if ( Skybox != NULL )
			Skybox->RemovePlayerCamera();
	}
	else
		CamerasInScene.erase( NameCamera );
}

//-------------------------------------------------------------------------//

void le::Scene::RemoveAllCameras()
{
	CamerasInScene.clear();

	ViewMatrix = NULL;
	Frustum = NULL;
	ActiveCamera = NULL;
	PositionCamera = NULL;

	if ( Skybox != NULL )
		Skybox->RemovePlayerCamera();
}

//-------------------------------------------------------------------------//

void le::Scene::Render()
{
	bool IsVisiblePointLight = false, IsVisibleSpotLight = false;

	if ( ActiveCamera )
	{
		ActiveCamera->UpdateViewMatrix();
		PVMatrix = *ProjectionMatrix * ( *ViewMatrix );

		// ****************************
		// Проверка видимости геометрии

		if ( Level )
		{
			Level->CalculateVisablePlanes( *ActiveCamera );
			Level->CalculateVisableModels( Models, *Frustum );
			IsVisiblePointLight = Level->CalculateVisableLights( *PointLights, *Frustum );
			IsVisibleSpotLight = Level->CalculateVisableLights( *SpotLights, *Frustum );
		}
	}

	// ****************************
	// Рендер сцены

	if ( IsVisiblePointLight || IsVisibleSpotLight || !DirectionalLights->empty() )
		GeometryRender_GBuffer();
	else if ( !IsVisiblePointLight && !IsVisibleSpotLight && DirectionalLights->empty() )
		GeometryRender();

	Shader::bind( NULL );
	VAO::UnbindVAO();
}

//-------------------------------------------------------------------------//

void le::Scene::Clear()
{
	Logger::Log( Logger::Info, "Scene Is Clearing" );

	for ( size_t IdModel = 0; IdModel < Models.size(); IdModel++ )
		Models[ IdModel ]->SetScene( NULL );

	Level->SetScene( NULL );
	LightManager->SetScene( NULL );

	PositionCamera = NULL;
	ViewMatrix = NULL;
	ProjectionMatrix = NULL;
	Frustum = NULL;
	ActiveCamera = NULL;
	Level = NULL;
	LightManager = NULL;
	Skybox = NULL;
	PointLights = NULL;
	SpotLights = NULL;
	DirectionalLights = NULL;
	RenderBuffer_Level = NULL;

	CamerasInScene.clear();
	RenderBuffer_AnimationModel.clear();
	RenderBuffer_StaticModel.clear();

	Logger::Log( Logger::Info, "Scene Is Cleared" );
}

//-------------------------------------------------------------------------//

le::Camera* le::Scene::SetActiveCamera( const string& NameCamera )
{
	ActiveCamera = CamerasInScene[ NameCamera ];

	ViewMatrix = &ActiveCamera->GetViewMatrix();
	Frustum = &ActiveCamera->GetFrustum();
	PositionCamera = &ActiveCamera->GetPosition();

	if ( Skybox != NULL )
		Skybox->SetPlayerCamera( *ActiveCamera );

	return ActiveCamera;
}

//-------------------------------------------------------------------------//

void le::Scene::SetCamera( const string& NameCamera, le::Camera& Camera )
{
	ViewMatrix = &Camera.GetViewMatrix();
	Frustum = &Camera.GetFrustum();
	PositionCamera = &Camera.GetPosition();
	CamerasInScene[ NameCamera ] = &Camera;
	this->ActiveCamera = &Camera;

	if ( Skybox != NULL )
		Skybox->SetPlayerCamera( Camera );
}

//-------------------------------------------------------------------------//

le::GBuffer& le::Scene::GetGBuffer()
{
	return GBuffer;
}

//-------------------------------------------------------------------------//

le::Level* le::Scene::GetLevel()
{
	return Level;
}

//-------------------------------------------------------------------------//

void le::Scene::BuildShadowMaps()
{
	if ( ActiveCamera )
		for ( size_t Id = 0; Id < DirectionalLights->size(); Id++ )
			DirectionalLights->at( Id ).SetCenter( ActiveCamera->GetPosition() ); // Перемещаем направленные источники света за камерой игрока

	// ***************************************** //
	// Обновление карт теней

	LightManager->BuildShadowMaps( *Level, RenderBuffer_StaticModel, RenderBuffer_AnimationModel );
}

//-------------------------------------------------------------------------//

void le::Scene::GeometryRender()
{
	// ****************************
	// Рендер скайбокса

	if ( Skybox != NULL )
	{
		glDisable( GL_DEPTH_TEST );
		Skybox->RenderSkybox();
		glEnable( GL_DEPTH_TEST );
	}

	// *****************************************
	// Рендер брашей уровня

	glCullFace( GL_FRONT );

	if ( LevelRender != NULL && RenderBuffer_Level != NULL )
	{
		Shader::bind( LevelRender );
		VAO::BindVAO( Level->GetArrayBuffer() );

		for ( size_t IdInfoPolygon = 0; IdInfoPolygon < RenderBuffer_Level->size(); IdInfoPolygon++ )
		{
			InfoBSPPolygon*								Info = &RenderBuffer_Level->at( IdInfoPolygon );
			map<glm::mat4*, vector<Plane*> >*			Planes = &Info->RenderPlanes;
			le::Plane*									Plane;

			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, Info->Texture );

			for ( auto ItPlanes = Planes->begin(); ItPlanes != Planes->end(); ItPlanes++ )
			{
				if ( ItPlanes->first )
					LevelRender->setUniform( "PVTMatrix", PVMatrix * *ItPlanes->first );
				else
					LevelRender->setUniform( "PVTMatrix", PVMatrix );

				for ( size_t IdPolygon = 0; IdPolygon < ItPlanes->second.size(); IdPolygon++ )
				{
					Plane = ItPlanes->second[ IdPolygon ];

					glActiveTexture( GL_TEXTURE1 );
					glBindTexture( GL_TEXTURE_2D, Plane->Lightmap );

					glDrawRangeElementsBaseVertex( GL_TRIANGLES, 0, Plane->NumberIndices, Plane->NumberIndices, GL_UNSIGNED_INT, ( void* ) ( Plane->StartIndex * sizeof( unsigned int ) ), Plane->StartVertex );
				}
			}
		}
	}

	// *****************************************
	// Рендер анимируемых моделей

	glCullFace( GL_BACK );

	if ( AnimationModelsRender != NULL && !RenderBuffer_AnimationModel.empty() )
	{
		vector<Skeleton::Bone>*						Bones;
		Shader::bind( AnimationModelsRender );

		for ( auto ItAnimationModel = RenderBuffer_AnimationModel.begin(); ItAnimationModel != RenderBuffer_AnimationModel.end(); ItAnimationModel++ )
		{
			vector<InfoMesh*>*						GeometryBuffer = &ItAnimationModel->second;
			glBindTexture( GL_TEXTURE_2D, ItAnimationModel->first );

			for ( size_t Id = 0; Id < GeometryBuffer->size(); Id++ )
			{
				InfoMesh*							InfoMesh = GeometryBuffer->at( Id );

				if ( !*InfoMesh->IsRender ) continue;

				PVTMatrix = PVMatrix * *InfoMesh->MatrixTransformation;
				Bones = InfoMesh->Skeleton->GetAllBones();
				AnimationModelsRender->setUniform( "PVTMatrix", PVTMatrix );

				for ( size_t IdBone = 0; IdBone < Bones->size(); IdBone++ )
					AnimationModelsRender->setUniform( "Bones[" + to_string( IdBone ) + "]", ( *Bones )[ IdBone ].TransformMatrix );

				VAO::BindVAO( InfoMesh->VertexArray );
				glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
			}
		}
	}

	// *****************************************
	// Рендер статичных моделей (те которые не анимируются)

	if ( StaticModelsRender != NULL && !RenderBuffer_StaticModel.empty() )
	{
		Shader::bind( StaticModelsRender );

		for ( auto IdStaticModel = RenderBuffer_StaticModel.begin(); IdStaticModel != RenderBuffer_StaticModel.end(); IdStaticModel++ )
		{
			vector<InfoMesh*>*						GeometryBuffer = &IdStaticModel->second;
			glBindTexture( GL_TEXTURE_2D, IdStaticModel->first );

			for ( size_t Id = 0; Id < GeometryBuffer->size(); Id++ )
			{
				InfoMesh*							InfoMesh = GeometryBuffer->at( Id );

				if ( !*InfoMesh->IsRender ) continue;

				PVTMatrix = PVMatrix * *InfoMesh->MatrixTransformation;
				StaticModelsRender->setUniform( "PVTMatrix", PVTMatrix );

				VAO::BindVAO( InfoMesh->VertexArray );
				glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
			}
		}
	}
}

//-------------------------------------------------------------------------//

void le::Scene::GeometryRender_GBuffer()
{
	GBuffer.ClearFrame();

	// ****************************
	// Рендер скайбокса

	glCullFace( GL_BACK );

	if ( Skybox != NULL )
	{
		GBuffer.Bind( GBuffer::RenderSkybox );

		glDisable( GL_DEPTH_TEST );
		Skybox->RenderSkybox();
		glEnable( GL_DEPTH_TEST );
	}

	// *****************************************
	// Рендер анимируемых моделей

	GBuffer.Bind( GBuffer::RenderBuffers );

	if ( AnimationModelsRender_GBuffer != NULL && !RenderBuffer_AnimationModel.empty() )
	{
		vector<Skeleton::Bone>*					Bones;

		Shader::bind( AnimationModelsRender_GBuffer );
		AnimationModelsRender_GBuffer->setUniform( "PVMatrix", PVMatrix );

		for ( auto ItAnimationModel = RenderBuffer_AnimationModel.begin(); ItAnimationModel != RenderBuffer_AnimationModel.end(); ItAnimationModel++ )
		{
			vector<InfoMesh*>*					GeometryBuffer = &ItAnimationModel->second;
			glBindTexture( GL_TEXTURE_2D, ItAnimationModel->first );

			for ( size_t Id = 0; Id < GeometryBuffer->size(); Id++ )
			{
				InfoMesh*						InfoMesh = GeometryBuffer->at( Id );

				if ( !*InfoMesh->IsRender ) continue;

				Bones = InfoMesh->Skeleton->GetAllBones();
				AnimationModelsRender_GBuffer->setUniform( "TransformMatrix", *InfoMesh->MatrixTransformation );

				for ( size_t IdBone = 0; IdBone < Bones->size(); IdBone++ )
					AnimationModelsRender_GBuffer->setUniform( "Bones[" + to_string( IdBone ) + "]", ( *Bones )[ IdBone ].TransformMatrix );

				VAO::BindVAO( InfoMesh->VertexArray );
				glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
			}
		}
	}

	// *****************************************
	// Рендер статичных моделей (те которые не анимируются)

	if ( StaticModelsRender_GBuffer != NULL && !RenderBuffer_StaticModel.empty() )
	{
		Shader::bind( StaticModelsRender_GBuffer );
		StaticModelsRender_GBuffer->setUniform( "PVMatrix", PVMatrix );

		for ( auto IdStaticModel = RenderBuffer_StaticModel.begin(); IdStaticModel != RenderBuffer_StaticModel.end(); IdStaticModel++ )
		{
			vector<InfoMesh*>*				GeometryBuffer = &IdStaticModel->second;
			glBindTexture( GL_TEXTURE_2D, IdStaticModel->first );

			for ( size_t Id = 0; Id < GeometryBuffer->size(); Id++ )
			{
				InfoMesh*					InfoMesh = GeometryBuffer->at( Id );

				if ( !*InfoMesh->IsRender ) continue;

				StaticModelsRender_GBuffer->setUniform( "TransformMatrix", *InfoMesh->MatrixTransformation );

				VAO::BindVAO( InfoMesh->VertexArray );
				glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
			}
		}
	}

	// ***************************************** //
	// Рендер брашей уровня

	glCullFace( GL_FRONT );

	if ( LevelRender_GBuffer != NULL && RenderBuffer_Level != NULL )
	{
		Shader::bind( LevelRender_GBuffer );
		VAO::BindVAO( Level->GetArrayBuffer() );

		for ( size_t IdInfoPolygon = 0; IdInfoPolygon < RenderBuffer_Level->size(); IdInfoPolygon++ )
		{
			InfoBSPPolygon*								Info = &RenderBuffer_Level->at( IdInfoPolygon );
			map<glm::mat4*, vector<Plane*> >*			Planes = &Info->RenderPlanes;
			le::Plane*									Plane;

			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D, Info->Texture );

			for ( auto ItPlanes = Planes->begin(); ItPlanes != Planes->end(); ItPlanes++ )
			{
				if ( ItPlanes->first )
					LevelRender_GBuffer->setUniform( "PVTMatrix", PVMatrix * *ItPlanes->first );
				else
					LevelRender_GBuffer->setUniform( "PVTMatrix", PVMatrix );

				for ( size_t IdPolygon = 0; IdPolygon < ItPlanes->second.size(); IdPolygon++ )
				{
					Plane = ItPlanes->second[ IdPolygon ];

					glActiveTexture( GL_TEXTURE1 );
					glBindTexture( GL_TEXTURE_2D, Plane->Lightmap );

					glDrawRangeElementsBaseVertex( GL_TRIANGLES, 0, Plane->NumberIndices, Plane->NumberIndices, GL_UNSIGNED_INT, ( void* ) ( Plane->StartIndex * sizeof( unsigned int ) ), Plane->StartVertex );
				}
			}
		}
	}

	// ************************************************
	// Обновляем карты теней и просчитываем освещение

	BuildShadowMaps();
	CalculateLight();

	GBuffer.RenderFrame();
	GBuffer.Unbind();
}

//-------------------------------------------------------------------------//

void le::Scene::CalculateLight()
{
	GBuffer.Bind( GBuffer::RenderLight );

	glDepthMask( GL_FALSE );
	glCullFace( GL_FRONT );

	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_ONE, GL_ONE );

	glStencilOpSeparate( GL_FRONT, GL_KEEP, GL_INCR_WRAP, GL_KEEP );
	glStencilOpSeparate( GL_BACK, GL_KEEP, GL_DECR_WRAP, GL_KEEP );

	// ***************************************** //
	// Просчитываем точечные источники света

	glEnable( GL_STENCIL_TEST );

	if ( PointLightRender != NULL && TestRender != NULL )
	{
		PointLightRender->setUniform( "ViewPosition", ActiveCamera->GetPosition() );

		for ( size_t Index = 0; Index < PointLights->size(); Index++ )
		{
			PointLight* PointLight = &PointLights->at( Index );

			if ( PointLight->IsVisible )
			{
				glActiveTexture( GL_TEXTURE3 );
				glBindTexture( GL_TEXTURE_2D, PointLight->ShadowMap );

				glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
				Shader::bind( TestRender );

				glEnable( GL_DEPTH_TEST );
				glClear( GL_STENCIL_BUFFER_BIT );
				glStencilFunc( GL_ALWAYS, 0, 0 );

				TestRender->setUniform( "PVTMatrix", PVMatrix * PointLight->LightSphere.GetTransformation() );
				PointLightRender->setUniform( "PVTMatrix", PVMatrix * PointLight->LightSphere.GetTransformation() );
				PointLightRender->setUniform( "light.Position", PointLight->Position );
				PointLightRender->setUniform( "light.Color", PointLight->Color );
				PointLightRender->setUniform( "light.Specular", PointLight->Specular );
				PointLightRender->setUniform( "light.Radius", PointLight->Radius );
				PointLightRender->setUniform( "light.Intensivity", PointLight->Intensivity );
				PointLight->LightSphere.RenderSphere();

				glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
				Shader::bind( PointLightRender );

				glStencilFunc( GL_NOTEQUAL, 0, 0xFF );
				glDisable( GL_DEPTH_TEST );

				glEnable( GL_BLEND );
				PointLight->LightSphere.RenderSphere();
				glDisable( GL_BLEND );
			}
		}
	}

	// ***************************************** //
	// Просчитываем прожекторные источники света

	if ( SpotLightRender != NULL && TestRender != NULL )
	{
		SpotLightRender->setUniform( "ViewPosition", ActiveCamera->GetPosition() );

		for ( size_t Index = 0; Index < SpotLights->size(); Index++ )
		{
			SpotLight* SpotLight = &SpotLights->at( Index );

			if ( SpotLight->IsVisible )
			{
				glActiveTexture( GL_TEXTURE3 );
				glBindTexture( GL_TEXTURE_2D, SpotLight->ShadowMap );

				glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
				Shader::bind( TestRender );

				glEnable( GL_DEPTH_TEST );
				glClear( GL_STENCIL_BUFFER_BIT );
				glStencilFunc( GL_ALWAYS, 0, 0 );

				TestRender->setUniform( "PVTMatrix", PVMatrix * SpotLight->LightCone.GetTransformation() );
				SpotLightRender->setUniform( "PVTMatrix", PVMatrix * SpotLight->LightCone.GetTransformation() );
				SpotLightRender->setUniform( "light.LightMatrix", SpotLight->LightTransforms[ 0 ] );
				SpotLightRender->setUniform( "light.Position", SpotLight->Position );
				SpotLightRender->setUniform( "light.Intensivity", SpotLight->Intensivity );
				SpotLightRender->setUniform( "light.Color", SpotLight->Color );
				SpotLightRender->setUniform( "light.Specular", SpotLight->Specular );
				SpotLightRender->setUniform( "light.SpotDirection", SpotLight->SpotDirection );
				SpotLightRender->setUniform( "light.Height", SpotLight->Height );
				SpotLightRender->setUniform( "light.SpotCutoff", SpotLight->SpotCutoff );
				SpotLight->LightCone.RenderCone();

				glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
				Shader::bind( SpotLightRender );

				glStencilFunc( GL_NOTEQUAL, 0, 0xFF );
				glDisable( GL_DEPTH_TEST );

				glEnable( GL_BLEND );
				SpotLight->LightCone.RenderCone();
				glDisable( GL_BLEND );
			}
		}
	}

	glDisable( GL_STENCIL_TEST );

	// ***************************************** //
	// Просчитываем направленные источники света

	glDisable( GL_DEPTH_TEST );
	glEnable( GL_BLEND );

	if ( DirectionalLights != NULL && DirectionalLightRender != NULL )
	{
		Shader::bind( DirectionalLightRender );
		DirectionalLightRender->setUniform( "ViewPosition", ActiveCamera->GetPosition() );

		for ( size_t Index = 0; Index < DirectionalLights->size(); Index++ )
		{
			DirectionalLight* DirectionalLight = &DirectionalLights->at( Index );

			glActiveTexture( GL_TEXTURE3 );
			glBindTexture( GL_TEXTURE_2D, DirectionalLight->ShadowMap );

			DirectionalLightRender->setUniform( "light.LightMatrix", DirectionalLight->LightTransforms[ 0 ] );
			DirectionalLightRender->setUniform( "light.Position", DirectionalLight->Position );
			DirectionalLightRender->setUniform( "light.Color", DirectionalLight->Color );
			DirectionalLightRender->setUniform( "light.Specular", DirectionalLight->Specular );
			DirectionalLightRender->setUniform( "light.Intensivity", DirectionalLight->Intensivity );
			DirectionalLight->Quad.RenderQuad();
		}
	}

	glDisable( GL_BLEND );
	glEnable( GL_CULL_FACE );
	glDepthMask( GL_TRUE );
}

//-------------------------------------------------------------------------//

le::Scene::InfoMesh::InfoMesh() :
	Skeleton( NULL ),
	MatrixTransformation( NULL ),
	Position( NULL ),
	IsRender( NULL )
{}

//-------------------------------------------------------------------------//