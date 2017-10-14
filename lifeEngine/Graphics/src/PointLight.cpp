#include <System\System.h>
#include "..\PointLight.h"

//-------------------------------------------------------------------------//

le::PointLight::PointLight() :
	Radius( 25 )
{
	LightSphere.InitSphere( Radius );
	LightSphere.SetPosition( Position );
	InitShadowMap( true );

	glm::vec3 TempPosition = Position;
	ShadowProjection = glm::perspective( glm::radians( 90.f ), NUMBER_TO_FLOAT( SHADOW_WIDTH / SHADOW_HEIGHT ), 1.f, Radius );

	ShadowTransforms =
	{
		ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 1, 0, 0 ), glm::vec3( 0, -1, 0 ) ),
		ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( -1, 0, 0 ), glm::vec3( 0, -1, 0 ) ),
		ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 1, 0 ), glm::vec3( 0, 0, 1 ) ),
		ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, -1, 0 ), glm::vec3( 0, 0, -1 ) ),
		ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 0, 1 ), glm::vec3( 0, -1, 0 ) ),
		ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 0, -1 ), glm::vec3( 0, -1, 0 ) )
	};
}

//-------------------------------------------------------------------------//

le::PointLight::PointLight( const PointLight& Copy )
{
	Radius = Copy.Radius;
	LightSphere = Copy.LightSphere;
}

//-------------------------------------------------------------------------//

le::PointLight::PointLight( float Radius, float Intensivity, const glm::vec3& Position, const glm::vec4& Color, const glm::vec4& Specular )
{
	this->Position = glm::vec4( Position, 1.0f );
	this->Color = Color / 255.f;
	this->Specular = Specular / 255.f;
	this->Intensivity = Intensivity;
	this->Radius = Radius;

	LightSphere.InitSphere( Radius );
	LightSphere.SetPosition( Position );
	InitShadowMap( true );

	glm::vec3 TempPosition = Position;
	ShadowProjection = glm::perspective( glm::radians( 90.f ), NUMBER_TO_FLOAT( SHADOW_WIDTH / SHADOW_HEIGHT ), 1.f, Radius );

	ShadowTransforms =
	{
		ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 1, 0, 0 ), glm::vec3( 0, -1, 0 ) ),
		ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( -1, 0, 0 ), glm::vec3( 0, -1, 0 ) ),
		ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 1, 0 ), glm::vec3( 0, 0, 1 ) ),
		ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, -1, 0 ), glm::vec3( 0, 0, -1 ) ),
		ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 0, 1 ), glm::vec3( 0, -1, 0 ) ),
		ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 0, -1 ), glm::vec3( 0, -1, 0 ) )
	};
}

//-------------------------------------------------------------------------//

le::PointLight::~PointLight()
{}

//-------------------------------------------------------------------------//

void le::PointLight::SetRadius( float Radius )
{
	this->Radius = Radius;
	LightSphere.SetRadius( Radius );

	glm::vec3 TempPosition = Position;
	ShadowProjection = glm::perspective( glm::radians( 90.f ), NUMBER_TO_FLOAT( SHADOW_WIDTH / SHADOW_HEIGHT ), 1.f, Radius );

	ShadowTransforms[ 0 ] = ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 1, 0, 0 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 1 ] = ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( -1, 0, 0 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 2 ] = ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 1, 0 ), glm::vec3( 0, 0, 1 ) );
	ShadowTransforms[ 3 ] = ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, -1, 0 ), glm::vec3( 0, 0, -1 ) );
	ShadowTransforms[ 4 ] = ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 0, 1 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 5 ] = ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 0, -1 ), glm::vec3( 0, -1, 0 ) );
}

//-------------------------------------------------------------------------//

void le::PointLight::SetPosition( const glm::vec4& Position )
{
	LightSphere.SetPosition( Position );
	glm::vec3 TempPosition = Position;

	ShadowTransforms[ 0 ] = ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 1, 0, 0 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 1 ] = ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( -1, 0, 0 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 2 ] = ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 1, 0 ), glm::vec3( 0, 0, 1 ) );
	ShadowTransforms[ 3 ] = ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, -1, 0 ), glm::vec3( 0, 0, -1 ) );
	ShadowTransforms[ 4 ] = ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 0, 1 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 5 ] = ShadowProjection * glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 0, -1 ), glm::vec3( 0, -1, 0 ) );
}

//-------------------------------------------------------------------------//
