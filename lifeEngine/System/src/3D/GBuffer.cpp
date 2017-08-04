#include "..\..\3D\GBuffer.h"

//-------------------------------------------------------------------------//

le::GBuffer::GBuffer() : IsGBufferInit( false )
{}

//-------------------------------------------------------------------------//

le::GBuffer::~GBuffer()
{
	if ( IsGBufferInit )
	{
		glDeleteFramebuffers( 1, &FrameBuffer );
		glDeleteRenderbuffers( 1, &DepthBuffer );
		glDeleteTextures( GBUFFER_NUM_TEXTURES, Buffers );
		glDeleteTextures( 1, &FinalFrame );
	}
}

//-------------------------------------------------------------------------//

bool le::GBuffer::InitGBuffer( float WindowWidth, float WindowHeight )
{
	glGenFramebuffers( 1, &FrameBuffer );
	glGenRenderbuffers( 1, &DepthBuffer );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, FrameBuffer );

	glGenTextures( GBUFFER_NUM_TEXTURES, Buffers );
	glGenTextures( 1, &FinalFrame );

	for ( int i = 0; i < GBUFFER_NUM_TEXTURES; i++ )
	{
		glBindTexture( GL_TEXTURE_2D, Buffers[ i ] );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL );
	
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, Buffers[i], 0 );
	}

	glBindTexture( GL_TEXTURE_2D, FinalFrame );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL );
	glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, FinalFrame, 0 );

	glBindRenderbuffer( GL_RENDERBUFFER, DepthBuffer );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, WindowWidth, WindowHeight );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthBuffer );
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );

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

void le::GBuffer::ClearFrame()
{
	glBindFramebuffer( GL_FRAMEBUFFER, FrameBuffer );
	glDrawBuffer( GL_COLOR_ATTACHMENT4 );
	glClear( GL_COLOR_BUFFER_BIT );
}

//-------------------------------------------------------------------------//

void le::GBuffer::RenderFrame( Vector2u WindowSize )
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
	glBindFramebuffer( GL_READ_FRAMEBUFFER, FrameBuffer );

	glReadBuffer( GL_COLOR_ATTACHMENT4 );
	glBlitFramebuffer( 0, 0, WindowSize.x, WindowSize.y, 0, 0, WindowSize.x, WindowSize.y, GL_COLOR_BUFFER_BIT, GL_LINEAR );
}

//-------------------------------------------------------------------------//

void le::GBuffer::BindForRenderBuffers()
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, FrameBuffer );

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers( 3, DrawBuffers );
}

//-------------------------------------------------------------------------//

void le::GBuffer::BindForStencilTest()
{
	glDrawBuffer( GL_NONE );
}

//-------------------------------------------------------------------------//

void le::GBuffer::BindForRenderLight()
{
	glDrawBuffer( GL_COLOR_ATTACHMENT4 );

	for ( unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++ )
	{
		glActiveTexture( GL_TEXTURE0 + i );
		glBindTexture( GL_TEXTURE_2D, Buffers[ i ] );
	}
}

//-------------------------------------------------------------------------//

void le::GBuffer::ShowDebug( Vector2u SizeWindow )
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glBindFramebuffer( GL_READ_FRAMEBUFFER, FrameBuffer );

	GLsizei HalfWidth = ( GLsizei ) ( SizeWindow.x / 2.0f );
	GLsizei HalfHeight = ( GLsizei ) ( SizeWindow.y / 2.0f );

	glReadBuffer( GL_COLOR_ATTACHMENT0 );
	glBlitFramebuffer( 0, 0, SizeWindow.x, SizeWindow.y, 0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR );

	glReadBuffer( GL_COLOR_ATTACHMENT1 );
	glBlitFramebuffer( 0, 0, SizeWindow.x, SizeWindow.y, 0, HalfHeight, HalfWidth, SizeWindow.y, GL_COLOR_BUFFER_BIT, GL_LINEAR );

	glReadBuffer( GL_COLOR_ATTACHMENT2 );
	glBlitFramebuffer( 0, 0, SizeWindow.x, SizeWindow.y, HalfWidth, HalfHeight, SizeWindow.x, SizeWindow.y, GL_COLOR_BUFFER_BIT, GL_LINEAR );
}

//-------------------------------------------------------------------------//