#include "..\..\3D\GBuffer.h"

//-------------------------------------------------------------------------//

le::GBuffer::GBuffer() : IsGBufferInit( false )
{}

//-------------------------------------------------------------------------//

le::GBuffer::~GBuffer()
{
	if ( IsGBufferInit )
	{
		glDeleteFramebuffers( 1, &FBO );
		glDeleteTextures( GBUFFER_NUM_TEXTURES, Buffers );
		glDeleteTextures( 1, &DepthTexture );
	}
}

//-------------------------------------------------------------------------//

bool le::GBuffer::InitGBuffer( float WindowWidth, float WindowHeight )
{
	glGenFramebuffers( 1, &FBO );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, FBO );

	glGenTextures( GBUFFER_NUM_TEXTURES, Buffers );
	glGenTextures( 1, &DepthTexture );

	for ( int i = 0; i < GBUFFER_NUM_TEXTURES; i++ )
	{
		glBindTexture( GL_TEXTURE_2D, Buffers[ i ] );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL );
	
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, Buffers[i], 0 );
	}

	glBindTexture( GL_TEXTURE_2D, DepthTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL );
	glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthTexture, 0 );

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

	glDrawBuffers( 3, DrawBuffers );

	GLenum Status = glCheckFramebufferStatus( GL_FRAMEBUFFER );

	if ( Status != GL_FRAMEBUFFER_COMPLETE )
	{
		cout << "Error: Framebuffer status: 0x" << Status << ". The function le::GBuffer::InitGBuffer() returned false\n";
		return false;
	}

	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

	IsGBufferInit = true;
	return true;
}

//-------------------------------------------------------------------------//

void le::GBuffer::BindForWriting()
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, FBO );
}

//-------------------------------------------------------------------------//

void le::GBuffer::BindForReading()
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

	for ( unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++ )
	{
		glActiveTexture( GL_TEXTURE0 + i );
		glBindTexture( GL_TEXTURE_2D, Buffers[ i ] );
	}
}

//-------------------------------------------------------------------------//
