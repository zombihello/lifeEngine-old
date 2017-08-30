#include "..\VAO.h"

//-------------------------------------------------------------------------//

GLuint le::VAO::CreateVAO()
{
	GLuint VAO;
	glGenVertexArrays( 1, &VAO );
	return VAO;
}

//-------------------------------------------------------------------------//

void le::VAO::AtachBuffer( TypeBuffer TypeBuffer, GLuint Buffer )
{
	glBindBuffer( TypeBuffer, Buffer );
}

//-------------------------------------------------------------------------//

void le::VAO::SetVertexAttribPointer( GLuint Index, GLint Size, GLenum Type, GLboolean Normalized, GLsizei Stride, const void* Pointer )
{
	glVertexAttribPointer( Index, Size, Type, Normalized, Stride, Pointer );
	glEnableVertexAttribArray( Index );
}

//-------------------------------------------------------------------------//

void le::VAO::EnableVertexPointer( GLuint Index )
{
	glEnableVertexAttribArray( Index );
}

//-------------------------------------------------------------------------//

void le::VAO::DisableVertexPointer( GLuint Index )
{
	glDisableVertexAttribArray( Index );
}

//-------------------------------------------------------------------------//

void le::VAO::BindBuffer( TypeBuffer TypeBuffer, GLuint Buffer )
{
	glBindBuffer( TypeBuffer, Buffer );
}

//-------------------------------------------------------------------------//

void le::VAO::BindVAO( GLuint VAO )
{
	glBindVertexArray( VAO );
}

//-------------------------------------------------------------------------//

void le::VAO::UnbindVAO()
{
	glBindVertexArray( 0 );
}

//-------------------------------------------------------------------------//

void le::VAO::UnbindBuffer( TypeBuffer TypeBuffer )
{
	glBindBuffer( TypeBuffer, 0 );
}

//-------------------------------------------------------------------------//

void le::VAO::DeleteBuffer( GLuint* Buffer )
{
	glDeleteBuffers( 1, Buffer );
}

//-------------------------------------------------------------------------//

void le::VAO::DeleteVAO( GLuint* VAO )
{
	glDeleteVertexArrays( 1, VAO );
}

//-------------------------------------------------------------------------//

