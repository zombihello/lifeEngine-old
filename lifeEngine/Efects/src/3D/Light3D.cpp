#include "..\..\3D\Light3D.h"

//-------------------------------------------------------------------------//

le::Light3D::Light3D() : Cubemap_DepthMap( 0 ), FBO_DepthMap( 0 )
{
	Specular.w = 1;
	fIntensivity = 1;
	Color = glm::vec4( 0.6f, 0.6f, 0.6f, 1.0f );

	fRadius = 25;
	LightSphere.InitSphere( fRadius, 15 );
	LightSphere.SetPosition( Position );
	InitDepthMap();

	ShadowProjection = glm::perspective( glm::radians( 90.f ), ( float ) SHADOW_WIDTH / ( float ) SHADOW_HEIGHT, 1.f, fRadius );

	ShadowTransforms[ 0 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 1, 0, 0 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 1 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( -1, 0, 0 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 2 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 0, 1, 0 ), glm::vec3( 0, 0, 1 ) );
	ShadowTransforms[ 3 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 0, -1, 0 ), glm::vec3( 0, 0, -1 ) );
	ShadowTransforms[ 4 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 0, 0, 1 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 5 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 0, 0, -1 ), glm::vec3( 0, -1, 0 ) );
}

//-------------------------------------------------------------------------//

le::Light3D::Light3D( const Light3D & Copy )
{
	fRadius = Copy.fRadius;
	fIntensivity = Copy.fIntensivity;
	bIsInitDepthMap = Copy.bIsInitDepthMap;

	sNameLight = Copy.sNameLight;

	Position = Copy.Position;
	Specular = Copy.Specular;
	Color = Copy.Color;

	LightSphere = Copy.LightSphere;

	for ( int Face = 0; Face < 6; Face++ )
		ShadowTransforms[ Face ] = Copy.ShadowTransforms[ Face ];

	ShadowProjection = Copy.ShadowProjection;

	InitDepthMap();
}

//-------------------------------------------------------------------------//

le::Light3D::Light3D( glm::vec3 Position, float Radius, float Intensivity, glm::vec4 Color, glm::vec4 Specular ) : Cubemap_DepthMap( 0 ), FBO_DepthMap( 0 )
{
	this->Position = Position;
	fIntensivity = Intensivity;
	this->Color = Color / 255.f;
	this->Specular = Specular / 255.f;
	fRadius = Radius;

	LightSphere.InitSphere( fRadius, 15 );
	LightSphere.SetPosition( Position );
	InitDepthMap();

	ShadowProjection = glm::perspective( glm::radians( 90.f ), ( float ) SHADOW_WIDTH / ( float ) SHADOW_HEIGHT, 1.f, fRadius );

	ShadowTransforms[ 0 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 1, 0, 0 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 1 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( -1, 0, 0 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 2 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 0, 1, 0 ), glm::vec3( 0, 0, 1 ) );
	ShadowTransforms[ 3 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 0, -1, 0 ), glm::vec3( 0, 0, -1 ) );
	ShadowTransforms[ 4 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 0, 0, 1 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 5 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 0, 0, -1 ), glm::vec3( 0, -1, 0 ) );
}

//-------------------------------------------------------------------------//

le::Light3D::~Light3D()
{
	if ( bIsInitDepthMap )
	{
		glDeleteTextures( 1, &Cubemap_DepthMap );
		glDeleteFramebuffers( 1, &FBO_DepthMap );
	}
}

//-------------------------------------------------------------------------//

void le::Light3D::InitDepthMap()
{
	glGenFramebuffers( 1, &FBO_DepthMap );

	glGenTextures( 1, &Cubemap_DepthMap );
	glBindTexture( GL_TEXTURE_CUBE_MAP, Cubemap_DepthMap );

	for ( int i = 0; i < 6; i++ )
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

	glBindFramebuffer( GL_FRAMEBUFFER, FBO_DepthMap );
	glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, Cubemap_DepthMap, 0 );
	glDrawBuffer( GL_NONE );
	glReadBuffer( GL_NONE );
	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );

	GLenum Status = glCheckFramebufferStatus( GL_FRAMEBUFFER );

	if ( Status != GL_FRAMEBUFFER_COMPLETE )
	{
		cout << "Error: Framebuffer status: 0x" << Status << ". In function le::Light3D::InitDepthMap()\n";
		bIsInitDepthMap = false;
	}
	else
		bIsInitDepthMap = true;

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

//-------------------------------------------------------------------------//

void le::Light3D::SetRadius( float Radius )
{
	fRadius = Radius;
	LightSphere.SetRadius( fRadius );

	ShadowProjection = glm::perspective( glm::radians( 90.f ), ( float ) SHADOW_WIDTH / ( float ) SHADOW_HEIGHT, 1.f, fRadius );

	ShadowTransforms[ 0 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 1, 0, 0 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 1 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( -1, 0, 0 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 2 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 0, 1, 0 ), glm::vec3( 0, 0, 1 ) );
	ShadowTransforms[ 3 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 0, -1, 0 ), glm::vec3( 0, 0, -1 ) );
	ShadowTransforms[ 4 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 0, 0, 1 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 5 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 0, 0, -1 ), glm::vec3( 0, -1, 0 ) );
}

//-------------------------------------------------------------------------//

void le::Light3D::SetIntensivity( float Intensivity )
{
	fIntensivity = Intensivity;
}

//-------------------------------------------------------------------------//

void le::Light3D::SetPosition( glm::vec3 Position )
{
	this->Position = Position;
	LightSphere.SetPosition( Position );

	ShadowTransforms[ 0 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 1, 0, 0 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 1 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( -1, 0, 0 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 2 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 0, 1, 0 ), glm::vec3( 0, 0, 1 ) );
	ShadowTransforms[ 3 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 0, -1, 0 ), glm::vec3( 0, 0, -1 ) );
	ShadowTransforms[ 4 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 0, 0, 1 ), glm::vec3( 0, -1, 0 ) );
	ShadowTransforms[ 5 ] = ShadowProjection * glm::lookAt( Position, Position + glm::vec3( 0, 0, -1 ), glm::vec3( 0, -1, 0 ) );
}

//-------------------------------------------------------------------------//

void le::Light3D::SetColor( glm::vec4 Color )
{
	this->Color = Color / 255.f;
}

//-------------------------------------------------------------------------//

void le::Light3D::SetSpecular( glm::vec4 Specular )
{
	this->Specular = Specular / 255.f;
}

//-------------------------------------------------------------------------//

void le::Light3D::SetName( string NameLight )
{
	this->sNameLight = NameLight;
}

//-------------------------------------------------------------------------//