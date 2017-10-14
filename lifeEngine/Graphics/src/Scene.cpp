#include <System\VAO.h>
#include <Graphics\Model.h>
#include <Graphics\Camera.h>
#include <Graphics\Level.h>
#include <Graphics\PointLight.h>
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
	Camera( NULL )
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

	GBuffer.InitGBuffer( System.GetConfiguration().WindowSize );

	Light.SetPosition( glm::vec4( 10.f, 2.f, 3.f,1 ) );
	Light.SetRadius( 45 );
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
	QueryTestRender.setUniform( "PVMatrix", PVMatrix );

	// ****************************
	// Проверка видимости геометрии
	// ****************************

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
		// Сортируем геометри по удалению от камеры

		sort( GeometryBuffer_Level.begin(), GeometryBuffer_Level.begin() + BrushesVisible, sortFUNCTION );
		sort( GeometryBuffer_Models.begin(), GeometryBuffer_Models.begin() + ModelsVisible, sortFUNCTION );

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

	Light.LightSphere.RenderSphere(); //          } для теста
	GBuffer.RenderFrame( GBuffer::Textures ); //

	Shader::bind( NULL );
	VAO::UnbindVAO();

	// ****************************
	// Просчитывание освещения
	// ****************************

	///TODO: Написать код

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
