#include <Graphics\Level\Entityes\Info_Player_Start.h>

//-------------------------------------------------------------------------//

le::Info_Player_Start::Info_Player_Start( map<string, string>& Values ) :
	BaseEntity( "info_player_start" )
{
	Position = ValueToVec3( Values[ "origin" ] );
	Healtch = StringToNumber<int>( Values[ "Healtch" ], 100 );
	Armor = StringToNumber<int>( Values[ "Armor" ], 0 );
}

//-------------------------------------------------------------------------//