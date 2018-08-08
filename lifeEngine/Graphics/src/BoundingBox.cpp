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
	Query = Copy.Query;
	Position = Copy.Position;
	Rotation = Copy.Rotation;
	Scale = Copy.Scale;
	Transformation = Copy.Transformation;
	MatrixPosition = Copy.MatrixPosition;
	MatrixRotation = Copy.MatrixRotation;
	MatrixScale = Copy.MatrixScale;
	MaxVertex = Copy.MaxVertex;
	MinVertex = Copy.MinVertex;

	for ( int i = 0; i < 8; i++ )
	{
		LocalVertexs[ i ] = Copy.LocalVertexs[ i ];
		GlobalVertexs[ i ] = Copy.GlobalVertexs[ i ];
	}

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

		VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, 8, LocalVertexs, VAO::Static_Draw );
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

	GlobalVertexs[ 0 ] = LocalVertexs[ 0 ] = glm::vec3( MinVertex.x, MinVertex.z, 0 );
	GlobalVertexs[ 1 ] = LocalVertexs[ 1 ] = glm::vec3( MaxVertex.x, MinVertex.z, 0 );
	GlobalVertexs[ 2 ] = LocalVertexs[ 2 ] = glm::vec3( MinVertex.x, MaxVertex.z, 0 );
	GlobalVertexs[ 3 ] = LocalVertexs[ 3 ] = glm::vec3( MaxVertex.x, MaxVertex.z, 0 );
	GlobalVertexs[ 4 ] = LocalVertexs[ 4 ] = glm::vec3( MinVertex.x, MinVertex.z, MaxVertex.y );
	GlobalVertexs[ 5 ] = LocalVertexs[ 5 ] = glm::vec3( MaxVertex.x, MinVertex.z, MaxVertex.y );
	GlobalVertexs[ 6 ] = LocalVertexs[ 6 ] = glm::vec3( MinVertex.x, MaxVertex.z, MaxVertex.y );
	GlobalVertexs[ 7 ] = LocalVertexs[ 7 ] = glm::vec3( MaxVertex.x, MaxVertex.z, MaxVertex.y );

	for ( int i = 0; i < 8; i++ )
	{
		if ( this->MaxVertex.x < GlobalVertexs[ i ].x )
			this->MaxVertex.x = GlobalVertexs[ i ].x;
		else
			if ( this->MinVertex.x > GlobalVertexs[ i ].x )
				this->MinVertex.x = GlobalVertexs[ i ].x;

		if ( this->MaxVertex.y < GlobalVertexs[ i ].y )
			this->MaxVertex.y = GlobalVertexs[ i ].y;
		else
			if ( this->MinVertex.y > GlobalVertexs[ i ].y )
				this->MinVertex.y = GlobalVertexs[ i ].y;

		if ( this->MaxVertex.z < GlobalVertexs[ i ].z )
			this->MaxVertex.z = GlobalVertexs[ i ].z;
		else
			if ( this->MinVertex.z > GlobalVertexs[ i ].z )
				this->MinVertex.z = GlobalVertexs[ i ].z;
	}

	ArrayBuffer = VAO::CreateVAO();
	VAO::BindVAO( ArrayBuffer );

	unsigned int IdVertexs[ 36 ] =
	{
		7, 3, 4, 3, 0, 4, 2, 6, 1,
		6, 5, 1, 7, 6, 3, 6, 2, 3,
		0, 1, 4, 1, 5, 4, 6, 4, 5,
		6, 7, 4, 0, 2, 1, 0, 3, 2
	};

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, 8, LocalVertexs, VAO::Static_Draw );
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
		GlobalVertexs[ i ] = LocalVertexs[ i ] = Vertexs[ i ];

	for ( int i = 0; i < 8; i++ )
	{
		if ( MaxVertex.x < GlobalVertexs[ i ].x )
			MaxVertex.x = GlobalVertexs[ i ].x;
		else
			if ( MinVertex.x > GlobalVertexs[ i ].x )
				MinVertex.x = GlobalVertexs[ i ].x;

		if ( MaxVertex.y < GlobalVertexs[ i ].y )
			MaxVertex.y = GlobalVertexs[ i ].y;
		else
			if ( MinVertex.y > GlobalVertexs[ i ].y )
				MinVertex.y = GlobalVertexs[ i ].y;

		if ( MaxVertex.z < GlobalVertexs[ i ].z )
			MaxVertex.z = GlobalVertexs[ i ].z;
		else
			if ( MinVertex.z > GlobalVertexs[ i ].z )
				MinVertex.z = GlobalVertexs[ i ].z;
	}

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

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, 8, LocalVertexs, VAO::Static_Draw );
	IndexBuffer = VAO::CreateBuffer( VAO::Index_Buffer, 36, IdVertexs, VAO::Static_Draw );

	VAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), 0 );

	VAO::UnbindVAO();
	VAO::UnbindBuffer( VAO::Vertex_Buffer );
	VAO::UnbindBuffer( VAO::Index_Buffer );
}

//-------------------------------------------------------------------------//

glm::vec3* le::BoundingBox::GetVertexs()
{
	return &GlobalVertexs[ 0 ];
}

//-------------------------------------------------------------------------//

glm::vec3 & le::BoundingBox::GetMaxVertex()
{
	return MaxVertex;
}

//-------------------------------------------------------------------------//

void le::BoundingBox::InitBox( const glm::vec3& Size )
{
	if ( ArrayBuffer != 0 ) return;

	float HalfWidth = Size.x / 2;
	float HalfHeight = Size.y / 2;
	float HalfDepth = Size.z / 2;

	GlobalVertexs[ 0 ] = LocalVertexs[ 0 ] = glm::vec3( -HalfWidth, -HalfHeight, HalfDepth );
	GlobalVertexs[ 1 ] = LocalVertexs[ 1 ] = glm::vec3( HalfWidth, -HalfHeight, HalfDepth );
	GlobalVertexs[ 2 ] = LocalVertexs[ 2 ] = glm::vec3( -HalfWidth, HalfHeight, HalfDepth );
	GlobalVertexs[ 3 ] = LocalVertexs[ 3 ] = glm::vec3( HalfWidth, HalfHeight, HalfDepth );
	GlobalVertexs[ 4 ] = LocalVertexs[ 4 ] = glm::vec3( -HalfWidth, -HalfHeight, -HalfDepth );
	GlobalVertexs[ 5 ] = LocalVertexs[ 5 ] = glm::vec3( HalfWidth, -HalfHeight, -HalfDepth );
	GlobalVertexs[ 6 ] = LocalVertexs[ 6 ] = glm::vec3( -HalfWidth, HalfHeight, -HalfDepth );
	GlobalVertexs[ 7 ] = LocalVertexs[ 7 ] = glm::vec3( HalfWidth, HalfHeight, -HalfDepth );

	for ( int i = 0; i < 8; i++ )
	{
		if ( MaxVertex.x < GlobalVertexs[ i ].x )
			MaxVertex.x = GlobalVertexs[ i ].x;
		else
			if ( MinVertex.x > GlobalVertexs[ i ].x )
				MinVertex.x = GlobalVertexs[ i ].x;

		if ( MaxVertex.y < GlobalVertexs[ i ].y )
			MaxVertex.y = GlobalVertexs[ i ].y;
		else
			if ( MinVertex.y > GlobalVertexs[ i ].y )
				MinVertex.y = GlobalVertexs[ i ].y;

		if ( MaxVertex.z < GlobalVertexs[ i ].z )
			MaxVertex.z = GlobalVertexs[ i ].z;
		else
			if ( MinVertex.z > GlobalVertexs[ i ].z )
				MinVertex.z = GlobalVertexs[ i ].z;
	}

	ArrayBuffer = VAO::CreateVAO();
	VAO::BindVAO( ArrayBuffer );

	unsigned int IdVertexs[ 36 ] =
	{
		7, 3, 4, 3, 0, 4, 2, 6, 1,
		6, 5, 1, 7, 6, 3, 6, 2, 3,
		0, 1, 4, 1, 5, 4, 6, 4, 5,
		6, 7, 4, 0, 2, 1, 0, 3, 2
	};

	VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, 8, LocalVertexs, VAO::Static_Draw );
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
	Transformation = MatrixPosition * MatrixRotation * MatrixScale;

	for ( int i = 0; i < 8; i++ )
	{
		GlobalVertexs[ i ] = Transformation * glm::vec4( LocalVertexs[ i ], 1.f );

		if ( MaxVertex.x < GlobalVertexs[ i ].x )
			MaxVertex.x = GlobalVertexs[ i ].x;
		else
			if ( MinVertex.x > GlobalVertexs[ i ].x )
				MinVertex.x = GlobalVertexs[ i ].x;

		if ( MaxVertex.y < GlobalVertexs[ i ].y )
			MaxVertex.y = GlobalVertexs[ i ].y;
		else
			if ( MinVertex.y > GlobalVertexs[ i ].y )
				MinVertex.y = GlobalVertexs[ i ].y;

		if ( MaxVertex.z < GlobalVertexs[ i ].z )
			MaxVertex.z = GlobalVertexs[ i ].z;
		else
			if ( MinVertex.z > GlobalVertexs[ i ].z )
				MinVertex.z = GlobalVertexs[ i ].z;
	}
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

	Transformation = MatrixPosition * MatrixRotation * MatrixScale;

	for ( int i = 0; i < 8; i++ )
	{
		GlobalVertexs[ i ] = Transformation * glm::vec4( LocalVertexs[ i ], 1.f );

		if ( MaxVertex.x < GlobalVertexs[ i ].x )
			MaxVertex.x = GlobalVertexs[ i ].x;
		else
			if ( MinVertex.x > GlobalVertexs[ i ].x )
				MinVertex.x = GlobalVertexs[ i ].x;

		if ( MaxVertex.y < GlobalVertexs[ i ].y )
			MaxVertex.y = GlobalVertexs[ i ].y;
		else
			if ( MinVertex.y > GlobalVertexs[ i ].y )
				MinVertex.y = GlobalVertexs[ i ].y;

		if ( MaxVertex.z < GlobalVertexs[ i ].z )
			MaxVertex.z = GlobalVertexs[ i ].z;
		else
			if ( MinVertex.z > GlobalVertexs[ i ].z )
				MinVertex.z = GlobalVertexs[ i ].z;
	}
}

//-------------------------------------------------------------------------//

void le::BoundingBox::SetRotation( const glm::quat& Rotation )
{
	this->Rotation = Rotation;
	MatrixRotation = glm::mat4_cast( Rotation );

	Transformation = MatrixPosition * MatrixRotation * MatrixScale;

	for ( int i = 0; i < 8; i++ )
	{
		GlobalVertexs[ i ] = Transformation * glm::vec4( LocalVertexs[ i ], 1.f );

		if ( MaxVertex.x < GlobalVertexs[ i ].x )
			MaxVertex.x = GlobalVertexs[ i ].x;
		else
			if ( MinVertex.x > GlobalVertexs[ i ].x )
				MinVertex.x = GlobalVertexs[ i ].x;

		if ( MaxVertex.y < GlobalVertexs[ i ].y )
			MaxVertex.y = GlobalVertexs[ i ].y;
		else
			if ( MinVertex.y > GlobalVertexs[ i ].y )
				MinVertex.y = GlobalVertexs[ i ].y;

		if ( MaxVertex.z < GlobalVertexs[ i ].z )
			MaxVertex.z = GlobalVertexs[ i ].z;
		else
			if ( MinVertex.z > GlobalVertexs[ i ].z )
				MinVertex.z = GlobalVertexs[ i ].z;
	}
}

//-------------------------------------------------------------------------//

void le::BoundingBox::SetScale( const glm::vec3& Scale )
{
	this->Scale = Scale;
	MatrixScale = glm::scale( Scale );
	Transformation = MatrixPosition * MatrixRotation * MatrixScale;

	for ( int i = 0; i < 8; i++ )
	{
		GlobalVertexs[ i ] = Transformation * glm::vec4( LocalVertexs[ i ], 1.f );

		if ( MaxVertex.x < GlobalVertexs[ i ].x )
			MaxVertex.x = GlobalVertexs[ i ].x;
		else
			if ( MinVertex.x > GlobalVertexs[ i ].x )
				MinVertex.x = GlobalVertexs[ i ].x;

		if ( MaxVertex.y < GlobalVertexs[ i ].y )
			MaxVertex.y = GlobalVertexs[ i ].y;
		else
			if ( MinVertex.y > GlobalVertexs[ i ].y )
				MinVertex.y = GlobalVertexs[ i ].y;

		if ( MaxVertex.z < GlobalVertexs[ i ].z )
			MaxVertex.z = GlobalVertexs[ i ].z;
		else
			if ( MinVertex.z > GlobalVertexs[ i ].z )
				MinVertex.z = GlobalVertexs[ i ].z;
	}
}

//-------------------------------------------------------------------------//

void le::BoundingBox::SetTransformation( const glm::mat4& Transformation, const glm::vec3& Position, const glm::quat& Rotation, const glm::vec3& Scale )
{
	this->Transformation = Transformation;
	this->Position = Position;
	this->Rotation = Rotation;
	this->Scale = Scale;

	for ( int i = 0; i < 8; i++ )
	{
		GlobalVertexs[ i ] = Transformation * glm::vec4( LocalVertexs[ i ], 1.f );

		if ( MaxVertex.x < GlobalVertexs[ i ].x )
			MaxVertex.x = GlobalVertexs[ i ].x;
		else
			if ( MinVertex.x > GlobalVertexs[ i ].x )
				MinVertex.x = GlobalVertexs[ i ].x;

		if ( MaxVertex.y < GlobalVertexs[ i ].y )
			MaxVertex.y = GlobalVertexs[ i ].y;
		else
			if ( MinVertex.y > GlobalVertexs[ i ].y )
				MinVertex.y = GlobalVertexs[ i ].y;

		if ( MaxVertex.z < GlobalVertexs[ i ].z )
			MaxVertex.z = GlobalVertexs[ i ].z;
		else
			if ( MinVertex.z > GlobalVertexs[ i ].z )
				MinVertex.z = GlobalVertexs[ i ].z;
	}
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

glm::vec3 & le::BoundingBox::GetMinVertex()
{
	return MinVertex;
}

//-------------------------------------------------------------------------//

le::BoundingBox & le::BoundingBox::operator=( const BoundingBox& Copy )
{
	Query = Copy.Query;
	Position = Copy.Position;
	Rotation = Copy.Rotation;
	Scale = Copy.Scale;
	Transformation = Copy.Transformation;
	MatrixPosition = Copy.MatrixPosition;
	MatrixRotation = Copy.MatrixRotation;
	MatrixScale = Copy.MatrixScale;
	MaxVertex = Copy.MaxVertex;
	MinVertex = Copy.MinVertex;

	for ( int i = 0; i < 8; i++ )
	{
		LocalVertexs[ i ] = Copy.LocalVertexs[ i ];
		GlobalVertexs[ i ] = Copy.GlobalVertexs[ i ];
	}

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

		VertexBuffer = VAO::CreateBuffer( VAO::Vertex_Buffer, 8, LocalVertexs, VAO::Static_Draw );
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
