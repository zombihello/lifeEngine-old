#include "../../3D/Scene3D.h"

/*
Vertex Shader
#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec2 out_TexCoord;
out vec3 surfaceNormal;
out vec3 toLightVector;
out float Distance;

uniform mat4 transformationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

void main()
{
	vec4 worldPosition = transformationMatrix * vec4( position, 1.0f );	
    gl_Position =  projectionMatrix * viewMatrix * worldPosition;
	
    out_TexCoord = texCoord;
	surfaceNormal = (transformationMatrix * vec4(normal,0.0f)).xyz;
	toLightVector = vec3(-8,48,-8) - worldPosition.xyz;
	
	Distance = length(toLightVector);
}*/

/*
Fragment Shader
#version 400 core

in vec2 out_TexCoord;
in vec3 surfaceNormal;
in vec3 toLightVector;
in float Distance;

out vec4 color;

uniform sampler2D ourTexture;
uniform vec3 lightColor;

void main()
{
vec3 unitNormal = normalize(surfaceNormal);
vec3 unitLightVector = normalize(toLightVector);

float attenuation = ( 1 + 0 * Distance + 0 * Distance * Distance );

float nDorl = dot(unitNormal,unitLightVector);
float brightness = max(nDorl,0.2);
vec3 diffuse = (brightness * vec3(1,1,1))/attenuation;

color =  vec4(diffuse,1.0) * texture(ourTexture, out_TexCoord);
}
*/

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

	ShaderRender.loadFromFile( "vertexShader.vs", "fragmentShader.fs" );
}

//-------------------------------------------------------------------------//

#include <Efects\3D\LightManager3D.h>
le::Light3D l;
le::Scene3D::Scene3D( le::System& System, le::Camera& PlayerCamera )
{
	this->PlayerCamera = &PlayerCamera;
	this->System = &System;

	ProjectionMatrix = &System.GetConfiguration().ProjectionMatrix;

	ShaderRender.loadFromFile( "vertexShader.vs", "fragmentShader.fs" );


	l.Position = glm::vec3( 0, 48, 0 );
	l.Color = glm::vec3( 1, 1, 1 );

	ShaderRender.setUniform( "lightPosition", Glsl::Vec3( l.Position.x, l.Position.y, l.Position.z ) );
	ShaderRender.setUniform( "lightColor", Glsl::Vec3( l.Color.x, l.Color.y, l.Color.z ) );
}

//-------------------------------------------------------------------------//

void le::Scene3D::AddMeshToScene( GLuint Texture, SceneInfoMesh Mesh )
{
	mRenderBuffer[Texture].push_back( Mesh );
}

//-------------------------------------------------------------------------//

void le::Scene3D::AddMeshToScene( map<GLuint, SceneInfoMesh> Mesh )
{
	for ( auto it = Mesh.begin(); it != Mesh.end(); it++ )
		mRenderBuffer[it->first].push_back( it->second );
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

	ShaderRender.setUniform( "projectionMatrix", glm::value_ptr( *ProjectionMatrix ) );
	ShaderRender.setUniform( "viewMatrix", glm::value_ptr( ViewMatrix ) );

	Shader::bind( &ShaderRender );

	for ( auto it = mRenderBuffer.begin(); it != mRenderBuffer.end(); it++ )
	{
		vRenderBuffer_Meshes = &it->second;
		glBindTexture( GL_TEXTURE_2D, it->first );

		for ( int i = 0; i < vRenderBuffer_Meshes->size(); i++ )
		{
			InfoMesh = &( *vRenderBuffer_Meshes )[i];
			ShaderRender.setUniform( "transformationMatrix", glm::value_ptr( *InfoMesh->MatrixTransformation ) );

			LoaderVAO::BindVAO( InfoMesh->VertexArray );
			LoaderVAO::EnableVertexPointer( VERT_POSITION );
			LoaderVAO::EnableVertexPointer( VERT_NORMAL );
			LoaderVAO::EnableVertexPointer( VERT_TEXCOORD );

			glDrawElements( GL_TRIANGLES, InfoMesh->CountIndexs, GL_UNSIGNED_INT, 0 );	
		}
	}

	Shader::bind( NULL );

	LoaderVAO::DisableVertexPointer( VERT_POSITION );
	LoaderVAO::DisableVertexPointer( VERT_NORMAL );
	LoaderVAO::DisableVertexPointer( VERT_TEXCOORD );
	LoaderVAO::UnbindVAO();

	ClearScene();
}

//-------------------------------------------------------------------------//

void le::Scene3D::ClearScene()
{
	mRenderBuffer.clear();
}

//-------------------------------------------------------------------------//