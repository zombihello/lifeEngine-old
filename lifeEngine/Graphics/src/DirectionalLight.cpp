#include <System\System.h>
#include <System\VAO.h>
#include "..\DirectionalLight.h"

//-------------------------------------------------------------------------//

le::DirectionalLight::DirectionalLight()
{
	Logger::Log( Logger::Info, "Creating Directional Light" );
	Logger::Log( Logger::None, "  StaticLight: " + to_string( IsStaticLight ) );
	Logger::Log( Logger::None, "  Intensivity: " + to_string( Intensivity ) );
	Logger::Log( Logger::None, "  Position: " + to_string( Position.x ) + " " + to_string( Position.y ) + " " + to_string( Position.z ) );
	Logger::Log( Logger::None, "  Color: " + to_string( Color.x ) + " " + to_string( Color.y ) + " " + to_string( Color.z ) + " " + to_string( Color.w ) );
	Logger::Log( Logger::None, "  Specular: " + to_string( Specular.x ) + " " + to_string( Specular.y ) + " " + to_string( Specular.z ) + " " + to_string( Specular.w ) );

	Position.w = 0;
	Quad.InitQuad( 1 );

	Logger::Log( Logger::Info, "Created Directional Light" );
}

//-------------------------------------------------------------------------//

le::DirectionalLight::DirectionalLight( const glm::vec3& Position, const glm::vec4& Color, float Intensivity, const glm::vec4& Specular )
{
	Logger::Log( Logger::Info, "Creating Directional Light" );
	Logger::Log( Logger::None, "  StaticLight: " + to_string( IsStaticLight ) );
	Logger::Log( Logger::None, "  Intensivity: " + to_string( Intensivity ) );
	Logger::Log( Logger::None, "  Position: " + to_string( Position.x ) + " " + to_string( Position.y ) + " " + to_string( Position.z ) );
	Logger::Log( Logger::None, "  Color: " + to_string( Color.x ) + " " + to_string( Color.y ) + " " + to_string( Color.z ) + " " + to_string( Color.w ) );
	Logger::Log( Logger::None, "  Specular: " + to_string( Specular.x ) + " " + to_string( Specular.y ) + " " + to_string( Specular.z ) + " " + to_string( Specular.w ) );

	this->Position = glm::vec4( Position, 0.0f );
	this->Color = Color / 255.f;
	this->Specular = Specular / 255.f;
	this->Intensivity = Intensivity;

	Quad.InitQuad( 1 );

	Logger::Log( Logger::Info, "Created Directional Light" );
}

//-------------------------------------------------------------------------//

le::DirectionalLight::DirectionalLight( const DirectionalLight& Copy )
{
	CopyBaseLight( Copy );

	Quad = Copy.Quad;
}

//-------------------------------------------------------------------------//

le::DirectionalLight::~DirectionalLight()
{}

//-------------------------------------------------------------------------//

void le::DirectionalLight::SetPosition( const glm::vec3& Position )
{
	this->Position = glm::vec4( Position, 0.0f );
}

//-------------------------------------------------------------------------//

le::DirectionalLight& le::DirectionalLight::operator=( const DirectionalLight& Copy )
{
	CopyBaseLight( Copy );

	Quad = Copy.Quad;

	return *this;
}

//-------------------------------------------------------------------------//

///////////////////////////////////
//			КЛАСС КВАДРАТА
//////////////////////////////////

//-------------------------------------------------------------------------//

le::DirectionalLight::LightQuad::LightQuad() :
	ArrayBuffer( 0 ),
	VertexBuffer( 0 ),
	IndexBuffer( 0 ),
	Size( 1 )
{}

//-------------------------------------------------------------------------//

le::DirectionalLight::LightQuad::LightQuad( const le::DirectionalLight::LightQuad& Copy )
{
	Size = Copy.Size;

	if ( Copy.ArrayBuffer != 0 )
		InitQuad( Size );
	else
		ArrayBuffer = VertexBuffer = IndexBuffer = 0;
}

//-------------------------------------------------------------------------//

le::DirectionalLight::LightQuad::~LightQuad()
{
	if ( ArrayBuffer != 0 )
	{
		VAO::DeleteVAO( &ArrayBuffer );
		VAO::DeleteBuffer( &VertexBuffer );
		VAO::DeleteBuffer( &IndexBuffer );
	}
}

//-------------------------------------------------------------------------//

void le::DirectionalLight::LightQuad::InitQuad( float Size )
{
	this->Size = Size;

	unsigned int IdVertexes[] = { 0, 1, 2, 1, 3, 2 };

	glm::vec3 Vertexes[] =
	{
		{ -Size, -Size, 0 },
		{ Size, -Size, 0 },
		{ -Size,  Size, 0 },
		{ Size,  Size, 0 }
	};

	ArrayBuffer = VAO::CreateVAO();
	VAO::BindVAO( ArrayBuffer );

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, 4, Vertexes, VAO::Static_Draw );
	IndexBuffer = VAO::CreateBuffer( VAO::Index_Buffer, 6, IdVertexes, VAO::Static_Draw );

	VAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), 0 );

	VAO::UnbindVAO();
	VAO::UnbindBuffer( VAO::Vertex_Buffer );
	VAO::UnbindBuffer( VAO::Index_Buffer );
}

//-------------------------------------------------------------------------//

void le::DirectionalLight::LightQuad::RenderQuad()
{
	VAO::BindVAO( ArrayBuffer );
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
	VAO::UnbindVAO();
}

//-------------------------------------------------------------------------//

void le::DirectionalLight::LightQuad::SetSize( float Size )
{
	this->Size = Size;

	glm::vec3 Vertexes[] =
	{
		{ -Size, -Size, 0 },
		{ Size, -Size, 0 },
		{ -Size,  Size, 0 },
		{ Size,  Size, 0 }
	};

	glBindBuffer( GL_ARRAY_BUFFER, VertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, 4 * sizeof( glm::vec3 ), Vertexes, GL_STATIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

//-------------------------------------------------------------------------//

le::DirectionalLight::LightQuad& le::DirectionalLight::LightQuad::operator=( const le::DirectionalLight::LightQuad& Copy )
{
	Size = Copy.Size;

	if ( Copy.ArrayBuffer != 0 )
		InitQuad( Size );
	else
		ArrayBuffer = VertexBuffer = IndexBuffer = 0;

	return *this;
}

//-------------------------------------------------------------------------//
