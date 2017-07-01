#include "../WeaponManager.h"

//-------------------------------------------------------------------------//

le::WeaponManager::WeaponManager( System& System )
{
	Event = &System.GetEvent();

	iIdTmpWeapon = iMaxWeapon = -1;
}

//-------------------------------------------------------------------------//

le::WeaponManager::~WeaponManager()
{
	DeleteAllWeapon();
}

//-------------------------------------------------------------------------//

void le::WeaponManager::GiveWeapon(le::BasicWeapon *BasicWeapon)
{
	vWeapon.push_back( BasicWeapon );
	iMaxWeapon++;
}

//-------------------------------------------------------------------------//

void le::WeaponManager::GiveAmmoInStock( int ammo, string NameWeapon )
{
	for ( int i = 0; i < vWeapon.size(); i++ )
	{
		BasicWeapon* Weapon = vWeapon[ i ];

		if ( Weapon->GetNameWeapon() == NameWeapon )
		{
			Weapon->AddAmmoInStock( ammo );
			return;
		}
	}
}

//-------------------------------------------------------------------------//

void le::WeaponManager::GiveAmmoInStore( int ammo, string NameWeapon )
{
	for ( int i = 0; i < vWeapon.size(); i++ )
	{
		BasicWeapon* Weapon = vWeapon[ i ];

		if ( Weapon->GetNameWeapon() == NameWeapon )
		{
			Weapon->AddAmmoInStore( ammo );
			return;
		}
	}
}

//-------------------------------------------------------------------------//

void le::WeaponManager::DeleteAllWeapon()
{
	for ( int i = 0; i < vWeapon.size(); i++ )
		delete vWeapon[ i ];

	vWeapon.clear();
}

//-------------------------------------------------------------------------//

void le::WeaponManager::DeleteWeapon( string sNameWeapon )
{
	for ( int i = 0; i < vWeapon.size(); i++ )
	{
		BasicWeapon* Weapon = vWeapon[ i ];

		if ( Weapon->GetNameWeapon() == sNameWeapon )
		{
			vWeapon.erase( vWeapon.begin() + i );
			delete Weapon;
			return;
		}
	}
}

//-------------------------------------------------------------------------//

void le::WeaponManager::UpdateWeapon()
{
	if ( iIdTmpWeapon >= 0 && iIdTmpWeapon <= iMaxWeapon )
	{
		BasicWeapon* Weapon = vWeapon[ iIdTmpWeapon ];
		Weapon->Update();
	}
}

//-------------------------------------------------------------------------//

void le::WeaponManager::SetTmpWeapon( int IdWeapon )
{
	//if ( IdWeapon-1 >= 0 && IdWeapon-1 <= iMaxWeapon )
	iIdTmpWeapon = IdWeapon-1;
}

//-------------------------------------------------------------------------//

le::BasicWeapon *le::WeaponManager::GetWeapon( int id )
{
	if ( id-1 >= 0 && id-1 < vWeapon.size() )
		return vWeapon[ id-1 ];

	return NULL;
}

//-------------------------------------------------------------------------//

int le::WeaponManager::GetMaxWeapon()
{
	return iMaxWeapon+1;
}

//-------------------------------------------------------------------------//

le::BasicWeapon* le::WeaponManager::GetWeapon( string NameWeapon )
{
	for ( int i = 0; i < vWeapon.size(); i++ )
	{
		BasicWeapon* Weapon = vWeapon[ i ];

		if ( Weapon->GetNameWeapon() == NameWeapon )
			return Weapon;
	}

	return NULL;
}

//-------------------------------------------------------------------------//

le::BasicWeapon *le::WeaponManager::GetTmpWeapon()
{
	if ( iIdTmpWeapon >= 0 && iIdTmpWeapon <= iMaxWeapon )
		return vWeapon[ iIdTmpWeapon ];

	return NULL;
}

//-------------------------------------------------------------------------//

vector<le::BasicWeapon *> le::WeaponManager::GetAllWeapon()
{
	return vWeapon;
}

//-------------------------------------------------------------------------//
