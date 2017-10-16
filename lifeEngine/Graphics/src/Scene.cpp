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

	PointLightRender.setUniform( "ScreenSize", System.GetConfiguration().WindowSize );
	PointLightRender.setUniform( "ColorMap", GBuffer::Textures );
	PointLightRender.setUniform( "PositionMap", GBuffer::Position );
	PointLightRender.setUniform( "NormalMap", GBuffer::Normal );

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
			RenderBuffer[ it->first ].StaticModels.push_back( &it->second );
	}
	else
	{
		for ( auto it = RenderMesh->begin(); it != RenderMesh->end(); it++ )
			RenderBuffer[ it->first ].AnimationModels.push_back( &it->second );
	}

	Model->SetScene( this );
}

//-------------------------------------------------------------------------//

void le::Scene::RemoveModelFromScene( Model* Model )
{
	map<GLuint, InfoMesh>* RenderMesh = &Model->GetRenderMesh();
	vector<InfoMesh*>* InfoMeshes;

	if ( Model->IsNoSkeleton() ) // если модель без скелета
		for ( auto it = RenderMesh->begin(); it != RenderMesh->end(); it++ )
		{
			InfoMeshes = &RenderBuffer[ it->first ].StaticModels;

			for ( size_t i = 0; i < InfoMeshes->size(); i++ )
				if ( ( *InfoMeshes )[ i ] == &it->second )
				{
					InfoMeshes->erase( InfoMeshes->begin() + i );
					break;
				}
		}
	else
		for ( auto it = RenderMesh->begin(); it != RenderMesh->end(); it++ )
		{
			InfoMeshes = &RenderBuffer[ it->first ].AnimationModels;

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
			RenderBuffer[ it->first ].Level.push_back( &it->second );
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
			InfoMeshes = &RenderBuffer[ it->first ].Level;

			for ( size_t i = 0; i < InfoMeshes->size(); i++ )
				if ( ( *InfoMeshes )[ i ] == &it->second )
				{
					InfoMeshes->erase( InfoMeshes->begin() + i );
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
	vector<PointLight>* Lights = LightManager->GetAllPointLights();

	for ( auto it = Lights->begin(); it != Lights->end(); it++ )
	{
		PointLights.push_back( &( *it ) );
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
	if ( RenderBuffer.empty() ) // если буффер рендера пуст, то выйти из метода
		return;

	glEnable( GL_TEXTURE_2D );
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );

	vector<InfoMesh*>* GeometryBuffer;
	vector<le::Skeleton::Bone>* Bones;
	InfoMesh* InfoMesh;

	if ( ViewMatrix )
		PVMatrix = *ProjectionMatrix * ( *ViewMatrix );
	else
		PVMatrix = *ProjectionMatrix;

	LevelRender.setUniform( "PVMatrix", PVMatrix );
	QueryTestRender.setUniform( "PVTMatrix", PVMatrix );

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

		for ( auto it = RenderBuffer.begin(); it != RenderBuffer.end(); it++ )
		{
			// ***************************************** //
			// Проверка брашей на отсечение по фрустуму

			GeometryBuffer = &RenderBuffer[ it->first ].Level;

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

			// ***************************************** //
			// Проверка статичных моделей на отсечение по фрустуму

			GeometryBuffer = &RenderBuffer[ it->first ].StaticModels;

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

			// ***************************************** //
			// Проверка анимируемых моделей на отсечение по фрустуму

			GeometryBuffer = &RenderBuffer[ it->first ].AnimationModels;

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

		// ***************************************** //
		// Рендер ограничивающих тел точечного освещения

		for ( size_t i = 0; i < PointLightVisible; i++ )
		{
			QueryTestRender.setUniform( "PVTMatrix", PVMatrix * LightBuffer_PointLight[ i ]->LightSphere.GetTransformation() );
			LightBuffer_PointLight[ i ]->LightSphere.QueryTest();
		}

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

	for ( auto it = RenderBuffer.begin(); it != RenderBuffer.end(); it++ )
	{
		glBindTexture( GL_TEXTURE_2D, it->first );

		// ***************************************** //
		// Рендер брашей уровня

		GeometryBuffer = &RenderBuffer[ it->first ].Level;

		if ( !GeometryBuffer->empty() )
		{
			Shader::bind( &LevelRender );

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

		// ***************************************** //
		// Рендер анимируемых моделей

		GeometryBuffer = &RenderBuffer[ it->first ].AnimationModels;

		if ( !GeometryBuffer->empty() )
		{
			Shader::bind( &AnimationModelsRender );

			for ( size_t i = 0; i < GeometryBuffer->size(); i++ )
			{
				InfoMesh = ( *GeometryBuffer )[ i ];

				if ( !InfoMesh->IsRender )
					continue;

				Bones = InfoMesh->Skeleton->GetAllBones();
				AnimationModelsRender.setUniform( "PVMatrix", PVMatrix );
				AnimationModelsRender.setUniform( "TransformMatrix", *InfoMesh->MatrixTransformation );

				for ( size_t j = 0; j < Bones->size(); j++ )
					AnimationModelsRender.setUniform( "Bones[" + to_string( j ) + "]", ( *Bones )[ j ].TransformMatrix );

				InfoMesh->BoundingBox->Query.StartConditionalRender( GL_QUERY_WAIT );

				VAO::BindVAO( InfoMesh->VertexArray );
				glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );

				InfoMesh->BoundingBox->Query.EndConditionalRender();
			}
		}

		// ***************************************** //
		// Рендер статичных моделей (те которые не анимируются)

		GeometryBuffer = &RenderBuffer[ it->first ].StaticModels;

		if ( !GeometryBuffer->empty() )
		{
			Shader::bind( &StaticModelsRender );

			for ( size_t i = 0; i < GeometryBuffer->size(); i++ )
			{
				InfoMesh = ( *GeometryBuffer )[ i ];

				if ( !InfoMesh->IsRender )
					continue;

				Bones = InfoMesh->Skeleton->GetAllBones();
				StaticModelsRender.setUniform( "PVMatrix", PVMatrix * ( *InfoMesh->MatrixTransformation ) );
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

	glDepthMask( GL_FALSE );
	glEnable( GL_STENCIL_TEST );
	
	// ***************************************** //
	// Просчитываем точечные источники света

	for ( size_t i = 0; i < PointLightVisible; i++ )
	{
		PointLight* PointLight = LightBuffer_PointLight[ i ];

		// ***************************************** //
		// Рендер в буффер трафарета для отсечения лишних объемов

		Shader::bind( &StencilTestRender );
		GBuffer.Bind( GBuffer::StencilTest );
		
		glEnable( GL_DEPTH_TEST );
		glClear( GL_STENCIL_BUFFER_BIT );
		glDisable( GL_CULL_FACE );

		glStencilFunc( GL_ALWAYS, 0, 0 );
		glStencilOpSeparate( GL_FRONT, GL_KEEP, GL_INCR_WRAP, GL_KEEP );
		glStencilOpSeparate( GL_BACK, GL_KEEP, GL_DECR_WRAP, GL_KEEP );

		StencilTestRender.setUniform( "PVTMatrix", PVMatrix * PointLight->LightSphere.GetTransformation() );
		PointLight->LightSphere.RenderSphere();	

		// ***************************************** //
		// Рендер источника света

		Shader::bind( &PointLightRender );
		GBuffer.Bind( GBuffer::RenderLight );
		
		glStencilFunc( GL_NOTEQUAL, 0, 0xFF );
		glDisable( GL_DEPTH_TEST );

		glEnable( GL_BLEND );
		glBlendEquation( GL_FUNC_ADD );
		glBlendFunc( GL_ONE, GL_ONE );

		glEnable( GL_CULL_FACE );
		glCullFace( GL_FRONT );

		PointLightRender.setUniform( "PVTMatrix", PVMatrix * PointLight->LightSphere.GetTransformation() );
		PointLightRender.setUniform( "light.Position", PointLight->Position );
		PointLightRender.setUniform( "light.Color", PointLight->Color );
		PointLightRender.setUniform( "light.Intensivity", PointLight->Intensivity );
		PointLightRender.setUniform( "light.Radius", PointLight->Radius );
		PointLight->LightSphere.RenderSphere();

		glCullFace( GL_BACK );
		glDisable( GL_BLEND );	
	}

	glDisable( GL_STENCIL_TEST );
	glDepthMask( GL_TRUE );

	GBuffer.RenderFrame();

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
