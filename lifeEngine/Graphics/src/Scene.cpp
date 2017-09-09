﻿#include <System\VAO.h>
#include <Graphics\Model.h>
#include <Graphics\Camera.h>
#include "..\Scene.h"

//-------------------------------------------------------------------------//

le::Scene::Scene( System& System )
{
	ProjectionMatrix = &System.GetConfiguration().ProjectionMatrix;
	ViewMatrix = NULL;

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

	vector<InfoMesh*>* RenderBuffer_Meshes;
	InfoMesh* InfoMesh;

	Shader::bind( &GeometryRender );
	glm::mat4 PVMatrix;

	if ( ViewMatrix )
		PVMatrix = *ProjectionMatrix * ( *ViewMatrix );
	else
		PVMatrix = *ProjectionMatrix;

	for ( auto it = RenderBuffer.begin(); it != RenderBuffer.end(); it++ )
	{
		RenderBuffer_Meshes = &it->second;
		glBindTexture( GL_TEXTURE_2D, it->first );

		for ( size_t i = 0; i < RenderBuffer_Meshes->size(); i++ )
		{
			InfoMesh = ( *RenderBuffer_Meshes )[ i ];

			GeometryRender.setUniform( "PVTMatrix", PVMatrix * ( *InfoMesh->MatrixTransformation ) );
			GeometryRender.setUniform( "TransformMatrix", *InfoMesh->MatrixTransformation );

			vector<le::Skeleton::Bone>* Bones = InfoMesh->Skeleton->GetAllBones();

			for ( size_t i = 0; i < Bones->size(); i++ )
				GeometryRender.setUniform( "Bones[" + to_string( i ) + "]", (*Bones )[ i ].TransformMatrix );

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
