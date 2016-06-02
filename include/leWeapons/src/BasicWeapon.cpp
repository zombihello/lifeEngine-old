#include "BasicWeapon.h"

//-------------------------------------------------------------------------//

le::BasicWeapon::BasicWeapon()
{
	AnimationManager = NULL;
}

//-------------------------------------------------------------------------//

le::BasicWeapon::~BasicWeapon()
{
}

//-------------------------------------------------------------------------//

void le::BasicWeapon::AddAmmoInStock( int Ammo )
{
	iAmmoInStock += Ammo;
}

//-------------------------------------------------------------------------//

void le::BasicWeapon::SetGroupColision( le::GroupColision GroupColision )
{
	this->GroupColision = GroupColision;
}

//-------------------------------------------------------------------------//

int le::BasicWeapon::GetDamage()
{
	return iDamage;
}

//-------------------------------------------------------------------------//

int le::BasicWeapon::GetMaxAmmo()
{
	return iMaxAmmo;
}

//-------------------------------------------------------------------------//

int le::BasicWeapon::GetCartridgesInStore()
{
	return iCartridgesInStore;
}

//-------------------------------------------------------------------------//

int le::BasicWeapon::GetAmmoInStock()
{
	return iAmmoInStock;
}

//-------------------------------------------------------------------------//

bool le::BasicWeapon::GetIsAtack()
{
	return bIsAtack;
}

//-------------------------------------------------------------------------//

string le::BasicWeapon::GetNameWeapon()
{
	return sNameAnimation;
}

//-------------------------------------------------------------------------//

le::GroupColision &le::BasicWeapon::GetGroupColision()
{
	return GroupColision;
}

//-------------------------------------------------------------------------//

void le::BasicWeapon::InitWeapon( int Damage, int MaxAmmo, int AmmoInStock, le::GroupColision GroupColision, string sNameAnimation )
{
	iDamage = Damage;
	iMaxAmmo = MaxAmmo;
	iAmmoInStock = AmmoInStock;
	iCartridgesInStore = 0;

	bIsAtack = false;

	this->GroupColision = GroupColision;
	this->sNameAnimation = sNameAnimation;
}

//-------------------------------------------------------------------------//

void le::BasicWeapon::InitWeapon( int Damage, int MaxAmmo, int AmmoInStock, le::GroupColision GroupColision, le::AnimationManager &AnimationManager, string sNameAnimation )
{
	iDamage = Damage;
	iMaxAmmo = MaxAmmo;
	iAmmoInStock = AmmoInStock;
	iCartridgesInStore = 0;

	bIsAtack = false;

	this->GroupColision = GroupColision;
	this->sNameAnimation = sNameAnimation;
	this->AnimationManager = &AnimationManager;
}

//-------------------------------------------------------------------------//

void le::BasicWeapon::InitWeapon( int Damage, le::AnimationManager &AnimationManager, string sNameAnimation )
{
	iDamage = Damage;
	iMaxAmmo = 0;
	iAmmoInStock = 0;
	iCartridgesInStore = 0;

	bIsAtack = false;

	this->sNameAnimation = sNameAnimation;
	this->AnimationManager = &AnimationManager;
}

//-------------------------------------------------------------------------//
