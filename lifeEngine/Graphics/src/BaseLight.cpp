#include <Graphics\Scene.h>
#include "..\BaseLight.h"

//-------------------------------------------------------------------------//

le::BaseLight::BaseLight() :
	IsInitShadowMap( false ),
	IsStaticLight( true ),
	Intensivity( 1 ),
	Color( 0.6f, 0.6f, 0.6f, 1 ),
	Specular( 0, 0, 0, 1 )
{}

//-------------------------------------------------------------------------//

le::BaseLight::~BaseLight()
{
	if ( IsInitShadowMap )
	{
		glDeleteTextures( 1, &ShadowMap );
		glDeleteFramebuffers( 1, &FBO_ShadowMap );
	}
}

//-------------------------------------------------------------------------//

void le::BaseLight::InitShadowMap( bool IsCupeMap )
{
	// ***************************************** //
	// Генерируем буфферы

	glGenFramebuffers( 1, &FBO_ShadowMap );
	glGenTextures( 1, &ShadowMap );

	// ***************************************** //
	// Инициализируем карту теней

	if ( !IsCupeMap )
	{
		glBindTexture( GL_TEXTURE_2D, ShadowMap );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	}
	else
	{
		glBindTexture( GL_TEXTURE_CUBE_MAP, ShadowMap );

		for ( int i = 0; i < 6; i++ )
			glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );

		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	}

	glBindFramebuffer( GL_FRAMEBUFFER, FBO_ShadowMap );
	glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, ShadowMap, 0 );
	glDrawBuffer( GL_NONE );
	glReadBuffer( GL_NONE );

	if ( !IsCupeMap )
		glBindTexture( GL_TEXTURE_2D, 0 );
	else
		glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );

	// ***************************************** //
	// Проверяем статус FBO, создан ли он

	GLenum Status = glCheckFramebufferStatus( GL_FRAMEBUFFER );

	if ( Status != GL_FRAMEBUFFER_COMPLETE )
	{
		Logger::Log( Logger::Error, "Shadow Map Not Initialized. Framebuffer Status [0x" + to_string( Status ) + "]" );

		glDeleteTextures( 1, &ShadowMap );
		glDeleteFramebuffers( 1, &FBO_ShadowMap );
		IsInitShadowMap = false;
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );
		return;
	}
	else
		IsInitShadowMap = true;

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

//-------------------------------------------------------------------------//

void le::BaseLight::SetName( const string& NameLight )
{
	this->NameLight = NameLight;
}

//-------------------------------------------------------------------------//

void le::BaseLight::SetSpecular( const glm::vec4& Specular )
{
	this->Specular = Specular / 255.f;
}

//-------------------------------------------------------------------------//

void le::BaseLight::SetColor( const glm::vec4& Color )
{
	this->Color = Color / 255.f;
}

//-------------------------------------------------------------------------//

void le::BaseLight::SetIntensivity( float Intensivity )
{
	this->Intensivity = Intensivity;
}

//-------------------------------------------------------------------------//

void le::BaseLight::SetDinamicLight( bool IsDinamic )
{
	IsStaticLight = !IsDinamic;
}

//-------------------------------------------------------------------------//

void le::BaseLight::CopyBaseLight( const BaseLight& Copy )
{
	IsStaticLight = Copy.IsStaticLight;
	IsInitShadowMap = Copy.IsInitShadowMap;
	NameLight = Copy.NameLight;
	Position = Copy.Position;
	Specular = Copy.Specular;
	Color = Copy.Color;
	Intensivity = Copy.Intensivity;

	ShadowProjection = Copy.ShadowProjection;
	ShadowTransforms = Copy.ShadowTransforms;

	if ( IsInitShadowMap )
		InitShadowMap( true );
}

//-------------------------------------------------------------------------//
