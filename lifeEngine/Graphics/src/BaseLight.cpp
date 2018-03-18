#include <Graphics\Scene.h>
#include "..\BaseLight.h"

//-------------------------------------------------------------------------//

le::BaseLight::BaseLight() :
	IsInitShadowMap( false ),
	IsVisible( true ),
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
		glDeleteFramebuffers( 1, &ShadowMap_FBO );
	}
}

//-------------------------------------------------------------------------//

bool le::BaseLight::InitShadowMap( bool IsCubeMap )
{
		if ( IsInitShadowMap )
			return true;

	GLsizei SizeShadowMap_Width = SHADOWMAP_SIZE;
	GLsizei SizeShadowMap_Height = SHADOWMAP_SIZE;

	if ( IsCubeMap )
	{
		SizeShadowMap_Width = 3 * SHADOWMAP_SIZE;
		SizeShadowMap_Height = 2 * SHADOWMAP_SIZE;
	}

	// ***************************************** //
	// Генерируем буфферы

	glGenFramebuffers( 1, &ShadowMap_FBO );
	glGenTextures( 1, &ShadowMap );

	// ***************************************** //
	// Инициализируем карту теней

	glBindTexture( GL_TEXTURE_2D, ShadowMap );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SizeShadowMap_Width, SizeShadowMap_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glBindFramebuffer( GL_FRAMEBUFFER, ShadowMap_FBO );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, ShadowMap, 0 );

	glDrawBuffer( GL_NONE );
	glReadBuffer( GL_NONE );

	// ***************************************** //
	// Проверяем статус FBO, создан ли он

	GLenum Status = glCheckFramebufferStatus( GL_FRAMEBUFFER );

	if ( Status != GL_FRAMEBUFFER_COMPLETE )
	{
		Logger::Log( Logger::Error, "Shadow Map Not Initialized. Framebuffer Status [0x" + to_string( Status ) + "]" );

		glDeleteTextures( 1, &ShadowMap );
		glDeleteFramebuffers( 1, &ShadowMap_FBO );
		IsInitShadowMap = false;
	}
	else
		IsInitShadowMap = true;

	glBindTexture( GL_TEXTURE_2D, 0 );
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	return IsInitShadowMap;
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

void le::BaseLight::CopyBaseLight( const BaseLight& Copy )
{
	NameLight = Copy.NameLight;
	Position = Copy.Position;
	Specular = Copy.Specular;
	Color = Copy.Color;
	Intensivity = Copy.Intensivity;
	IsVisible = Copy.IsVisible;

	Frustums = Copy.Frustums;
	LightProjection = Copy.LightProjection;
	LightViews = Copy.LightViews;
	LightTransforms = Copy.LightTransforms;

	//TODO: [zombiHello] Сделать копирование карты теней
}

//-------------------------------------------------------------------------//
