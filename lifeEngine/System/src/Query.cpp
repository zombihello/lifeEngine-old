#include <System\System.h>
#include "..\Query.h"

//-------------------------------------------------------------------------//

le::Query::Query() :
	Type( GL_ANY_SAMPLES_PASSED )
{
	glGenQueries( 1, &Id );
}

//-------------------------------------------------------------------------//

le::Query::Query( Query& Copy ) :
	Type( Copy.Type )
{
	glGenQueries( 1, &Id );
}

//-------------------------------------------------------------------------//

le::Query::Query( GLenum Type ) :
	Type( Type )
{
	glGenQueries( 1, &Id );
}

//-------------------------------------------------------------------------//

le::Query::~Query()
{
	glDeleteQueries( 1, &Id );
}

//-------------------------------------------------------------------------//

inline void le::Query::Start()
{
	InUse = true;
	glBeginQuery( Type, Id );
}

//-------------------------------------------------------------------------//

inline void le::Query::StartConditionalRender( GLenum Mode )
{
	glBeginConditionalRender( Id, Mode );
}

//-------------------------------------------------------------------------//

inline void le::Query::EndConditionalRender()
{
	glEndConditionalRender();
}

//-------------------------------------------------------------------------//

inline void le::Query::End()
{
	glEndQuery( Type );
}

//-------------------------------------------------------------------------//

inline bool le::Query::IsInUse()
{
	return InUse;
}

//-------------------------------------------------------------------------//

inline bool le::Query::IsResultReady()
{
	GLint IsReady;
	glGetQueryObjectiv( Id, GL_QUERY_RESULT_AVAILABLE, &IsReady );

	return NUMBER_TO_BOOL( IsReady );
}

//-------------------------------------------------------------------------//

inline int le::Query::GetResult()
{
	InUse = false;

	GLint Result;
	glGetQueryObjectiv( Id, GL_QUERY_RESULT, &Result );

	return Result;
}

//-------------------------------------------------------------------------//

le::Query & le::Query::operator=( const Query & Copy )
{
	Type = Copy.Type;
	glGenQueries( 1, &Id );

	return *this;
}

//-------------------------------------------------------------------------//
