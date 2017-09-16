#include <System\VAO.h>
#include <Graphics\Model.h>
#include <Graphics\Camera.h>
#include <Graphics\Level.h>
#include "..\Scene.h"

//-------------------------------------------------------------------------//

le::Scene::Scene( System& System )
{
	ProjectionMatrix = &System.GetConfiguration().ProjectionMatrix;
	ViewMatrix = NULL;

	if ( !ModelsRender.loadFromFile( "../shaders/ModelsRender.vs", "../shaders/ModelsRender.fs" ) )
		Logger::Log( Logger::Error, ModelsRender.getErrorMessage().str() );

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

	for ( auto it = RenderMesh->begin(); it != RenderMesh->end(); it++ )
		RenderBuffer_Models[ it->first ].push_back( &it->second );

	Model->SetScene( this );
}

//-------------------------------------------------------------------------//

void le::Scene::RemoveModelFromScene( Model* Model )
{
	map<GLuint, InfoMesh>* RenderMesh = &Model->GetRenderMesh();
	vector<InfoMesh*>* InfoMeshes;

	for ( auto it = RenderMesh->begin(); it != RenderMesh->end(); it++ )
	{
		InfoMeshes = &RenderBuffer_Models[ it->first ];

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

	Shader::bind( &ModelsRender );

	for ( auto it = RenderBuffer_Models.begin(); it != RenderBuffer_Models.end(); it++ )
	{
		RenderBuffer = &it->second;
		glBindTexture( GL_TEXTURE_2D, it->first );

		for ( size_t i = 0; i < RenderBuffer->size(); i++ )
		{
			InfoMesh = ( *RenderBuffer )[ i ];

			ModelsRender.setUniform( "PVTMatrix", PVMatrix * ( *InfoMesh->MatrixTransformation ) );
			ModelsRender.setUniform( "TransformMatrix", *InfoMesh->MatrixTransformation );

			vector<le::Skeleton::Bone>* Bones = InfoMesh->Skeleton->GetAllBones();

			for ( size_t i = 0; i < Bones->size(); i++ )
				ModelsRender.setUniform( "Bones[" + to_string( i ) + "]", ( *Bones )[ i ].TransformMatrix );

			VAO::BindVAO( InfoMesh->VertexArray );
			glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
		}
	}

	// ****************************
	// Рендерим уровень
	// ****************************

	Shader::bind( &LevelRender );
	LevelRender.setUniform( "PVMatrix", PVMatrix );

	for ( auto it = RenderBuffer_Level.begin(); it != RenderBuffer_Level.end(); it++ )
	{
		RenderBuffer = &it->second;
		glBindTexture( GL_TEXTURE_2D, it->first );

		for ( size_t i = 0; i < RenderBuffer->size(); i++ )
		{
			InfoMesh = ( *RenderBuffer )[ i ];		

			VAO::BindVAO( InfoMesh->VertexArray );
			glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
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
}

//-------------------------------------------------------------------------//

le::Scene::InfoMesh::InfoMesh() :
	Skeleton( NULL ),
	MatrixTransformation( NULL )
{}

//-------------------------------------------------------------------------//
