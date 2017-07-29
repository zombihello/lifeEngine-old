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

	SizeWindow = System.GetWindow().getSize();
	string ShaderDir = System.GetConfiguration().sShadersDir;

	GBuffer.InitGBuffer( SizeWindow.x, SizeWindow.y );
	
	GeometryRender.loadFromFile( ShaderDir + "/vertexShader.vs", ShaderDir + "/fragmentShader.fs" );
	PointLight.loadFromFile( ShaderDir + "/pointlight.vs", ShaderDir + "/pointlight.fs" );

	PointLight.setUniform( "ScreenSize", Glsl::Vec2( SizeWindow ) );
	PointLight.setUniform( "PositionMap", GBuffer::Position );
	PointLight.setUniform( "ColorMap", GBuffer::Textures );
	PointLight.setUniform( "NormalMap", GBuffer::Normal );
}

//-------------------------------------------------------------------------//

le::Scene3D::Scene3D( le::System& System, le::Camera& PlayerCamera )
{
	this->PlayerCamera = &PlayerCamera;
	this->System = &System;

	ProjectionMatrix = &System.GetConfiguration().ProjectionMatrix;
	SizeWindow = System.GetWindow().getSize();
	string ShaderDir = System.GetConfiguration().sShadersDir;

	GBuffer.InitGBuffer( SizeWindow.x, SizeWindow.y );

	GeometryRender.loadFromFile( ShaderDir + "/vertexShader.vs", ShaderDir + "/fragmentShader.fs" );
	PointLight.loadFromFile( ShaderDir + "/pointlight.vs", ShaderDir + "/pointlight.fs" );

	PointLight.setUniform( "ScreenSize", Glsl::Vec2( SizeWindow ) );
	PointLight.setUniform( "PositionMap", GBuffer::Position );
	PointLight.setUniform( "ColorMap", GBuffer::Textures );
	PointLight.setUniform( "NormalMap", GBuffer::Normal );
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

void le::Scene3D::AddPointLightToScene( le::Light3D* Light )
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
	if ( PlayerCamera != NULL )
		ViewMatrix = PlayerCamera->GetViewMatrix();

	PVMatrix = *ProjectionMatrix * ViewMatrix;

	GeometryPass();
	LightPass();

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

void le::Scene3D::GeometryPass()
{
	vector<SceneInfoMesh>* vRenderBuffer_Meshes;
	SceneInfoMesh* InfoMesh;

	GeometryRender.setUniform( "PVMatrix", PVMatrix );

	Shader::bind( &GeometryRender );

	GBuffer.BindForWriting();
	glDepthMask( GL_TRUE );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glEnable( GL_DEPTH_TEST );
	glDisable( GL_BLEND );

	for ( auto it = mRenderBuffer.begin(); it != mRenderBuffer.end(); it++ )
	{
		vRenderBuffer_Meshes = &it->second;
		glBindTexture( GL_TEXTURE_2D, it->first );

		for ( int i = 0; i < vRenderBuffer_Meshes->size(); i++ )
		{
			InfoMesh = &( *vRenderBuffer_Meshes )[ i ];

			GeometryRender.setUniform( "transformationMatrix", *InfoMesh->MatrixTransformation );

			LoaderVAO::BindVAO( InfoMesh->VertexArray );
			glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
		}
	}

	LoaderVAO::UnbindVAO();		

	glDepthMask( GL_FALSE );
	glDisable( GL_DEPTH_TEST );
}

//-------------------------------------------------------------------------//

void le::Scene3D::LightPass()
{
	glEnable( GL_BLEND );
	glBlendEquation( GL_MAX );
	glBlendFunc( GL_ONE, GL_ONE );

	GBuffer.BindForReading();
	glClear( GL_COLOR_BUFFER_BIT );

	PointLightPass();
}

//-------------------------------------------------------------------------//

void le::Scene3D::PointLightPass()
{
	Shader::bind( &PointLight );

	PointLight.setUniform( "PVMatrix", PVMatrix );

	for ( int i = 0; i < vPointLights.size(); i++ )
	{
		PointLight.setUniform( "transformationMatrix", vPointLights[i]->LightSphere.GetTransformationMatrix() );
		PointLight.setUniform( "light.Position", vPointLights[ i ]->Position );
		PointLight.setUniform( "light.Color", vPointLights[ i ]->Color );
		PointLight.setUniform( "light.Intensivity", vPointLights[ i ]->fIntensivity );
		PointLight.setUniform( "light.Radius", vPointLights[ i ]->fRadius );

		vPointLights[ i ]->LightSphere.RenderSphere();
	}
}

//-------------------------------------------------------------------------//