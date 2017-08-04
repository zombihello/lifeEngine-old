#include "../../3D/Scene3D.h"

#define GEOMETRY_RENDER 0
#define STENCIL_TEST 1
#define POINT_LIGHT 2

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

	SceneRender.loadFromFile( ShaderDir + "/sceneRender.vs", ShaderDir + "/sceneRender.fs" );

	SceneRender.setUniform( "ScreenSize", Glsl::Vec2( SizeWindow ) );
	SceneRender.setUniform( "PositionMap", GBuffer::Position );
	SceneRender.setUniform( "NormalMap", GBuffer::Normal );
	SceneRender.setUniform( "DepthMap", 3 );
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

	SceneRender.loadFromFile( ShaderDir + "/sceneRender.vs", ShaderDir + "/sceneRender.fs" );

	SceneRender.setUniform( "ScreenSize", Glsl::Vec2( SizeWindow ) );
	SceneRender.setUniform( "PositionMap", GBuffer::Position );
	SceneRender.setUniform( "NormalMap", GBuffer::Normal );
	SceneRender.setUniform( "DepthMap", 3 );
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
	Shader::bind( &SceneRender );
	glEnable( GL_TEXTURE_2D );

	if ( PlayerCamera != NULL )
		ViewMatrix = PlayerCamera->GetViewMatrix();
	PVMatrix = *ProjectionMatrix * ViewMatrix;

	GBuffer.ClearFrame();

	GeometryPass();
	LightPass();

	if ( !Keyboard::isKeyPressed( Keyboard::E ) )
		GBuffer.RenderFrame( SizeWindow );
	else
		GBuffer.ShowDebug( SizeWindow );

	
	ClearScene();
	glDisable( GL_TEXTURE_2D );
	Shader::bind( NULL );
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

	SceneRender.setUniform( "TypeShader", GEOMETRY_RENDER );
	SceneRender.setUniform( "ColorMap", 0 );
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

			SceneRender.setUniform( "PVTMatrix", PVMatrix * (*InfoMesh->MatrixTransformation) );
			SceneRender.setUniform( "TransformMatrix", *InfoMesh->MatrixTransformation );

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
	SceneRender.setUniform( "ColorMap", GBuffer::Textures );
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
	SceneRender.setUniform( "TypeShader", STENCIL_TEST );
	GBuffer.BindForStencilTest();

	glEnable( GL_DEPTH_TEST );
	glClear( GL_STENCIL_BUFFER_BIT );
	glDisable( GL_CULL_FACE );

	glStencilFunc( GL_ALWAYS, 0, 0 );
	glStencilOpSeparate( GL_FRONT, GL_KEEP, GL_INCR_WRAP, GL_KEEP );
	glStencilOpSeparate( GL_BACK, GL_KEEP, GL_DECR_WRAP, GL_KEEP );

	SceneRender.setUniform( "PVTMatrix", PVMatrix * vPointLights[ IndexLight ]->LightSphere.GetTransformationMatrix() );
	vPointLights[ IndexLight ]->LightSphere.RenderSphere();
}

//-------------------------------------------------------------------------//

void le::Scene3D::RenderPointLight( int IndexLight )
{
	SceneRender.setUniform( "TypeShader", POINT_LIGHT );
	GBuffer.BindForRenderLight();

	glActiveTexture( GL_TEXTURE0 + GBUFFER_NUM_TEXTURES );
	glBindTexture( GL_TEXTURE_CUBE_MAP, vPointLights[ IndexLight ]->Cubemap_DepthMap );

	glStencilFunc( GL_NOTEQUAL, 0, 0xFF );
	glDisable( GL_DEPTH_TEST );

	glEnable( GL_BLEND );
	glBlendEquation( GL_FUNC_ADD );
	glBlendFunc( GL_ONE, GL_ONE );

	glEnable( GL_CULL_FACE );

	SceneRender.setUniform( "PVTMatrix", PVMatrix * vPointLights[ IndexLight ]->LightSphere.GetTransformationMatrix() );
	SceneRender.setUniform( "light.Position", vPointLights[ IndexLight ]->Position );
	SceneRender.setUniform( "light.Color", vPointLights[ IndexLight ]->Color );
	SceneRender.setUniform( "light.Intensivity", vPointLights[ IndexLight ]->fIntensivity );
	SceneRender.setUniform( "light.Radius", vPointLights[ IndexLight ]->fRadius );

	vPointLights[ IndexLight ]->LightSphere.RenderSphere();

	glDisable( GL_BLEND );
}

//-------------------------------------------------------------------------//