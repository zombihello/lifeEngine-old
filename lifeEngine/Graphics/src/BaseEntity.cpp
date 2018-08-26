#include "..\BaseEntity.h"

//-------------------------------------------------------------------------//

le::BaseEntity::BaseEntity( const map<string, string>& Values )
{
	this->ArrayString = Values;	
	string TempString;
	stringstream StringStream( ArrayString[ "origin" ] );
	vector<float> ArrayPosition;

	NameEntity = ArrayString[ "classname" ];

	while ( !StringStream.eof() )
	{
		StringStream >> TempString;
		ArrayPosition.push_back( NUMBER_TO_FLOAT( atof( TempString.c_str() ) ) );
		TempString.clear();
	}

	if ( ArrayPosition.size() >= 3 )
		Position = glm::vec3( ArrayPosition[ 0 ], ArrayPosition[ 2 ], -ArrayPosition[ 1 ] );
}

//-------------------------------------------------------------------------//