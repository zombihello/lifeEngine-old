#include "../GroupColision.h"

//-------------------------------------------------------------------------//

le::GroupColision::GroupColision()
{
	ColisionGroup = IgnoreGroup = 0x0000;
}

//-------------------------------------------------------------------------//

le::GroupColision::GroupColision( uint16 ColisionGroup, uint16 IgnoreGroup )
{
	this->ColisionGroup = ColisionGroup;
	this->IgnoreGroup = IgnoreGroup;
}

//-------------------------------------------------------------------------//
