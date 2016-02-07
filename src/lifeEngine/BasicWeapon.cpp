#include "BasicWeapon.h"

le::BasicWeapon::BasicWeapon( System & System , le::AnimationManager & AnimationManager , const bool IsPlayer )
{
    RenderWindow = &System.GetWindow();
    this->AnimationManager = &AnimationManager;
    bIsPlayer = IsPlayer;
}

le::BasicWeapon::~BasicWeapon()
{
}

int& le::BasicWeapon::GetCartridgesInStore()
{
    return iCartridgesInStore;
}

int& le::BasicWeapon::GetAmmoInStock()
{
    return iAmmoInStock;
}

int le::BasicWeapon::GetMaxAmmo() const
{
    return iMaxAmmo;
}

int le::BasicWeapon::GetDamage() const
{
    return iDamage;
}

string le::BasicWeapon::GetNameWeapon() const
{
    return sNameAnimation;
}

void le::BasicWeapon::Option( const int MaxAmmo , const int AmmoInStock , const int Damage , const int ID , const string sNameAnimation )
{
    iMaxAmmo = MaxAmmo;
    iDamage = Damage;
    this->sNameAnimation = sNameAnimation;
    iAmmoInStock = AmmoInStock;

    iCartridgesInStore = 0;
    bIsAtack = false;
}
