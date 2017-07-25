#include "../../3D/Scene3D.h"

//-------------------------------------------------------------------------//

le::Scene3D::Scene3D( le::System& System )
{
	PlayerCamera = NULL;
	this->System = &System;
	ProjectionMatrix = &System.GetConfiguration().ProjectionMatrix;

	ViewMatrix = glm::lookAt(
		glm::vec3( 0, 0, 0 ),
		glm::vec3( 0, 0, 0 ),
		glm::vec3( 0, 1, 0 ) );

	string ShadersDir = System.GetConfiguration().sShadersDir;
	GeometryRender.loadFromFile( ShadersDir + "/vertexShader.vs", ShadersDir + "/fragmentShader.fs" );
}

//-------------------------------------------------------------------------//

le::Scene3D::Scene3D( le::System& System, le::Camera& PlayerCamera )
{
	this->PlayerCamera = &PlayerCamera;
	this->System = &System;

	ProjectionMatrix = &System.GetConfiguration().ProjectionMatrix;

	string ShadersDir = System.GetConfiguration().sShadersDir;
	GeometryRender.loadFromFile( ShadersDir + "/vertexShader.vs", ShadersDir + "/fragmentShader.fs" );
}

//-------------------------------------------------------------------------//

le::Scene3D::~Scene3D()
{}

//-------------------------------------------------------------------------//

void le::Scene3D::AddMeshToScene( GLuint Texture, SceneInfoMesh Mesh )
{
	mRenderBuffer[ Texture ].push_back( Mesh );
}

//-------------------------------------------------------------------------//

void le::Scene3D::AddMeshToScene( map<GLuint, SceneInfoMesh> Mesh )
{
	for ( auto it = Mesh.begin(); it != Mesh.end(); it++ )
		mRenderBuffer[ it->first ].push_back( it->second );
}

//-------------------------------------------------------------------------//

void le::Scene3D::AddLightToScene( Light3D Light )
{
	vPointLights.push_back( Light );
}

//-------------------------------------------------------------------------//

void le::Scene3D::SetPlayerCamera( Camera& PlayerCamera )
{
	this->PlayerCamera = &PlayerCamera;
}

//-------------------------------------------------------------------------//

void le::Scene3D::RenderScene()
{
	vector<SceneInfoMesh>* vRenderBuffer_Meshes;
	SceneInfoMesh* InfoMesh;

	if ( PlayerCamera != NULL )
		ViewMatrix = PlayerCamera->GetViewMatrix();

	GeometryRender.setUniform( "transform.projectionMatrix", *ProjectionMatrix );
	GeometryRender.setUniform( "transform.viewMatrix", ViewMatrix );
	GeometryRender.setUniform( "transform.viewPosition", PlayerCamera->GetPosition() );

	Shader::bind( &GeometryRender );

	for ( auto it = mRenderBuffer.begin(); it != mRenderBuffer.end(); it++ )
	{
		vRenderBuffer_Meshes = &it->second;
		glBindTexture( GL_TEXTURE_2D, it->first );

		for ( int i = 0; i < vRenderBuffer_Meshes->size(); i++ )
		{
			InfoMesh = &( *vRenderBuffer_Meshes )[ i ];

			GeometryRender.setUniform( "transform.transformationMatrix", *InfoMesh->MatrixTransformation );

			GeometryRender.setUniform( "material.ambient", InfoMesh->Material.Ambient );
			GeometryRender.setUniform( "material.diffuse", InfoMesh->Material.Diffuse );
			GeometryRender.setUniform( "material.specular", InfoMesh->Material.Specular );
			GeometryRender.setUniform( "material.emission", InfoMesh->Material.Emission );
			GeometryRender.setUniform( "material.shininess", InfoMesh->Material.fShininess );

			LoaderVAO::BindVAO( InfoMesh->VertexArray );
			LoaderVAO::EnableVertexPointer( VERT_POSITION );
			LoaderVAO::EnableVertexPointer( VERT_NORMAL );
			LoaderVAO::EnableVertexPointer( VERT_TEXCOORD );

			for ( int id = 0, j = 0; id < abs((float)vPointLights.size() - MAX_LIGHT); id += MAX_LIGHT ) // TODO: НЕ ОПТИМИЗИРОВАНО! ИСПРАВИТЬ!
			{
				int CountLights = 0;
				for ( ; CountLights < MAX_LIGHT && vPointLights.size() > j; CountLights++, j++ )
				{
					GeometryRender.setUniform( "light[" + to_string( j ) + "].position", vPointLights[ j ].Position );
					GeometryRender.setUniform( "light[" + to_string( j ) + "].ambient", vPointLights[ j ].Ambient );
					GeometryRender.setUniform( "light[" + to_string( j ) + "].diffuse", vPointLights[ j ].Diffuse );
					GeometryRender.setUniform( "light[" + to_string( j ) + "].specular", vPointLights[ j ].Specular );
					GeometryRender.setUniform( "light[" + to_string( j ) + "].attenuation", vPointLights[ j ].Attenuation );
					
				}

				GeometryRender.setUniform( "UsingLights", CountLights );
				glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
			}
		}
	}

	LoaderVAO::UnbindVAO();
	LoaderVAO::DisableVertexPointer( VERT_POSITION );
	LoaderVAO::DisableVertexPointer( VERT_NORMAL );
	LoaderVAO::DisableVertexPointer( VERT_TEXCOORD );

	Shader::bind( NULL );
	ClearScene();
}

//-------------------------------------------------------------------------//

void le::Scene3D::ClearScene()
{
	mRenderBuffer.clear();
	vPointLights.clear();
}

//-------------------------------------------------------------------------//