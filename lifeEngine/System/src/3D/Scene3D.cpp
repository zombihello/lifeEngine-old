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

	GeometryRender.loadFromFile( ShaderDir + "/geometryRender.vs", ShaderDir + "/geometryRender.fs" );
	PointLight.loadFromFile( ShaderDir + "/pointlight.vs", ShaderDir + "/pointlight.fs" );
	StencilTest.loadFromFile( ShaderDir + "/stencilTest.vs", ShaderDir + "/stencilTest.fs" );

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

	GeometryRender.loadFromFile( ShaderDir + "/geometryRender.vs", ShaderDir + "/geometryRender.fs" );
	PointLight.loadFromFile( ShaderDir + "/pointlight.vs", ShaderDir + "/pointlight.fs" );
	StencilTest.loadFromFile( ShaderDir + "/stencilTest.vs", ShaderDir + "/stencilTest.fs" );

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
	glEnable( GL_TEXTURE_2D );

	if ( PlayerCamera != NULL )
		ViewMatrix = PlayerCamera->GetViewMatrix();
	PVMatrix = *ProjectionMatrix * ViewMatrix;

	GBuffer.ClearFrame();

	GeometryPass();
	LightPass();

	GBuffer.RenderFrame( SizeWindow );

	Shader::bind( NULL );
	ClearScene();

	glDisable( GL_TEXTURE_2D );
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

	Shader::bind( &GeometryRender );
	GBuffer.BindForRenderBuffers();

	glDepthMask( GL_TRUE );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );

	for ( auto it = mRenderBuffer.begin(); it != mRenderBuffer.end(); it++ )
	{
		vRenderBuffer_Meshes = &it->second;
		glBindTexture( GL_TEXTURE_2D, it->first );

		for ( int i = 0; i < vRenderBuffer_Meshes->size(); i++ )
		{
			InfoMesh = &( *vRenderBuffer_Meshes )[ i ];

			GeometryRender.setUniform( "PVTMatrix", PVMatrix * (*InfoMesh->MatrixTransformation) );
			GeometryRender.setUniform( "TransformMatrix", *InfoMesh->MatrixTransformation );

			LoaderVAO::BindVAO( InfoMesh->VertexArray );
			glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );
		}
	}

	LoaderVAO::UnbindVAO();

	glDepthMask( GL_FALSE );
}

//-------------------------------------------------------------------------//

void le::Scene3D::LightPass()
{
	glEnable( GL_STENCIL_TEST );

	for ( int i = 0; i < vPointLights.size(); i++ )
	{
		StencilTestPointLight( i );
		RenderPointLight( i );
	}

	glDisable( GL_STENCIL_TEST );
}

//-------------------------------------------------------------------------//

void le::Scene3D::StencilTestPointLight( int IndexLight )
{
	Shader::bind( &StencilTest );
	GBuffer.BindForStencilTest();

	glEnable( GL_DEPTH_TEST );
	glClear( GL_STENCIL_BUFFER_BIT );
	glDisable( GL_CULL_FACE );

	glStencilFunc( GL_ALWAYS, 0, 0 );
	glStencilOpSeparate( GL_FRONT, GL_KEEP, GL_INCR_WRAP, GL_KEEP );
	glStencilOpSeparate( GL_BACK, GL_KEEP, GL_DECR_WRAP, GL_KEEP );

	StencilTest.setUniform( "PVTMatrix", PVMatrix * vPointLights[ IndexLight ]->LightSphere.GetTransformationMatrix() );
	vPointLights[ IndexLight ]->LightSphere.RenderSphere();
}

//-------------------------------------------------------------------------//

void le::Scene3D::RenderPointLight( int IndexLight )
{
	Shader::bind( &PointLight );
	GBuffer.BindForRenderLight();

	glStencilFunc( GL_NOTEQUAL, 0, 0xFF );
	glDisable( GL_DEPTH_TEST );

	glEnable( GL_BLEND );
	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_ONE, GL_ONE );

	glEnable( GL_CULL_FACE );

	PointLight.setUniform( "PVTMatrix", PVMatrix * vPointLights[ IndexLight ]->LightSphere.GetTransformationMatrix() );
	PointLight.setUniform( "light.Position", vPointLights[ IndexLight ]->Position );
	PointLight.setUniform( "light.Color", vPointLights[ IndexLight ]->Color );
	PointLight.setUniform( "light.Intensivity", vPointLights[ IndexLight ]->fIntensivity );
	PointLight.setUniform( "light.Radius", vPointLights[ IndexLight ]->fRadius );

	vPointLights[ IndexLight ]->LightSphere.RenderSphere();

	glDisable( GL_BLEND );
}

//-------------------------------------------------------------------------//