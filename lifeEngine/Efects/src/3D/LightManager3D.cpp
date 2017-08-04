#include "..\..\3D\LightManager3D.h"

#include <Level\3D\Level3D.h>

//-------------------------------------------------------------------------//

le::LightManager3D::LightManager3D( System& System ) : Scene( NULL )
{
	string DirShaders = System.GetConfiguration().sShadersDir;
	ProjectionMatrix = &System.GetConfiguration().ProjectionMatrix;
	SizeWindow = System.GetWindow().getSize();
	DepthRender.loadFromFile( DirShaders + "/depthRender.vs", DirShaders + "/depthRender.gs", DirShaders + "/depthRender.fs" );
}

//-------------------------------------------------------------------------//

void le::LightManager3D::InitShadows( Level3D & Level )
{
	vector<Brush*>& vBrushes = Level.GetAllBrushes();
	map<GLuint, SceneInfoMesh>* mRenderMesh;
	Light3D* Light;

	glEnable( GL_DEPTH_TEST );
	glViewport( 0, 0, SHADOW_WIDTH, SHADOW_HEIGHT );

	Shader::bind( &DepthRender );

	for ( int i = 0; i < vLights.size(); i++ )
	{
		Light = &vLights[ i ];

		glBindFramebuffer( GL_FRAMEBUFFER, Light->FBO_DepthMap );
		glClear( GL_DEPTH_BUFFER_BIT );

		for ( int Face = 0; Face < 6; Face++ )
			DepthRender.setUniform( "ShadowMatrices[" + to_string( Face ) + "]", Light->ShadowTransforms[ Face ] );

		DepthRender.setUniform( "LightPosition", Light->Position );
		DepthRender.setUniform( "Far_Plane", Light->fRadius );

		for ( int j = 0; j < vBrushes.size(); j++ )
		{
			mRenderMesh = &vBrushes[ j ]->GetRenderMesh();
			for ( auto it = mRenderMesh->begin(); it != mRenderMesh->end(); it++ )
			{
				DepthRender.setUniform( "TransformMatrix", *( *mRenderMesh )[ it->first ].MatrixTransformation );

				LoaderVAO::BindVAO( ( *mRenderMesh )[ it->first ].VertexArray );
				glDrawElements( GL_TRIANGLES, ( *mRenderMesh )[ it->first ].CountIndexs, GL_UNSIGNED_INT, 0 );
			}
		}
	}

	LoaderVAO::UnbindVAO();
	Shader::bind( NULL );

	glViewport( 0, 0, SizeWindow.x, SizeWindow.y );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glDisable( GL_DEPTH_TEST );
}

//-------------------------------------------------------------------------//

void le::LightManager3D::AddLightsToScene( Scene3D & Scene )
{
	this->Scene = &Scene;
}

//-------------------------------------------------------------------------//

void le::LightManager3D::UpdateLights()
{
	if ( Scene != NULL )
		for ( int i = 0; i < vLights.size(); i++ )
			Scene->AddPointLightToScene( &vLights[ i ] );
}

//-------------------------------------------------------------------------//

void le::LightManager3D::CreateLight( le::Light3D Light3D, string NameLight )
{
	if ( NameLight != "" )
		Light3D.SetName( NameLight );

	vLights.push_back( Light3D );
}

//-------------------------------------------------------------------------//

void le::LightManager3D::DestroyLight( string NameLight )
{
	for ( int i = 0; i < vLights.size(); i++ )
		if ( vLights[ i ].sNameLight == NameLight )
			vLights.erase( i + vLights.begin() );
}

//-------------------------------------------------------------------------//

void le::LightManager3D::DestroyAllLights()
{
	vLights.clear();
}

//-------------------------------------------------------------------------//

le::Light3D* le::LightManager3D::GetLight( string NameLight )
{
	for ( int i = 0; i < vLights.size(); i++ )
		if ( vLights[ i ].sNameLight == NameLight )
			return &vLights[ i ];

	return NULL;
}

//-------------------------------------------------------------------------//
