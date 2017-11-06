#include <System\System.h>
#include "..\GBuffer.h"

//-------------------------------------------------------------------------//

le::GBuffer::GBuffer() :
	IsInit( false )
{}

//-------------------------------------------------------------------------//

le::GBuffer::~GBuffer()
{
	if ( IsInit )
	{
		glDeleteFramebuffers( 1, &FrameBuffer );
		glDeleteRenderbuffers( 1, &DepthBuffer );
		glDeleteTextures( GBUFFER_NUM_TEXTURES, Buffers );
		glDeleteTextures( 1, &FinalFrame );
	}
}

//-------------------------------------------------------------------------//

bool le::GBuffer::InitGBuffer( const float& WindowWidth, const float& WindowHeight )
{
	Logger::Log( Logger::Info, "Init GBuffer (" + to_string( NUMBER_TO_INT( WindowWidth ) ) + "x" + to_string( NUMBER_TO_INT( WindowHeight ) ) + ")" );

	// ***************************************** //
	// Генерируем буфферы

	glGenFramebuffers( 1, &FrameBuffer );
	glGenRenderbuffers( 1, &DepthBuffer );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, FrameBuffer );

	glGenTextures( GBUFFER_NUM_TEXTURES, Buffers );
	glGenTextures( 1, &FinalFrame );

	// ***************************************** //
	// Присоединяем буфферы нормалей, позиций и т.д к FBO

	for ( int i = 0; i < GBUFFER_NUM_TEXTURES; i++ )
	{
		glBindTexture( GL_TEXTURE_2D, Buffers[ i ] );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB32F, NUMBER_TO_INT( WindowWidth ), NUMBER_TO_INT( WindowHeight ), 0, GL_RGB, GL_FLOAT, NULL );

		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, Buffers[ i ], 0 );
		DrawBuffers[ i ] = GL_COLOR_ATTACHMENT0 + i;
	}

	// ***************************************** //
	// Инициализируем текстуру для финального кадра

	glBindTexture( GL_TEXTURE_2D, FinalFrame );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, NUMBER_TO_INT( WindowWidth ), NUMBER_TO_INT( WindowHeight ), 0, GL_RGB, GL_FLOAT, NULL );
	glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, FinalFrame, 0 );

	// ***************************************** //
	// Инициализируем буффер глубины

	glBindRenderbuffer( GL_RENDERBUFFER, DepthBuffer );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, NUMBER_TO_INT( WindowWidth ), NUMBER_TO_INT( WindowHeight ) );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthBuffer );
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );

	// ***************************************** //
	// Разрешаем рендер в буфферы нормалей, позиций и т.д

	glDrawBuffers( GBUFFER_NUM_TEXTURES, DrawBuffers );

	// ***************************************** //
	// Проверяем статус FBO, создан ли он

	GLenum Status = glCheckFramebufferStatus( GL_FRAMEBUFFER );

	if ( Status != GL_FRAMEBUFFER_COMPLETE )
	{
		Logger::Log( Logger::Error, "GBuffer Not Initialized. Framebuffer Status [0x" + to_string( Status ) + "]" );

		glDeleteFramebuffers( 1, &FrameBuffer );
		glDeleteRenderbuffers( 1, &DepthBuffer );
		glDeleteTextures( GBUFFER_NUM_TEXTURES, Buffers );
		glDeleteTextures( 1, &FinalFrame );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		return false;
	}

	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

	SizeWindow.x = WindowWidth;
	SizeWindow.y = WindowHeight;

	Logger::Log( Logger::Info, "GBuffer Initialized" );
	IsInit = true;
	return true;
}

//-------------------------------------------------------------------------//

bool le::GBuffer::InitGBuffer( const glm::vec2& SizeWindow )
{
	return InitGBuffer( SizeWindow.x, SizeWindow.y );
}

//-------------------------------------------------------------------------//

void le::GBuffer::ClearFrame()
{
	glBindFramebuffer( GL_FRAMEBUFFER, FrameBuffer );
	glDrawBuffer( GL_COLOR_ATTACHMENT4 );
	glClear( GL_COLOR_BUFFER_BIT );
}

//-------------------------------------------------------------------------//

void le::GBuffer::RenderFrame()
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
	glBindFramebuffer( GL_READ_FRAMEBUFFER, FrameBuffer );

	glReadBuffer( GL_COLOR_ATTACHMENT4 );
	glBlitFramebuffer( 0, 0, NUMBER_TO_INT( SizeWindow.x ), NUMBER_TO_INT( SizeWindow.y ), 0, 0, NUMBER_TO_INT( SizeWindow.x ), NUMBER_TO_INT( SizeWindow.y ), GL_COLOR_BUFFER_BIT, GL_LINEAR );
}

//-------------------------------------------------------------------------//

void le::GBuffer::RenderFrame( TypeTexture TypeBufferRender )
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
	glBindFramebuffer( GL_READ_FRAMEBUFFER, FrameBuffer );

	glReadBuffer( GL_COLOR_ATTACHMENT0 + TypeBufferRender );
	glBlitFramebuffer( 0, 0, NUMBER_TO_INT( SizeWindow.x ), NUMBER_TO_INT( SizeWindow.y ), 0, 0, NUMBER_TO_INT( SizeWindow.x ), NUMBER_TO_INT( SizeWindow.y ), GL_COLOR_BUFFER_BIT, GL_LINEAR );
}

//-------------------------------------------------------------------------//

void le::GBuffer::RenderFrame( const glm::vec2& SizeWindow )
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
	glBindFramebuffer( GL_READ_FRAMEBUFFER, FrameBuffer );

	glReadBuffer( GL_COLOR_ATTACHMENT4 );
	glBlitFramebuffer( 0, 0, NUMBER_TO_INT( SizeWindow.x ), NUMBER_TO_INT( SizeWindow.y ), 0, 0, NUMBER_TO_INT( SizeWindow.x ), NUMBER_TO_INT( SizeWindow.y ), GL_COLOR_BUFFER_BIT, GL_LINEAR );
}

//-------------------------------------------------------------------------//

void le::GBuffer::Bind( TypeBind TypeBind )
{
	switch ( TypeBind )
	{
	case TypeBind::RenderBuffers:
		glDepthRange( 0, 0.8 );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, FrameBuffer );
		glDrawBuffers( GBUFFER_NUM_TEXTURES, DrawBuffers );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		break;

	case TypeBind::RenderLight:
		glDepthRange( 0, 0.8 );
		glDrawBuffer( GL_COLOR_ATTACHMENT4 );

		for ( unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++ )
		{
			glActiveTexture( GL_TEXTURE0 + i );
			glBindTexture( GL_TEXTURE_2D, Buffers[ i ] );
		}
		break;

	case TypeBind::RenderSkybox:		
		glDepthRange( 0.8, 1.0 );
		glDrawBuffer( GL_COLOR_ATTACHMENT4 );
		break;
	}
}

//-------------------------------------------------------------------------//

void le::GBuffer::ShowDebug()
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glBindFramebuffer( GL_READ_FRAMEBUFFER, FrameBuffer );

	float HalfWidth = SizeWindow.x / 2;
	float HalfHeight = SizeWindow.y / 2;

	glReadBuffer( GL_COLOR_ATTACHMENT0 );
	glBlitFramebuffer( 0, 0, NUMBER_TO_INT( SizeWindow.x ), NUMBER_TO_INT( SizeWindow.y ), 0, 0, NUMBER_TO_INT( HalfWidth ), NUMBER_TO_INT( HalfHeight ), GL_COLOR_BUFFER_BIT, GL_LINEAR );

	glReadBuffer( GL_COLOR_ATTACHMENT1 );
	glBlitFramebuffer( 0, 0, NUMBER_TO_INT( SizeWindow.x ), NUMBER_TO_INT( SizeWindow.y ), 0, NUMBER_TO_INT( HalfHeight ), NUMBER_TO_INT( HalfWidth ), NUMBER_TO_INT( SizeWindow.y ), GL_COLOR_BUFFER_BIT, GL_LINEAR );

	glReadBuffer( GL_COLOR_ATTACHMENT2 );
	glBlitFramebuffer( 0, 0, NUMBER_TO_INT( SizeWindow.x ), NUMBER_TO_INT( SizeWindow.y ), NUMBER_TO_INT( HalfWidth ), NUMBER_TO_INT( HalfHeight ), NUMBER_TO_INT( SizeWindow.x ), NUMBER_TO_INT( SizeWindow.y ), GL_COLOR_BUFFER_BIT, GL_LINEAR );
}

//-------------------------------------------------------------------------//

void le::GBuffer::ShowDebug( const glm::vec2& SizeWindow )
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glBindFramebuffer( GL_READ_FRAMEBUFFER, FrameBuffer );

	float HalfWidth = SizeWindow.x / 2;
	float HalfHeight = SizeWindow.y / 2;

	glReadBuffer( GL_COLOR_ATTACHMENT0 );
	glBlitFramebuffer( 0, 0, NUMBER_TO_INT( SizeWindow.x ), NUMBER_TO_INT( SizeWindow.y ), 0, 0, NUMBER_TO_INT( HalfWidth ), NUMBER_TO_INT( HalfHeight ), GL_COLOR_BUFFER_BIT, GL_LINEAR );

	glReadBuffer( GL_COLOR_ATTACHMENT1 );
	glBlitFramebuffer( 0, 0, NUMBER_TO_INT( SizeWindow.x ), NUMBER_TO_INT( SizeWindow.y ), 0, NUMBER_TO_INT( HalfHeight ), NUMBER_TO_INT( HalfWidth ), NUMBER_TO_INT( SizeWindow.y ), GL_COLOR_BUFFER_BIT, GL_LINEAR );

	glReadBuffer( GL_COLOR_ATTACHMENT2 );
	glBlitFramebuffer( 0, 0, NUMBER_TO_INT( SizeWindow.x ), NUMBER_TO_INT( SizeWindow.y ), NUMBER_TO_INT( HalfWidth ), NUMBER_TO_INT( HalfHeight ), NUMBER_TO_INT( SizeWindow.x ), NUMBER_TO_INT( SizeWindow.y ), GL_COLOR_BUFFER_BIT, GL_LINEAR );
}

//-------------------------------------------------------------------------//
