#include <System\VAO.h>
#include <Graphics\Model.h>
#include <Graphics\Camera.h>
#include <Graphics\Level.h>
#include "..\Scene.h"

//-------------------------------------------------------------------------//

le::Scene::Scene( System& System ) :
	ViewMatrix( NULL ),
	Frustum( NULL )
{
	ProjectionMatrix = &System.GetConfiguration().ProjectionMatrix;

	if ( !AnimationModelsRender.loadFromFile( "../shaders/AnimationModelsRender.vs", "../shaders/AnimationModelsRender.fs" ) )
		Logger::Log( Logger::Error, AnimationModelsRender.getErrorMessage().str() );

	if ( !StaticModelsRender.loadFromFile( "../shaders/StaticModelsRender.vs", "../shaders/StaticModelsRender.fs" ) )
		Logger::Log( Logger::Error, StaticModelsRender.getErrorMessage().str() );

	if ( !LevelRender.loadFromFile( "../shaders/LevelRender.vs", "../shaders/LevelRender.fs" ) )
		Logger::Log( Logger::Error, LevelRender.getErrorMessage().str() );
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
			RenderBuffer_StaticModels[ it->first ].push_back( &it->second );
	}
	else
	{
		for ( auto it = RenderMesh->begin(); it != RenderMesh->end(); it++ )
			RenderBuffer_AnimationModels[ it->first ].push_back( &it->second );
	}

	Model->SetScene( this );
}

//-------------------------------------------------------------------------//

void le::Scene::RemoveModelFromScene( Model* Model )
{
	map<GLuint, InfoMesh>* RenderMesh = &Model->GetRenderMesh();
	map<GLuint, vector<InfoMesh*>>* RenderBuffer_Models;

	if ( Model->IsNoSkeleton() )
		RenderBuffer_Models = &RenderBuffer_StaticModels;
	else
		RenderBuffer_Models = &RenderBuffer_AnimationModels;

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

void le::Scene::RemoveCamera()
{
	ViewMatrix = NULL;
	Frustum = NULL;
}

//-------------------------------------------------------------------------//

void le::Scene::RenderScene()
{
	glDepthMask( GL_TRUE );

	glEnable( GL_TEXTURE_2D );
	glEnable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );

	vector<InfoMesh*>* RenderBuffer;
	InfoMesh* InfoMesh;

	glm::mat4 PVMatrix;

	if ( ViewMatrix )
		PVMatrix = *ProjectionMatrix * ( *ViewMatrix );
	else
		PVMatrix = *ProjectionMatrix;

	// ****************************
	// Рендерим модели на сцене
	// ****************************

	// Рендерим анимируемые модели (со скелетом)
	if ( !RenderBuffer_AnimationModels.empty() ) // Если они есть (чтобы меньше переключаться между шейдерами)
	{
		Shader::bind( &AnimationModelsRender );

		for ( auto it = RenderBuffer_AnimationModels.begin(); it != RenderBuffer_AnimationModels.end(); it++ )
		{
			RenderBuffer = &it->second;
			glBindTexture( GL_TEXTURE_2D, it->first );

			for ( size_t i = 0; i < RenderBuffer->size(); i++ )
			{
				InfoMesh = ( *RenderBuffer )[ i ];

				if ( !Frustum->IsVisible( *InfoMesh->BoundingBox ) )
					continue;

				AnimationModelsRender.setUniform( "PVTMatrix", PVMatrix * ( *InfoMesh->MatrixTransformation ) );
				AnimationModelsRender.setUniform( "TransformMatrix", *InfoMesh->MatrixTransformation );

				vector<le::Skeleton::Bone>* Bones = InfoMesh->Skeleton->GetAllBones();

				for ( size_t i = 0; i < Bones->size(); i++ )
					AnimationModelsRender.setUniform( "Bones[" + to_string( i ) + "]", ( *Bones )[ i ].TransformMatrix );

				VAO::BindVAO( InfoMesh->VertexArray );
				glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
			}
		}
	}

	// Рендерим статические модели (без скелета)
	if ( !RenderBuffer_StaticModels.empty() )
	{
		Shader::bind( &StaticModelsRender );

		for ( auto it = RenderBuffer_StaticModels.begin(); it != RenderBuffer_StaticModels.end(); it++ )
		{
			RenderBuffer = &it->second;
			glBindTexture( GL_TEXTURE_2D, it->first );

			for ( size_t i = 0; i < RenderBuffer->size(); i++ )
			{
				InfoMesh = ( *RenderBuffer )[ i ];

				if ( !Frustum->IsVisible( *InfoMesh->BoundingBox ) )
					continue;

				StaticModelsRender.setUniform( "PVTMatrix", PVMatrix * ( *InfoMesh->MatrixTransformation ) );
				StaticModelsRender.setUniform( "TransformMatrix", *InfoMesh->MatrixTransformation );

				VAO::BindVAO( InfoMesh->VertexArray );
				glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
			}
		}
	}

	// ****************************
	// Рендерим уровень
	// ****************************

	if ( !RenderBuffer_Level.empty() )
	{
		Shader::bind( &LevelRender );
		LevelRender.setUniform( "PVMatrix", PVMatrix );

		for ( auto it = RenderBuffer_Level.begin(); it != RenderBuffer_Level.end(); it++ )
		{
			RenderBuffer = &it->second;
			glBindTexture( GL_TEXTURE_2D, it->first );

			for ( size_t i = 0; i < RenderBuffer->size(); i++ )
			{
				InfoMesh = ( *RenderBuffer )[ i ];

				if ( !Frustum->IsVisible( *InfoMesh->BoundingBox ) )
					continue;

				VAO::BindVAO( InfoMesh->VertexArray );
				glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
			}
		}
	}

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

void le::Scene::SetCamera( Camera& Camera )
{
	ViewMatrix = &Camera.GetViewMatrix();
	Frustum = &Camera.GetFrustum();
}

//-------------------------------------------------------------------------//

le::Scene::InfoMesh::InfoMesh() :
	Skeleton( NULL ),
	MatrixTransformation( NULL )
{}

//-------------------------------------------------------------------------//
