#include <Graphics\Level\Entityes\Worldspawn.h>

//-------------------------------------------------------------------------//

le::Worldspawn::Worldspawn( map<string, string>& Values ) :
	BaseEntity( "worldspawn" )
{
	Skybox = Values[ "Skybox" ];
}

//-------------------------------------------------------------------------//