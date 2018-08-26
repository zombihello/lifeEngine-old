#include "..\Info_Player_Start.h"

//-------------------------------------------------------------------------//

le::Info_Player_Start::Info_Player_Start( const map<string, string>& Values ) :
	BaseEntity( Values )
{
	if ( ArrayString.find( "Healtch" ) != ArrayString.end() )
	{
		ArrayInt[ "Healtch" ] = atoi( ArrayString[ "Healtch" ].c_str() );
		ArrayString.erase( "Healtch" );
	}
	else
		ArrayInt[ "Healtch" ] = 100;

	if ( ArrayString.find( "Armor" ) != ArrayString.end() )
	{
		ArrayInt[ "Armor" ] = atoi( ArrayString[ "Armor" ].c_str() );
		ArrayString.erase( "Armor" );
	}
	else
		ArrayInt[ "Armor" ] = 0;
}

//-------------------------------------------------------------------------//

void le::Info_Player_Start::Update()
{}

//-------------------------------------------------------------------------//

void le::Info_Player_Start::Action()
{}

//-------------------------------------------------------------------------//