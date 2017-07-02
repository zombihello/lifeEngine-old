#include "../BasicPersonages.h"

//-------------------------------------------------------------------------//

le::BasicPersonages::BasicPersonages( le::System &System, le::Physic2D& Physic ) : le::BasicEntity( System, Physic )
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

void le::BasicPersonages::AddEnemy( string sNameEntity )
{
	ListEnemies[ sNameEntity ] = sNameEntity;
}

//-------------------------------------------------------------------------//

void le::BasicPersonages::ClearListEnemies()
{
	ListEnemies.clear();
}

//-------------------------------------------------------------------------//

void le::BasicPersonages::RemoveEnemy( string sNameEntity )
{
	if ( ListEnemies.count( sNameEntity ) != 0 )
		ListEnemies.erase( ListEnemies.find( sNameEntity ) );
}

//-------------------------------------------------------------------------//

le::WeaponManager &le::BasicPersonages::GetWeaponManager()
{
	return *WeaponManager;
}

//-------------------------------------------------------------------------//

le::GroupColision &le::BasicPersonages::GetGroupColisionWeapon()
{
	return GroupColisionWeapon;
}

//-------------------------------------------------------------------------//

le::GroupColision &le::BasicPersonages::GetGroupColisionPersonage()
{
	return GroupColisionPersonage;
}

//-------------------------------------------------------------------------//

bool le::BasicPersonages::IsEnemy( string sNameEntity )
{
	if ( ListEnemies.count( sNameEntity ) != 0 )
		return true;

	return false;
}

//-------------------------------------------------------------------------//

bool le::BasicPersonages::IsAtackHand()
{
	return bAtackHand;
}

//-------------------------------------------------------------------------//

int &le::BasicPersonages::GetArmor()
{
	return iArmor;
}

//-------------------------------------------------------------------------//

int le::BasicPersonages::GetDamageHand()
{
	return iDamageHand;
}

//-------------------------------------------------------------------------//

map<string, string> le::BasicPersonages::GetListEnemies()
{
	return ListEnemies;
}

//-------------------------------------------------------------------------//

void le::BasicPersonages::InitPersonage( int Armor, int DamageHand, le::GroupColision GroupColisionPersonage )
{
	iArmor = Armor;
	iDamageHand = DamageHand;
	bAtackHand = false;
	this->GroupColisionPersonage = GroupColisionPersonage;
}

//-------------------------------------------------------------------------//

void le::BasicPersonages::InitPersonage( int Armor, int DamageHand, le::GroupColision GroupColisionPersonage, le::GroupColision GroupColisionWeapon )
{
	iArmor = Armor;
	iDamageHand = DamageHand;
	bAtackHand = false;
	this->GroupColisionPersonage = GroupColisionPersonage;
	this->GroupColisionWeapon = GroupColisionWeapon;
}

//-------------------------------------------------------------------------//
