#include "../BasicPersonages.h"

//-------------------------------------------------------------------------//

le::BasicPersonages::BasicPersonages( le::System &System, le::Physic& Physic ) : le::BasicEntity( System, Physic )
{
	WeaponManager = NULL;
}

//-------------------------------------------------------------------------//

le::BasicPersonages::~BasicPersonages()
{
    if ( WeaponManager != NULL )
    {
        delete WeaponManager;
        WeaponManager = NULL;
    }
}

//-------------------------------------------------------------------------//

le::WeaponManager &le::BasicPersonages::GetWeaponManager()
{
    return *WeaponManager;
}

//-------------------------------------------------------------------------//

void le::BasicPersonages::InitPersonage( int Armor )
{
    iArmor = Armor;
}

//-------------------------------------------------------------------------//
