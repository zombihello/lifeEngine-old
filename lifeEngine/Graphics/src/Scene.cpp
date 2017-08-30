#include <System\VAO.h>
#include <Graphics\Model.h>
#include "..\Scene.h"

//-------------------------------------------------------------------------//

le::Scene::Scene( System& System )
{
	ProjectionMatrix = &System.GetConfiguration().ProjectionMatrix;

	ViewMatrix = glm::lookAt(
		glm::vec3( 0, 0, -100 ),
		glm::vec3( 0, 0, 0 ),
		glm::vec3( 0, 1, 0 ) );

	if ( !GeometryRender.loadFromFile( "geometryRender.vs", "geometryRender.fs" ) )
		Logger::Log( Logger::Error, GeometryRender.getErrorMessage().str() );
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
		RenderBuffer[ it->first ].push_back( &it->second );

	Model->SetScene( this );
}

//-------------------------------------------------------------------------//

void le::Scene::RemoveModelFromScene( Model* Model )
{
	map<GLuint, InfoMesh>* RenderMesh = &Model->GetRenderMesh();
	vector<InfoMesh*>* InfoMeshes;

	for ( auto it = RenderMesh->begin(); it != RenderMesh->end(); it++ )
	{
		InfoMeshes = &RenderBuffer[ it->first ];

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

void le::Scene::RenderScene()
{
	glEnable( GL_TEXTURE_2D );

	vector<InfoMesh*>* RenderBuffer_Meshes;
	InfoMesh* InfoMesh;

	Shader::bind( &GeometryRender );
	glm::mat4 PVMatrix = *ProjectionMatrix * ViewMatrix;

	glDepthMask( GL_TRUE );
	//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );

	for ( auto it = RenderBuffer.begin(); it != RenderBuffer.end(); it++ )
	{
		RenderBuffer_Meshes = &it->second;
		glBindTexture( GL_TEXTURE_2D, it->first );

		for ( size_t i = 0; i < RenderBuffer_Meshes->size(); i++ )
		{
			InfoMesh = ( *RenderBuffer_Meshes )[ i ];

			GeometryRender.setUniform( "PVTMatrix", PVMatrix * ( *InfoMesh->MatrixTransformation ) );
			GeometryRender.setUniform( "TransformMatrix", *InfoMesh->MatrixTransformation );

			VAO::BindVAO( InfoMesh->VertexArray );
			glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
		}
	}

	VAO::UnbindVAO();
	Shader::bind( NULL );
	//glDepthMask( GL_FALSE );
	
	glDisable( GL_TEXTURE_2D );
}

//-------------------------------------------------------------------------//

void le::Scene::ClearScene()
{}

//-------------------------------------------------------------------------//
