#include "..\LoaderVAO.h"

//-------------------------------------------------------------------------//

GLuint le::LoaderVAO::CreateVAO( )
{
	GLuint VAO;
	glGenVertexArrays( 1, &VAO );
	return VAO;
}

//-------------------------------------------------------------------------//

void le::LoaderVAO::SetVertexAttribPointer( GLuint Index, GLint Size, GLenum Type, GLboolean Normalized, GLsizei Stride, const void* Pointer )
{
	glVertexAttribPointer( Index, Size, Type, Normalized, Stride, Pointer );
	
}

//-------------------------------------------------------------------------//

void le::LoaderVAO::EnableVertexPointer( GLuint Index )
{
	glEnableVertexAttribArray( Index );
}

//-------------------------------------------------------------------------//

void le::LoaderVAO::DisableVertexPointer( GLuint Index )
{
	glDisableVertexAttribArray( Index );
}

//-------------------------------------------------------------------------//

void le::LoaderVAO::BindVAO( GLuint VAO )
{
	glBindVertexArray( VAO );
}

//-------------------------------------------------------------------------//

void le::LoaderVAO::UnbindVAO( )
{
	glBindVertexArray( 0 );
}

//-------------------------------------------------------------------------//

void le::LoaderVAO::UnbindBuffer( GLenum TypeBuffer )
{
	glBindBuffer( TypeBuffer, 0 );
}

//-------------------------------------------------------------------------//

void le::LoaderVAO::DeleteBuffer( GLuint* Buffer )
{
	glDeleteBuffers( 1, Buffer );
}

//-------------------------------------------------------------------------//

void le::LoaderVAO::DeleteVAO( GLuint* VAO )
{
	glDeleteVertexArrays( 1, VAO );
}

//-------------------------------------------------------------------------//

void le::LoaderVAO::AtachBuffer( GLenum TypeBuffer, GLuint Buffer )
{
	glBindBuffer( TypeBuffer, Buffer );
}

//-------------------------------------------------------------------------//