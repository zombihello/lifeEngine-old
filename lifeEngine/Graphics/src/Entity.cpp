#include "..\Entity.h"

//-------------------------------------------------------------------------//

le::Entity::Entity( const string& EntityData )
{
	bool		IsBracket = false;
	bool		IsName = false;
	bool		IsValue = false;

	string		Name;
	string		Temp;

	// *****************************************
	// Парсим информацию о энтити

	for ( size_t IdChar = 0; IdChar < EntityData.size(); IdChar++ )
	{
		if ( EntityData[ IdChar ] == '\"' && !IsName && !IsValue && !IsBracket )
		{
			IsName = IsBracket = true;
			Temp.clear();
			continue;
		}
		else if ( EntityData[ IdChar ] == '\"' && IsName && !IsValue && IsBracket )
		{
			Name = Temp;
			IsBracket = false;
			Temp.clear();
			continue;
		}
		else if ( EntityData[ IdChar ] == '\"' && IsName && !IsValue && !IsBracket )
		{
			IsBracket = IsValue = true;
			Temp.clear();
			continue;
		}
		else if ( EntityData[ IdChar ] == '\"' && IsName && IsValue && IsBracket )
		{
			IsBracket = IsValue = IsName = false;

			Values[ Name ] = Temp;
			Temp.clear();

			continue;
		}

		Temp += EntityData[ IdChar ];
	}

	// *****************************************
	// Запоминаем название энтити и позицию, если она есть

	NameEntity = GetValueString( "classname" );
	vector<float> ArrayPosition = GetVelueVectorFloat( "origin" );

	if ( ArrayPosition.size() >= 3 )
		Position = glm::vec3( ArrayPosition[ 0 ], ArrayPosition[ 2 ], -ArrayPosition[ 1 ] );
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