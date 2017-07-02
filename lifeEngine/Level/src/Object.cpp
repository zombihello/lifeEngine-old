#include "../Object.h"

//-------------------------------------------------------------------------//

le::Object::Object( le::Physic2D& Physic )
{
	this->Physic = &Physic;
	BodyObject = NULL;
	Rotation = 0;
}

//-------------------------------------------------------------------------//

le::Object::~Object()
{
	if ( BodyObject != NULL )
	{
		Physic->DestroyBody( BodyObject );
		delete BodyObject;
		BodyObject = NULL;
	}
}

//-------------------------------------------------------------------------//

int le::Object::GetPropertyInt( const string sName )
{
	return atoi( mProperties[sName].c_str() );
}

//-------------------------------------------------------------------------//

float le::Object::GetPropertyFloat( const string sName )
{
	return strtod( mProperties[sName].c_str(), NULL );
}

//-------------------------------------------------------------------------//

string &le::Object::GetPropertyString( const string sName )
{
	return mProperties[sName];
}

//-------------------------------------------------------------------------//
