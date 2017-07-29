#include "..\..\3D\LightSphere.h"

#define PI 3.141592653

//-------------------------------------------------------------------------//

le::LightSphere::LightSphere() : fDetail( 15 ), VertexArray( 0 ), VertexBuffer( 0 )
{}

//-------------------------------------------------------------------------//

le::LightSphere::LightSphere( const LightSphere & Copy )
{
	vector<glm::vec3> Vertexs;

	iCountVertexs = Copy.iCountVertexs;
	fDetail = Copy.fDetail;
	fRadius = Copy.fRadius;
	Position = Copy.Position;
	transformationMatrix = Copy.transformationMatrix;
	vVertexs = Copy.vVertexs;
	
	if ( !vVertexs.empty() )
	{
		for ( int i = 0; i < vVertexs.size(); i++ )
			Vertexs.push_back( vVertexs[ i ] * fRadius );

		VertexArray = LoaderVAO::CreateVAO();
		LoaderVAO::BindVAO( VertexArray );

		VertexBuffer = LoaderVAO::AtachBuffer( GL_ARRAY_BUFFER, Vertexs, GL_STATIC_DRAW );

		LoaderVAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), 0 );

		LoaderVAO::UnbindVAO();
		LoaderVAO::UnbindBuffer( GL_ARRAY_BUFFER );
	}
	else
	{
		VertexArray = 0;
		VertexBuffer = 0;
	}
}

//-------------------------------------------------------------------------//

le::LightSphere::~LightSphere()
{
	if ( VertexBuffer != 0 )
		LoaderVAO::DeleteBuffer( &VertexBuffer );

	if ( VertexArray != 0 )
		LoaderVAO::DeleteVAO( &VertexArray );
}

//-------------------------------------------------------------------------//

void le::LightSphere::InitSphere( float Radius, float Detail )
{
	fDetail = Detail;
	fRadius = Radius;

	vector<glm::vec3> Vertexs;
	glm::vec3 Coord;
	float theta1, theta2, phi;

	for ( int i = 0; i < Detail / 2; i++ )
	{
		theta1 = ( i )*2.0f*PI / ( float ) Detail - PI / 2.0f;
		theta2 = ( i + 1 )*2.0f*PI / ( float ) Detail - PI / 2.0f;

		for ( int j = 0; j < Detail; j++ )
		{
			phi = j*2.0f*PI / ( float ) ( Detail - 1 );

			Coord = glm::vec3( cosf( theta2 )*cosf( phi ), sinf( theta2 ), cosf( theta2 )*sinf( phi ) );
			Vertexs.push_back( Coord * Radius );
			vVertexs.push_back( Coord );

			Coord = glm::vec3( cosf( theta1 )*cosf( phi ), sinf( theta1 ), cosf( theta1 )*sinf( phi ) );
			Vertexs.push_back( Coord * Radius );
			vVertexs.push_back( Coord );
		}
	}

	iCountVertexs = Vertexs.size();

	VertexArray = LoaderVAO::CreateVAO();
	LoaderVAO::BindVAO( VertexArray );

	VertexBuffer = LoaderVAO::AtachBuffer( GL_ARRAY_BUFFER, Vertexs, GL_STATIC_DRAW );

	LoaderVAO::SetVertexAttribPointer( VERT_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof( glm::vec3 ), 0 );

	LoaderVAO::UnbindVAO();
	LoaderVAO::UnbindBuffer( GL_ARRAY_BUFFER );
}

//-------------------------------------------------------------------------//

void le::LightSphere::RenderSphere()
{
	LoaderVAO::BindVAO( VertexArray );
	glDrawArrays( GL_QUAD_STRIP, 0, iCountVertexs );
	LoaderVAO::UnbindVAO();
}

//-------------------------------------------------------------------------//

void le::LightSphere::SetPosition( glm::vec3 Position )
{
	this->Position = Position;

	transformationMatrix = glm::translate( Position );
}

//-------------------------------------------------------------------------//

void le::LightSphere::SetRadius( float Radius )
{
	fRadius = Radius;

	vector<glm::vec3> Vertexs;

	for ( int i = 0; i < vVertexs.size(); i++ )
		Vertexs.push_back( vVertexs[ i ] * Radius );

	glBindBuffer( GL_ARRAY_BUFFER, VertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, Vertexs.size() * sizeof( glm::vec3 ), Vertexs.data(), GL_STATIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

//-------------------------------------------------------------------------//

glm::mat4& le::LightSphere::GetTransformationMatrix()
{
	return transformationMatrix;
}

//-------------------------------------------------------------------------//
