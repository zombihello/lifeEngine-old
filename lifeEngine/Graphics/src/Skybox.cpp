#include <System\VAO.h>
#include <System\ResourcesManager.h>
#include <Graphics\Camera.h>
#include "..\Skybox.h"

//-------------------------------------------------------------------------//

le::Skybox::Skybox( System& System ) :
	CubeTexture ( 0 ),
	SizeSkybox( 1 ),
	IsLoadSkybox( false ),
	SkyboxRender( NULL ),
	PositionCamera( NULL ),
	ViewMatrix( NULL )
{
	ResourcesManager::LoadShader( "Skybox", "../shaders/SkyboxRender.vs", "../shaders/SkyboxRender.fs" );
	SkyboxRender = ResourcesManager::GetShader( "Skybox" );
	ProjectionMatrix = &System.Configuration.ProjectionMatrix;

	glm::vec3 Vertexes[] =
	{
		{ -1.0f,  1.0f,  1.0f },
		{ -1.0f, -1.0f,  1.0f },
		{ 1.0f, -1.0f,  1.0f },
		{ 1.0f,  1.0f,  1.0f },
		{ -1.0f,  1.0f, -1.0f },
		{ -1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ 1.0f,  1.0f, -1.0f }
	};

	unsigned int IdVertexes[] =
	{
		7, 3, 4, 3, 0, 4, 2, 6, 1,
		6, 5, 1, 7, 6, 3, 6, 2, 3,
		0, 1, 4, 1, 5, 4, 6, 4, 5,
		6, 7, 4, 0, 2, 1, 0, 3, 2
	};

	ArrayBuffer = VAO::CreateVAO();
	VAO::BindVAO( ArrayBuffer );

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, 8, Vertexes, VAO::Static_Draw );
	IndexBuffer = VAO::CreateBuffer( VAO::Index_Buffer, 36, IdVertexes, VAO::Static_Draw );

	VAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), 0 );

	VAO::UnbindVAO();
	VAO::UnbindBuffer( VAO::Vertex_Buffer );
	VAO::UnbindBuffer( VAO::Index_Buffer );
}

//-------------------------------------------------------------------------//

le::Skybox::Skybox( Skybox& Copy )
{	
	SizeSkybox = Copy.SizeSkybox;
	IsLoadSkybox = Copy.IsLoadSkybox;
	
	SkyboxRender = Copy.SkyboxRender;
	PositionCamera = Copy.PositionCamera;
	ProjectionMatrix = Copy.ProjectionMatrix;
	ViewMatrix = Copy.ViewMatrix;

	if ( IsLoadSkybox )
		LoadSkybox( Copy.RouteSkyBox );

	glm::vec3 Vertexes[] =
	{
		{ -SizeSkybox,  SizeSkybox,  SizeSkybox },
		{ -SizeSkybox, -SizeSkybox,  SizeSkybox },
		{ SizeSkybox, -SizeSkybox,  SizeSkybox },
		{ SizeSkybox,  SizeSkybox,  SizeSkybox },
		{ -SizeSkybox,  SizeSkybox, -SizeSkybox },
		{ -SizeSkybox, -SizeSkybox, -SizeSkybox },
		{ SizeSkybox, -SizeSkybox, -SizeSkybox },
		{ SizeSkybox,  SizeSkybox, -SizeSkybox }
	};

	unsigned int IdVertexes[] =
	{
		7, 3, 4, 3, 0, 4, 2, 6, 1,
		6, 5, 1, 7, 6, 3, 6, 2, 3,
		0, 1, 4, 1, 5, 4, 6, 4, 5,
		6, 7, 4, 0, 2, 1, 0, 3, 2
	};

	ArrayBuffer = VAO::CreateVAO();
	VAO::BindVAO( ArrayBuffer );

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, 8, Vertexes, VAO::Static_Draw );
	IndexBuffer = VAO::CreateBuffer( VAO::Index_Buffer, 36, IdVertexes, VAO::Static_Draw );

	VAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), 0 );

	VAO::UnbindVAO();
	VAO::UnbindBuffer( VAO::Vertex_Buffer );
	VAO::UnbindBuffer( VAO::Index_Buffer );
}

//-------------------------------------------------------------------------//

le::Skybox::~Skybox()
{
	ResourcesManager::DeleteGlCubeTexture( "Skybox" );

	VAO::DeleteBuffer( &VertexBuffer );
	VAO::DeleteVAO( &ArrayBuffer );
	VAO::DeleteBuffer( &IndexBuffer );
}

//-------------------------------------------------------------------------//

bool le::Skybox::LoadSkybox( const string& Route )
{
	string FormatTexture = Route;
	string NameTexture = Route;

	FormatTexture.erase( 0, FormatTexture.find_last_of( '.' ) );
	NameTexture.erase( NameTexture.find_last_of( '.' ), NameTexture.size() );

	vector<string> TextureSides =
	{
		NameTexture + "_right" + FormatTexture,
		NameTexture + "_left" + FormatTexture,
		NameTexture + "_top" + FormatTexture,
		NameTexture + "_bottom" + FormatTexture,
		NameTexture + "_back" + FormatTexture,
		NameTexture + "_front" + FormatTexture
	};

	if ( !ResourcesManager::LoadGlCubeTexture( "Skybox", TextureSides ) )
	{
		IsLoadSkybox = false;
		return false;
	}
	
	CubeTexture = ResourcesManager::GetGlCubeTexture( "Skybox" );
	RouteSkyBox = Route;
	IsLoadSkybox = true;
	return true;
}

//-------------------------------------------------------------------------//

void le::Skybox::RenderSkybox()
{
	bool IsValidSkybox = SkyboxRender == NULL || PositionCamera == NULL || ViewMatrix == NULL;

	if ( IsValidSkybox ) return;

	Shader::bind( SkyboxRender );	
	VAO::BindVAO( ArrayBuffer );
	glBindTexture( GL_TEXTURE_CUBE_MAP, CubeTexture );

	SkyboxRender->setUniform( "PVMatrix", *ProjectionMatrix * *ViewMatrix );	
	SkyboxRender->setUniform( "PositionCamera", *PositionCamera );
	glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0 );

	VAO::UnbindVAO();
	Shader::bind( NULL );
}

//-------------------------------------------------------------------------//

void le::Skybox::RemovePlayerCamera()
{
	PositionCamera = NULL;
	ViewMatrix = NULL;
}

//-------------------------------------------------------------------------//

void le::Skybox::SetPlayerCamera( Camera& Camera )
{
	PositionCamera = &Camera.GetPosition();
	ViewMatrix = &Camera.GetViewMatrix();
}

//-------------------------------------------------------------------------//

void le::Skybox::SetSizeSkybox( float Size )
{
	SizeSkybox = Size;

	glm::vec3 Vertexes[] =
	{
		{ -SizeSkybox,  SizeSkybox,  SizeSkybox },
		{ -SizeSkybox, -SizeSkybox,  SizeSkybox },
		{ SizeSkybox, -SizeSkybox,  SizeSkybox },
		{ SizeSkybox,  SizeSkybox,  SizeSkybox },
		{ -SizeSkybox,  SizeSkybox, -SizeSkybox },
		{ -SizeSkybox, -SizeSkybox, -SizeSkybox },
		{ SizeSkybox, -SizeSkybox, -SizeSkybox },
		{ SizeSkybox,  SizeSkybox, -SizeSkybox }
	};

	glBindBuffer( GL_ARRAY_BUFFER, VertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, 8 * sizeof( glm::vec3 ), Vertexes, GL_STATIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

//-------------------------------------------------------------------------//

le::Skybox& le::Skybox::operator=( const Skybox & Copy )
{
	SizeSkybox = Copy.SizeSkybox;
	IsLoadSkybox = Copy.IsLoadSkybox;

	SkyboxRender = Copy.SkyboxRender;
	PositionCamera = Copy.PositionCamera;
	ProjectionMatrix = Copy.ProjectionMatrix;
	ViewMatrix = Copy.ViewMatrix;

	if ( IsLoadSkybox )
		LoadSkybox( Copy.RouteSkyBox );

	glm::vec3 Vertexes[] =
	{
		{ -SizeSkybox,  SizeSkybox,  SizeSkybox },
		{ -SizeSkybox, -SizeSkybox,  SizeSkybox },
		{ SizeSkybox, -SizeSkybox,  SizeSkybox },
		{ SizeSkybox,  SizeSkybox,  SizeSkybox },
		{ -SizeSkybox,  SizeSkybox, -SizeSkybox },
		{ -SizeSkybox, -SizeSkybox, -SizeSkybox },
		{ SizeSkybox, -SizeSkybox, -SizeSkybox },
		{ SizeSkybox,  SizeSkybox, -SizeSkybox }
	};

	unsigned int IdVertexes[] =
	{
		7, 3, 4, 3, 0, 4, 2, 6, 1,
		6, 5, 1, 7, 6, 3, 6, 2, 3,
		0, 1, 4, 1, 5, 4, 6, 4, 5,
		6, 7, 4, 0, 2, 1, 0, 3, 2
	};

	ArrayBuffer = VAO::CreateVAO();
	VAO::BindVAO( ArrayBuffer );

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, 8, Vertexes, VAO::Static_Draw );
	IndexBuffer = VAO::CreateBuffer( VAO::Index_Buffer, 36, IdVertexes, VAO::Static_Draw );

	VAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), 0 );

	VAO::UnbindVAO();
	VAO::UnbindBuffer( VAO::Vertex_Buffer );
	VAO::UnbindBuffer( VAO::Index_Buffer );

	return *this;
}

//-------------------------------------------------------------------------//
