#include "..\..\3D\Entity.h"

//-------------------------------------------------------------------------//

le::Entity::Entity( TiXmlElement& ElementEntity )
{
	if ( ElementEntity.Attribute( "Name" ) != NULL )
		sNameEntity = ElementEntity.Attribute( "Name" );

	// Работаем с контейнером Position
	TiXmlElement* position;
	position = ElementEntity.FirstChildElement( "Position" );

	Position.x = atof( position->Attribute( "X" ) );
	Position.y = atof( position->Attribute( "Y" ) );
	Position.z = atof( position->Attribute( "Z" ) );

	// Работаем с контейнером Value
	TiXmlElement* Value;
	Value = ElementEntity.FirstChildElement( "Value" );

	while ( Value )
	{
		string Key = Value->Attribute( "Name" );
		string sValue = "";

		if ( Value->Attribute( "Value" ) != NULL )
			string sValue = Value->Attribute( "Value" );

		mValues[Key] = sValue;

		Value = Value->NextSiblingElement();
	}
}

//-------------------------------------------------------------------------//

string le::Entity::GetValueString( string NameValue )
{
	if ( mValues.count( NameValue ) )
		return mValues[NameValue];

	return "ERROR";
}

//-------------------------------------------------------------------------//

Vector3f le::Entity::GetPosition()
{
	return Position;
}

//-------------------------------------------------------------------------//

int le::Entity::GetValueInt( string NameValue )
{
	if ( mValues.count( NameValue ) )
		return atoi( mValues[NameValue].c_str() );

	return -1;
}

//-------------------------------------------------------------------------//

float le::Entity::GetValueFloat( string NameValue )
{
	if ( mValues.count( NameValue ) )
		return atof( mValues[NameValue].c_str() );

	return -1;
}

//-------------------------------------------------------------------------//

string le::Entity::GetNameEntity()
{
	return sNameEntity;
}

//-------------------------------------------------------------------------//