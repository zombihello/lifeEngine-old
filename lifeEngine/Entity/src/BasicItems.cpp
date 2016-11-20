#include "../BasicItems.h"

//-------------------------------------------------------------------------//

le::BasicItems::BasicItems( le::System &System, le::Physic& Physic ) : le::BasicEntity( System, Physic )
{
	iValue = 0;
}

//-------------------------------------------------------------------------//

le::BasicItems::~BasicItems()
{

}

//-------------------------------------------------------------------------//

void le::BasicItems::InitItem( int Value )
{
    iValue = Value;
}

//-------------------------------------------------------------------------//

int le::BasicItems::GetValue()
{
	return iValue;
}

//-------------------------------------------------------------------------//