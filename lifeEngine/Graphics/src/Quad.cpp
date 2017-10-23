#include <System\VAO.h>
#include "..\Quad.h"

//-------------------------------------------------------------------------//

le::Quad::Quad() : 
	ArrayBuffer( 0 ), 
	VertexBuffer( 0 ), 
	IndexBuffer( 0 ), 
	Size( 1 )
{}

//-------------------------------------------------------------------------//

le::Quad::Quad( const Quad& Copy )
{
	Size = Copy.Size;

	if ( Copy.ArrayBuffer != 0 )
		InitQuad( Size );
	else
		ArrayBuffer = VertexBuffer = IndexBuffer = 0;
}

//-------------------------------------------------------------------------//

le::Quad::~Quad()
{
	if ( ArrayBuffer != 0 )
	{
		VAO::DeleteVAO( &ArrayBuffer );
		VAO::DeleteBuffer( &VertexBuffer );
		VAO::DeleteBuffer( &IndexBuffer );
	}
}

//-------------------------------------------------------------------------//

void le::Quad::InitQuad( float Size )
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

void le::Quad::RenderQuad()
{
	VAO::BindVAO( ArrayBuffer );
	glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
	VAO::UnbindVAO();
}

//-------------------------------------------------------------------------//

void le::Quad::SetSize( float Size )
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

le::Quad& le::Quad::operator=( const Quad & Copy )
{
	Size = Copy.Size;

	if ( Copy.ArrayBuffer != 0 )
		InitQuad( Size );
	else
		ArrayBuffer = VertexBuffer = IndexBuffer = 0;

	return *this;
}

//-------------------------------------------------------------------------//
