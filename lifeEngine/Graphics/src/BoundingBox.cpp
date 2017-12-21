#include <System\VAO.h>
#include "..\BoundingBox.h"

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
	Position = Copy.Position;
	Rotation = Copy.Rotation;
	Transformation = Copy.Transformation;
	MatrixPosition = Copy.MatrixPosition;
	MatrixRotation = Copy.MatrixRotation;

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
	if ( ArrayBuffer != 0 ) return;

	Vertexs[ 0 ] = glm::vec3( MinVertex.x, MinVertex.z, 0 );
	Vertexs[ 1 ] = glm::vec3( MaxVertex.x, MinVertex.z, 0 );
	Vertexs[ 2 ] = glm::vec3( MinVertex.x, MaxVertex.z, 0 );
	Vertexs[ 3 ] = glm::vec3( MaxVertex.x, MaxVertex.z, 0 );
	Vertexs[ 4 ] = glm::vec3( MinVertex.x, MinVertex.z, MaxVertex.y );
	Vertexs[ 5 ] = glm::vec3( MaxVertex.x, MinVertex.z, MaxVertex.y );
	Vertexs[ 6 ] = glm::vec3( MinVertex.x, MaxVertex.z, MaxVertex.y );
	Vertexs[ 7 ] = glm::vec3( MaxVertex.x, MaxVertex.z, MaxVertex.y );

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

void le::BoundingBox::InitBox( const glm::vec3& Size )
{
	if ( ArrayBuffer != 0 ) return;

	float HalfWidth = Size.x / 2;
	float HalfHeight = Size.y / 2;
	float HalfDepth = Size.z / 2;

	Vertexs[ 0 ] = glm::vec3( -HalfWidth, -HalfHeight, HalfDepth );
	Vertexs[ 1 ] = glm::vec3( HalfWidth, -HalfHeight, HalfDepth );
	Vertexs[ 2 ] = glm::vec3( -HalfWidth, HalfHeight, HalfDepth );
	Vertexs[ 3 ] = glm::vec3( HalfWidth, HalfHeight, HalfDepth );
	Vertexs[ 4 ] = glm::vec3( -HalfWidth, -HalfHeight, -HalfDepth );
	Vertexs[ 5 ] = glm::vec3( HalfWidth, -HalfHeight, -HalfDepth );
	Vertexs[ 6 ] = glm::vec3( -HalfWidth, HalfHeight, -HalfDepth );
	Vertexs[ 7 ] = glm::vec3( HalfWidth, HalfHeight, -HalfDepth );

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

void le::BoundingBox::SetPosition( const glm::vec3& Position )
{
	this->Position = Position;
	MatrixPosition = glm::translate( Position );
	Transformation = MatrixPosition * MatrixRotation;
}

//-------------------------------------------------------------------------//

void le::BoundingBox::SetRotation( const glm::vec3& Rotation )
{
	glm::vec3 Axis( sin( Rotation.x / 2 ), sin( Rotation.y / 2 ), sin( Rotation.z / 2 ) );
	glm::vec3 Rotations( cos( Rotation.x / 2 ), cos( Rotation.y / 2 ), cos( Rotation.z / 2 ) );

	glm::quat RotateX( Rotations.x, Axis.x, 0, 0 );
	glm::quat RotateY( Rotations.y, 0, Axis.y, 0 );
	glm::quat RotateZ( Rotations.z, 0, 0, Axis.z );

	this->Rotation = RotateX * RotateY * RotateZ;
	MatrixRotation = glm::mat4_cast( this->Rotation );

	Transformation = MatrixPosition * MatrixRotation;
}

//-------------------------------------------------------------------------//

void le::BoundingBox::SetRotation( const glm::quat& Rotation )
{
	this->Rotation = Rotation;
	MatrixRotation = glm::mat4_cast( Rotation );

	Transformation = MatrixPosition * MatrixRotation;
}

//-------------------------------------------------------------------------//

glm::mat4& le::BoundingBox::GetTransformation()
{
	return Transformation;
}

//-------------------------------------------------------------------------//

glm::vec3 & le::BoundingBox::GetPosition()
{
	return Position;
}

//-------------------------------------------------------------------------//

glm::quat & le::BoundingBox::GetRotation()
{
	return Rotation;
}

//-------------------------------------------------------------------------//

le::BoundingBox & le::BoundingBox::operator=( const BoundingBox& Copy )
{
	Query = Copy.Query;
	Position = Copy.Position;
	Rotation = Copy.Rotation;
	Transformation = Copy.Transformation;
	MatrixPosition = Copy.MatrixPosition;
	MatrixRotation = Copy.MatrixRotation;

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
