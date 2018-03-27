#include "..\Entity.h"

//-------------------------------------------------------------------------//

le::Entity::Entity( TiXmlElement& ElementEntity )
{
	// ***************************************** //
	// Загружаем название энтити

	NameEntity = ElementEntity.Attribute( "Name" );

	// ***************************************** //
	// Загружаем позицию энтити в мире

	TiXmlElement* position;
	position = ElementEntity.FirstChildElement( "Position" );

	Position.x = NUMBER_TO_FLOAT( atof( position->Attribute( "X" ) ) );
	Position.y = NUMBER_TO_FLOAT( atof( position->Attribute( "Y" ) ) );
	Position.z = NUMBER_TO_FLOAT( atof( position->Attribute( "Z" ) ) );

	// ***************************************** //
	// Загружаем значения у энтити

	TiXmlElement* Value;
	Value = ElementEntity.FirstChildElement( "Value" );

	while ( Value )
	{
		Values[ Value->Attribute( "Name" ) ] = Value->Attribute( "Value" );
		Value = Value->NextSiblingElement();
	}
}

//-------------------------------------------------------------------------//

string& le::Entity::GetNameEntity()
{
	return NameEntity;
}

//-------------------------------------------------------------------------//

string le::Entity::GetValueString( const string& NameValue )
{
	if ( Values.find( NameValue ) != Values.end() )
		return Values[ NameValue ];

	return "";
}

//-------------------------------------------------------------------------//

int le::Entity::GetValueInt( const string& NameValue )
{
	if ( Values.find( NameValue ) != Values.end() )
		return atoi( Values[ NameValue ].c_str() );

	return -1;
}

//-------------------------------------------------------------------------//

float le::Entity::GetValueFloat( const string& NameValue )
{
	if ( Values.find( NameValue ) != Values.end() )
		return NUMBER_TO_FLOAT( atof( Values[ NameValue ].c_str() ) );

	return -1.f;
}

//-------------------------------------------------------------------------//

vector<string> le::Entity::GetVelueVectorString( const string& NameValue )
{
	vector<string> Vector;

	if ( Values.find( NameValue ) != Values.end() )
	{
		string TempString;
		stringstream StringStream( Values[ NameValue ] );

		while ( !StringStream.eof() )
		{
			StringStream >> TempString;
			Vector.push_back( TempString );
			TempString.clear();
		}
	}

	return Vector;
}

//-------------------------------------------------------------------------//

vector<int> le::Entity::GetVelueVectorInt( const string& NameValue )
{
	vector<int> Vector;

	if ( Values.find( NameValue ) != Values.end() )
	{
		string TempString;
		stringstream StringStream( Values[ NameValue ] );

		while ( !StringStream.eof() )
		{
			StringStream >> TempString;
			Vector.push_back( atoi( TempString.c_str() ) );
			TempString.clear();
		}
	}

	return Vector;
}

//-------------------------------------------------------------------------//

vector<float> le::Entity::GetVelueVectorFloat( const string& NameValue )
{
	vector<float> Vector;

	if ( Values.find( NameValue ) != Values.end() )
	{
		string TempString;
		stringstream StringStream( Values[ NameValue ] );

		while ( !StringStream.eof() )
		{
			StringStream >> TempString;
			Vector.push_back( NUMBER_TO_FLOAT( atof( TempString.c_str() ) ) );
			TempString.clear();
		}
	}

	return Vector;
}

//-------------------------------------------------------------------------//

glm::vec3& le::Entity::GetPosition()
{
	return Position;
}

//-------------------------------------------------------------------------//