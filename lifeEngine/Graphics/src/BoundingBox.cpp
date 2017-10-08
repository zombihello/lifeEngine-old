#include "..\BoundingBox.h"
#include <System\VAO.h>

//-------------------------------------------------------------------------//

le::BoundingBox::BoundingBox() :
	ArrayBuffer( 0 ),
	VertexBuffer( 0 ),
	IndexBuffer( 0 )
{}

//-------------------------------------------------------------------------//

le::BoundingBox::BoundingBox( BoundingBox& Copy )
{
	glm::vec3* Vertexs = Copy.GetVertexs();
	Query = Copy.Query;

	for ( int i = 0; i < 8; i++ )
		this->Vertexs[ i ] = Vertexs[ i ];

	if ( Copy.ArrayBuffer != 0 )
	{
		ArrayBuffer = VAO::CreateVAO();
		VAO::BindVAO( ArrayBuffer );

		unsigned int IdVertexs[ 36 ] =
		{
			7, 3, 4, 3, 0, 4, 2, 6, 1,
			6, 5, 1, 7, 6, 3, 6, 2, 3,
			0, 1, 4, 1, 5, 4, 6, 4, 5,
			6, 7, 4, 0, 2, 1, 0, 3, 2
		};

		VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, 8, Vertexs, VAO::Static_Draw );
		IndexBuffer = VAO::CreateBuffer( VAO::Index_Buffer, 36, IdVertexs, VAO::Static_Draw );

		VAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), 0 );

		VAO::UnbindVAO();
		VAO::UnbindBuffer( VAO::Vertex_Buffer );
		VAO::UnbindBuffer( VAO::Index_Buffer );
	}
	else
		ArrayBuffer = IndexBuffer = VertexBuffer = 0;
}

//-------------------------------------------------------------------------//

le::BoundingBox::~BoundingBox()
{
	if ( ArrayBuffer != 0 )
	{
		VAO::DeleteVAO( &ArrayBuffer );
		VAO::DeleteBuffer( &IndexBuffer );
		VAO::DeleteBuffer( &VertexBuffer );
	}
}

//-------------------------------------------------------------------------//

void le::BoundingBox::InitBox( const glm::vec3& MinVertex, const glm::vec3& MaxVertex )
{
	Vertexs[ 0 ] = glm::vec3( MinVertex.x, MinVertex.z, 0 );
	Vertexs[ 1 ] = glm::vec3( MaxVertex.x, MinVertex.z, 0 );
	Vertexs[ 2 ] = glm::vec3( MinVertex.x, MaxVertex.z, 0 );
	Vertexs[ 3 ] = glm::vec3( MaxVertex.x, MaxVertex.z, 0 );
	Vertexs[ 4 ] = glm::vec3( MinVertex.x, MinVertex.z, MaxVertex.y );
	Vertexs[ 5 ] = glm::vec3( MaxVertex.x, MinVertex.z, MaxVertex.y );
	Vertexs[ 6 ] = glm::vec3( MinVertex.x, MaxVertex.z, MaxVertex.y );
	Vertexs[ 7 ] = glm::vec3( MaxVertex.x, MaxVertex.z, MaxVertex.y );

	if ( ArrayBuffer != 0 ) return;

	ArrayBuffer = VAO::CreateVAO();
	VAO::BindVAO( ArrayBuffer );

	unsigned int IdVertexs[ 36 ] =
	{
		7, 3, 4, 3, 0, 4, 2, 6, 1,
		6, 5, 1, 7, 6, 3, 6, 2, 3,
		0, 1, 4, 1, 5, 4, 6, 4, 5,
		6, 7, 4, 0, 2, 1, 0, 3, 2
	};

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, 8, Vertexs, VAO::Static_Draw );
	IndexBuffer = VAO::CreateBuffer( VAO::Index_Buffer, 36, IdVertexs, VAO::Static_Draw );

	VAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), 0 );

	VAO::UnbindVAO();
	VAO::UnbindBuffer( VAO::Vertex_Buffer );
	VAO::UnbindBuffer( VAO::Index_Buffer );
}

//-------------------------------------------------------------------------//

void le::BoundingBox::InitBox( const vector<glm::vec3>& Vertexs )
{
	for ( int i = 0; i < 8; i++ )
		this->Vertexs[ i ] = Vertexs[ i ];

	if ( ArrayBuffer != 0 ) return;

	ArrayBuffer = VAO::CreateVAO();
	VAO::BindVAO( ArrayBuffer );

	unsigned int IdVertexs[ 36 ] =
	{
		7, 3, 4, 3, 0, 4, 2, 6, 1,
		6, 5, 1, 7, 6, 3, 6, 2, 3,
		0, 1, 4, 1, 5, 4, 6, 4, 5,
		6, 7, 4, 0, 2, 1, 0, 3, 2
	};

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, 8, this->Vertexs, VAO::Static_Draw );
	IndexBuffer = VAO::CreateBuffer( VAO::Index_Buffer, 36, IdVertexs, VAO::Static_Draw );

	VAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), 0 );

	VAO::UnbindVAO();
	VAO::UnbindBuffer( VAO::Vertex_Buffer );
	VAO::UnbindBuffer( VAO::Index_Buffer );
}

//-------------------------------------------------------------------------//

glm::vec3* le::BoundingBox::GetVertexs()
{
	return &Vertexs[ 0 ];
}

//-------------------------------------------------------------------------//

void le::BoundingBox::QueryTest()
{
	Query.Start();

	VAO::BindVAO( ArrayBuffer );
	glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0 );
	VAO::UnbindVAO();

	Query.End();
}

//-------------------------------------------------------------------------//

void le::BoundingBox::RenderBox()
{
	VAO::BindVAO( ArrayBuffer );
	glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0 );
	VAO::UnbindVAO();
}

//-------------------------------------------------------------------------//

le::BoundingBox & le::BoundingBox::operator=( const BoundingBox& Copy )
{
	Query = Copy.Query;

	for ( int i = 0; i < 8; i++ )
		Vertexs[ i ] = Copy.Vertexs[ i ];

	if ( Copy.ArrayBuffer != 0 )
	{
		ArrayBuffer = VAO::CreateVAO();
		VAO::BindVAO( ArrayBuffer );

		unsigned int IdVertexs[ 36 ] =
		{
			7, 3, 4, 3, 0, 4, 2, 6, 1,
			6, 5, 1, 7, 6, 3, 6, 2, 3,
			0, 1, 4, 1, 5, 4, 6, 4, 5,
			6, 7, 4, 0, 2, 1, 0, 3, 2
		};

		VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, 8, Vertexs, VAO::Static_Draw );
		IndexBuffer = VAO::CreateBuffer( VAO::Index_Buffer, 36, IdVertexs, VAO::Static_Draw );

		VAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), 0 );

		VAO::UnbindVAO();
		VAO::UnbindBuffer( VAO::Vertex_Buffer );
		VAO::UnbindBuffer( VAO::Index_Buffer );
	}
	else
		ArrayBuffer = IndexBuffer = VertexBuffer = 0;

	return *this;
}

//-------------------------------------------------------------------------//
