#include "WeaponManager.h"

le::WeaponManager::WeaponManager( le::System & System )
{
    this->System = &System;
    TypeEvent = &System.GetTypeEvent();
    Event = &System.GetEvent();
    iMaxWeapon = iIdTmpWeapon = 0;
}

le::WeaponManager::~WeaponManager()
{
}

void le::WeaponManager::GiveWeapon( BasicWeapon * BasicWeapon )
{
    vWeapon.push_back( BasicWeapon );
    iMaxWeapon++;
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

void le::WeaponManager::UpdateWeapons( const float fDx , const float fDy )
{
    if ( *TypeEvent == Event::MouseWheelScrolled )
    {
        if ( Event->mouseWheelScroll.delta == 1 )
        {
            if ( iIdTmpWeapon < iMaxWeapon )   iIdTmpWeapon++;
            else iIdTmpWeapon = 0;
        }

        if ( Event->mouseWheelScroll.delta == -1 )
        {
            if ( iIdTmpWeapon > 0 ) iIdTmpWeapon--;
            else iIdTmpWeapon = iMaxWeapon;
        }
    }

    if ( fDx == 0 && fDy == 0 )
        if ( vWeapon.size() > 0 && iIdTmpWeapon > 0 )
            vWeapon[ iIdTmpWeapon - 1 ]->UpdateWeapon();
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

le::BasicWeapon * le::WeaponManager::GetTmpWeapon()
{
    if ( iIdTmpWeapon - 1 > -1 && iIdTmpWeapon - 1 < vWeapon.size() )
        return vWeapon[ iIdTmpWeapon - 1 ];

    return NULL;
}
