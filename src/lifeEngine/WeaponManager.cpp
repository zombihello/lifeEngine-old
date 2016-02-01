#include "WeaponManager.h"

le::WeaponManager::WeaponManager( le::System & System )
{
    this->System = &System;
}

le::WeaponManager::WeaponManager( le::System & System , BasicWeapon * Hand )
{
    this->System = &System;
    vWeapon.push_back( Hand );
}

le::WeaponManager::~WeaponManager()
{
}

void le::WeaponManager::GiveWeapon( BasicWeapon * BasicWeapon )
{
    vWeapon.push_back( BasicWeapon );
}

void le::WeaponManager::DeleteAllWeapon()
{
    for ( int i = 0; i < vWeapon.size(); i++ )
        delete vWeapon[ i ];

    vWeapon.clear();
}

void le::WeaponManager::DeleteWeapon( const string sNameWeapon )
{
    for ( int i = 0; i < vWeapon.size(); i++ )
    {
        BasicWeapon* Weapon = vWeapon[ i ];

        if ( Weapon->GetNameWeapon() == sNameWeapon )
        {
            delete Weapon;
            vWeapon.erase( vWeapon.begin() + i );
            break;
        }
    }
}

void le::WeaponManager::UpdateWeapons()
{
    for ( int i = 0; i < vWeapon.size(); i++ )
        vWeapon[ i ]->UpdateWeapon();
}

le::BasicWeapon * le::WeaponManager::GetWeapon( const int id )
{
    if ( id - 1 > -1 && id - 1 < vWeapon.size() )
        return vWeapon[ id - 1 ];

    return NULL;
}

le::BasicWeapon * le::WeaponManager::GetWeapon( const string NameWeapon )
{
    for ( int i = 0; i < vWeapon.size(); i++ )
        if ( vWeapon[ i ]->GetNameWeapon() == NameWeapon )
            return vWeapon[ i ];

    return NULL;
}
