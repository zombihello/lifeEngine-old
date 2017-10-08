#include "..\Query.h"

//-------------------------------------------------------------------------//

le::Query::Query() :
	Type ( GL_ANY_SAMPLES_PASSED )
{
	glGenQueries( 1, &Id );
}

//-------------------------------------------------------------------------//

le::Query::Query( Query& Copy ) :
	Type ( Copy.Type )
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

le::Query & le::Query::operator=( const Query & Copy )
{
	Type = Copy.Type;
	glGenQueries( 1, &Id );

	return *this;
}

//-------------------------------------------------------------------------//
