#include <System\VAO.h>
#include <Graphics\Model.h>
#include <Graphics\Camera.h>
#include <Graphics\Level.h>
#include <Graphics\LightManager.h>
#include "..\Scene.h"

//-------------------------------------------------------------------------//

inline bool sortFUNCTION( le::Scene::InfoMesh* InfoMesh_1, le::Scene::InfoMesh* InfoMesh_2 )
{
	return InfoMesh_1->DistanceToCamera < InfoMesh_2->DistanceToCamera;
}

//-------------------------------------------------------------------------//

le::Scene::Scene( System& System ) :
	ViewMatrix( NULL ),
	Frustum( NULL ),
	Camera( NULL ),
	LightManager( NULL )
{
	ProjectionMatrix = &System.GetConfiguration().ProjectionMatrix;
	PVMatrix = *ProjectionMatrix;

	if ( !AnimationModelsRender.loadFromFile( "../shaders/AnimationModelsRender.vs", "../shaders/AnimationModelsRender.fs" ) )
		Logger::Log( Logger::Error, AnimationModelsRender.getErrorMessage().str() );

	if ( !StaticModelsRender.loadFromFile( "../shaders/StaticModelsRender.vs", "../shaders/StaticModelsRender.fs" ) )
		Logger::Log( Logger::Error, StaticModelsRender.getErrorMessage().str() );

	if ( !LevelRender.loadFromFile( "../shaders/LevelRender.vs", "../shaders/LevelRender.fs" ) )
		Logger::Log( Logger::Error, LevelRender.getErrorMessage().str() );

	if ( !QueryTestRender.loadFromFile( "../shaders/QueryTestRender.vs", "../shaders/QueryTestRender.fs" ) )
		Logger::Log( Logger::Error, QueryTestRender.getErrorMessage().str() );

	if ( !PointLightRender.loadFromFile( "../shaders/PointLight.vs", "../shaders/PointLight.fs" ) )
		Logger::Log( Logger::Error, PointLightRender.getErrorMessage().str() );

	if ( !StencilTestRender.loadFromFile( "../shaders/StencilTestRender.vs", "../shaders/StencilTestRender.fs" ) )
		Logger::Log( Logger::Error, StencilTestRender.getErrorMessage().str() );

	if ( !DirectionalLightRender.loadFromFile( "../shaders/DirectionalLightRender.vs", "../shaders/DirectionalLightRender.fs" ) )
		Logger::Log( Logger::Error, DirectionalLightRender.getErrorMessage().str() );

	PointLightRender.setUniform( "ScreenSize", System.GetConfiguration().WindowSize );
	PointLightRender.setUniform( "ColorMap", GBuffer::Textures );
	PointLightRender.setUniform( "PositionMap", GBuffer::Position );
	PointLightRender.setUniform( "NormalMap", GBuffer::Normal );

	DirectionalLightRender.setUniform( "ScreenSize", System.GetConfiguration().WindowSize );
	DirectionalLightRender.setUniform( "ColorMap", GBuffer::Textures );
	DirectionalLightRender.setUniform( "NormalMap", GBuffer::Normal );

	GBuffer.InitGBuffer( System.GetConfiguration().WindowSize );
}

//-------------------------------------------------------------------------//

le::Scene::~Scene()
{}

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
	LevelInScene = NULL;
}

//-------------------------------------------------------------------------//

void le::Scene::AddLightManagerToScene( le::LightManager* LightManager )
{
	vector<PointLight>* PointLights = LightManager->GetAllPointLights();
	vector<DirectionalLight>* DirectionalLights = LightManager->GetAllDirectionalLights();

	for ( auto it = PointLights->begin(); it != PointLights->end(); it++ )
	{
		this->PointLights.push_back( &( *it ) );
		it->SetScene( this );
	}

	for ( auto it = DirectionalLights->begin(); it != DirectionalLights->end(); it++ )
	{
		this->DirectionalLights.push_back( &( *it ) );
		it->SetScene( this );
	}

	LightManager->SetScene( this );
	this->LightManager = LightManager;
}

//-------------------------------------------------------------------------//

void le::Scene::AddPointLightToScene( le::PointLight* PointLight )
{
	PointLights.push_back( PointLight );
	PointLight->SetScene( this );
}

//-------------------------------------------------------------------------//

void le::Scene::AddDirectionalLightToScene( le::DirectionalLight* DirectionalLight )
{
	DirectionalLights.push_back( DirectionalLight );
	DirectionalLight->SetScene( this );
}

//-------------------------------------------------------------------------//

void le::Scene::RemovePointLightFromScene( PointLight* PointLight )
{
	for ( auto it = PointLights.begin(); it != PointLights.end(); it++ )
		if ( *it == PointLight )
		{
			( *it )->SetScene( NULL );
			PointLights.erase( it );
			break;
		}
}

//-------------------------------------------------------------------------//

void le::Scene::RemovePointLightFromScene( const string& NameLight )
{
	for ( auto it = PointLights.begin(); it != PointLights.end(); it++ )
		if ( ( *it )->NameLight == NameLight )
		{
			( *it )->SetScene( NULL );
			PointLights.erase( it );
			break;
		}
}

//-------------------------------------------------------------------------//

void le::Scene::RemoveDirectionalLightToScene( le::DirectionalLight* DirectionalLight )
{
	for ( auto it = DirectionalLights.begin(); it != DirectionalLights.end(); it++ )
		if ( *it == DirectionalLight )
		{
			( *it )->SetScene( NULL );
			DirectionalLights.erase( it );
			break;
		}
}

//-------------------------------------------------------------------------//

void le::Scene::RemoveDirectionalLightToScene( const string& NameLight )
{
	for ( auto it = DirectionalLights.begin(); it != DirectionalLights.end(); it++ )
		if ( ( *it )->NameLight == NameLight )
		{
			( *it )->SetScene( NULL );
			DirectionalLights.erase( it );
			break;
		}
}

//-------------------------------------------------------------------------//

void le::Scene::RemoveLightManagerFromScene( le::LightManager* LightManager )
{
	vector<PointLight>* Lights = LightManager->GetAllPointLights();

	for ( auto it = Lights->begin(); it != Lights->end(); it++ )
		for ( auto it_Lights = PointLights.begin(); it_Lights != PointLights.end(); it_Lights++ )
			if ( &( *it ) == *it_Lights )
			{
				( *it_Lights )->SetScene( NULL );
				PointLights.erase( it_Lights );
				break;
			}

	LightManager->SetScene( NULL );
	this->LightManager = NULL;
}

//-------------------------------------------------------------------------//

void le::Scene::RemoveCamera()
{
	ViewMatrix = NULL;
	Frustum = NULL;
	Camera = NULL;
}

//-------------------------------------------------------------------------//

void le::Scene::RenderScene()
{
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );

	vector<InfoMesh*>* GeometryBuffer;
	vector<le::Skeleton::Bone>* Bones;
	InfoMesh* InfoMesh;

	if ( Camera )
		PVMatrix = *ProjectionMatrix * ( *ViewMatrix );

	LevelRender.setUniform( "PVMatrix", PVMatrix );
	QueryTestRender.setUniform( "PVTMatrix", PVMatrix );
	AnimationModelsRender.setUniform( "PVMatrix", PVMatrix );
	StaticModelsRender.setUniform( "PVMatrix", PVMatrix );

	// ****************************
	// Проверка видимости геометрии
	// ****************************

	size_t PointLightVisible = 0;

	if ( Frustum != NULL && Camera != NULL )
	{
		if ( Configuration::IsWireframeRender ) // отключаем каркасный рендер если он включен
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

		Shader::bind( &QueryTestRender );
		glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );

		size_t ModelsVisible = 0;
		size_t BrushesVisible = 0;

		// ***************************************** //
		// Проверка брашей на отсечение по фрустуму

		for ( auto it = RenderBuffer_Level.begin(); it != RenderBuffer_Level.end(); it++ )
		{
			GeometryBuffer = &it->second;

			for ( size_t i = 0; i < GeometryBuffer->size(); i++ )
			{
				InfoMesh = ( *GeometryBuffer )[ i ];

				if ( !Frustum->IsVisible( *InfoMesh->BoundingBox ) )
					InfoMesh->IsRender = false;
				else
				{
					InfoMesh->IsRender = true;
					InfoMesh->DistanceToCamera = Camera->GetDistance( *InfoMesh->Position );

					if ( BrushesVisible >= GeometryBuffer_Level.size() )
						GeometryBuffer_Level.push_back( InfoMesh );
					else
						GeometryBuffer_Level[ BrushesVisible ] = InfoMesh;

					BrushesVisible++;
				}
			}
		}

		// ***************************************** //
		// Проверка статичных моделей на отсечение по фрустуму

		for ( auto it = RenderBuffer_StaticModel.begin(); it != RenderBuffer_StaticModel.end(); it++ )
		{
			GeometryBuffer = &it->second;

			for ( size_t i = 0; i < GeometryBuffer->size(); i++ )
			{
				InfoMesh = ( *GeometryBuffer )[ i ];

				if ( !Frustum->IsVisible( *InfoMesh->BoundingBox ) )
					InfoMesh->IsRender = false;
				else
				{
					InfoMesh->IsRender = true;
					InfoMesh->DistanceToCamera = Camera->GetDistance( *InfoMesh->Position );

					if ( ModelsVisible >= GeometryBuffer_Models.size() )
						GeometryBuffer_Models.push_back( InfoMesh );
					else
						GeometryBuffer_Models[ ModelsVisible ] = InfoMesh;

					ModelsVisible++;
				}
			}
		}

		// ***************************************** //
		// Проверка анимируемых моделей на отсечение по фрустуму

		for ( auto it = RenderBuffer_AnimationModel.begin(); it != RenderBuffer_AnimationModel.end(); it++ )
		{
			GeometryBuffer = &it->second;

			for ( size_t i = 0; i < GeometryBuffer->size(); i++ )
			{
				InfoMesh = ( *GeometryBuffer )[ i ];

				if ( !Frustum->IsVisible( *InfoMesh->BoundingBox ) )
					InfoMesh->IsRender = false;
				else
				{
					InfoMesh->IsRender = true;
					InfoMesh->DistanceToCamera = Camera->GetDistance( *InfoMesh->Position );

					if ( ModelsVisible >= GeometryBuffer_Models.size() )
						GeometryBuffer_Models.push_back( InfoMesh );
					else
						GeometryBuffer_Models[ ModelsVisible ] = InfoMesh;

					ModelsVisible++;
				}
			}
		}

		// ***************************************** //
		// Проверка точечного освещения на отсечение по фрустуму

		for ( auto it = PointLights.begin(); it != PointLights.end(); it++ )
			if ( Frustum->IsVisible( ( *it )->LightSphere ) )
			{
				if ( PointLightVisible >= LightBuffer_PointLight.size() )
					LightBuffer_PointLight.push_back( ( *it ) );
				else
					LightBuffer_PointLight[ PointLightVisible ] = ( *it );

				PointLightVisible++;
			}


		// ***************************************** //
		// Сортируем геометрию уровня по удалению от камеры

		sort( GeometryBuffer_Level.begin(), GeometryBuffer_Level.begin() + BrushesVisible, sortFUNCTION );

		// ***************************************** //
		// Рендер ограничивающих тел брашей

		for ( size_t i = 0; i < BrushesVisible; i++ )
			GeometryBuffer_Level[ i ]->BoundingBox->QueryTest();

		// ***************************************** //
		// Рендер ограничивающих тел моделей

		glDepthMask( GL_FALSE );

		for ( size_t i = 0; i < ModelsVisible; i++ )
			GeometryBuffer_Models[ i ]->BoundingBox->QueryTest();

		glDepthMask( GL_TRUE );
		glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

		if ( Configuration::IsWireframeRender ) // включаем каркасный рендер обратно если он был включен
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	}

	// ****************************
	// Рендер геометрии сцены
	// ****************************

	GBuffer.ClearFrame();
	GBuffer.Bind( GBuffer::RenderBuffers );

	// ***************************************** //
	// Рендер брашей уровня

	if ( !RenderBuffer_Level.empty() )
	{
		Shader::bind( &LevelRender );

		for ( auto it = RenderBuffer_Level.begin(); it != RenderBuffer_Level.end(); it++ )
		{
			glBindTexture( GL_TEXTURE_2D, it->first );
			GeometryBuffer = &it->second;

			for ( size_t i = 0; i < GeometryBuffer->size(); i++ )
			{
				InfoMesh = ( *GeometryBuffer )[ i ];

				if ( !InfoMesh->IsRender )
					continue;

				InfoMesh->BoundingBox->Query.StartConditionalRender( GL_QUERY_WAIT );

				VAO::BindVAO( InfoMesh->VertexArray );
				glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );

				InfoMesh->BoundingBox->Query.EndConditionalRender();
			}
		}
	}

	// ***************************************** //
	// Рендер анимируемых моделей

	if ( !RenderBuffer_AnimationModel.empty() )
	{
		Shader::bind( &AnimationModelsRender );

		for ( auto it = RenderBuffer_AnimationModel.begin(); it != RenderBuffer_AnimationModel.end(); it++ )
		{
			glBindTexture( GL_TEXTURE_2D, it->first );
			GeometryBuffer = &it->second;

			for ( size_t i = 0; i < GeometryBuffer->size(); i++ )
			{
				InfoMesh = ( *GeometryBuffer )[ i ];

				if ( !InfoMesh->IsRender )
					continue;

				Bones = InfoMesh->Skeleton->GetAllBones();
				AnimationModelsRender.setUniform( "TransformMatrix", *InfoMesh->MatrixTransformation );

				for ( size_t j = 0; j < Bones->size(); j++ )
					AnimationModelsRender.setUniform( "Bones[" + to_string( j ) + "]", ( *Bones )[ j ].TransformMatrix );

				InfoMesh->BoundingBox->Query.StartConditionalRender( GL_QUERY_WAIT );

				VAO::BindVAO( InfoMesh->VertexArray );
				glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );

				InfoMesh->BoundingBox->Query.EndConditionalRender();
			}
		}
	}

	// ***************************************** //
	// Рендер статичных моделей (те которые не анимируются)

	if ( !RenderBuffer_StaticModel.empty() )
	{
		Shader::bind( &StaticModelsRender );

		for ( auto it = RenderBuffer_StaticModel.begin(); it != RenderBuffer_StaticModel.end(); it++ )
		{
			glBindTexture( GL_TEXTURE_2D, it->first );
			GeometryBuffer = &it->second;

			for ( size_t i = 0; i < GeometryBuffer->size(); i++ )
			{
				InfoMesh = ( *GeometryBuffer )[ i ];

				if ( !InfoMesh->IsRender )
					continue;

				StaticModelsRender.setUniform( "TransformMatrix", *InfoMesh->MatrixTransformation );

				InfoMesh->BoundingBox->Query.StartConditionalRender( GL_QUERY_WAIT );

				VAO::BindVAO( InfoMesh->VertexArray );
				glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );

				InfoMesh->BoundingBox->Query.EndConditionalRender();
			}
		}
	}

	// ****************************
	// Просчитывание освещения
	// ****************************

	if ( !PointLights.empty() || !DirectionalLights.empty() )
	{
		glDepthMask( GL_FALSE );
		glEnable( GL_STENCIL_TEST );

		glBlendEquation( GL_FUNC_ADD );
		glBlendFunc( GL_ONE, GL_ONE );
		
		glStencilOpSeparate( GL_FRONT, GL_KEEP, GL_INCR_WRAP, GL_KEEP );
		glStencilOpSeparate( GL_BACK, GL_KEEP, GL_DECR_WRAP, GL_KEEP );

		GBuffer.Bind( GBuffer::RenderLight );

		// ***************************************** //
		// Просчитываем точечные источники света

		for ( size_t i = 0; i < PointLightVisible; i++ )
		{
			PointLight* PointLight = LightBuffer_PointLight[ i ];
			PVTMatrix = PVMatrix * PointLight->LightSphere.GetTransformation();

			// ***************************************** //
			// Рендер в буффер трафарета для отсечения лишних объемов

			glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );

			Shader::bind( &StencilTestRender );

			glEnable( GL_DEPTH_TEST );
			glClear( GL_STENCIL_BUFFER_BIT );
			glDisable( GL_CULL_FACE );

			glStencilFunc( GL_ALWAYS, 0, 0 );

			StencilTestRender.setUniform( "PVTMatrix", PVTMatrix );
			PointLight->LightSphere.RenderSphere();

			glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );

			// ***************************************** //
			// Рендер источника света

			Shader::bind( &PointLightRender );

			glStencilFunc( GL_NOTEQUAL, 0, 0xFF );
			glDisable( GL_DEPTH_TEST );

			glEnable( GL_BLEND );
			glEnable( GL_CULL_FACE );
			glCullFace( GL_FRONT );

			PointLightRender.setUniform( "PVTMatrix", PVTMatrix );
			PointLightRender.setUniform( "light.Position", PointLight->Position );
			PointLightRender.setUniform( "light.Color", PointLight->Color );
			PointLightRender.setUniform( "light.Radius", PointLight->Radius );
			PointLight->LightSphere.RenderSphere();

			glCullFace( GL_BACK );
			glDisable( GL_BLEND );
		}

		glDisable( GL_STENCIL_TEST );

		// ***************************************** //
		// Просчитываем направленные источники света

		glDisable( GL_DEPTH_TEST );
		glEnable( GL_BLEND );

		if ( !DirectionalLights.empty() )
		{
			Shader::bind( &DirectionalLightRender );

			for ( size_t i = 0; i < DirectionalLights.size(); i++ )
			{
				DirectionalLight* DirectionalLight = DirectionalLights[ i ];

				DirectionalLightRender.setUniform( "light.Position", DirectionalLight->Position );
				DirectionalLightRender.setUniform( "light.Color", DirectionalLight->Color );
				DirectionalLights[ i ]->LightQuad.RenderQuad();
			}
		}

		glDisable( GL_BLEND );
		glDepthMask( GL_TRUE );

		GBuffer.RenderFrame();
	}
	else
		GBuffer.RenderFrame( GBuffer::Textures );

	Shader::bind( NULL );
	VAO::UnbindVAO();

	glDisable( GL_TEXTURE_2D );
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
}

//-------------------------------------------------------------------------//

void le::Scene::ClearScene()
{}

//-------------------------------------------------------------------------//

void le::Scene::SetCamera( le::Camera& Camera )
{
	ViewMatrix = &Camera.GetViewMatrix();
	Frustum = &Camera.GetFrustum();
	this->Camera = &Camera;
}

//-------------------------------------------------------------------------//

le::GBuffer& le::Scene::GetGBuffer()
{
	return GBuffer;
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
