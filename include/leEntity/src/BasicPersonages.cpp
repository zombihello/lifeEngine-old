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

map<string, string> le::BasicPersonages::GetListEnemies()
{
	return ListEnemies;
}

//-------------------------------------------------------------------------//

void le::BasicPersonages::InitPersonage( int Armor, le::GroupColision GroupColisionPersonage )
{
	iArmor = Armor;
	this->GroupColisionPersonage = GroupColisionPersonage;
}

//-------------------------------------------------------------------------//

void le::BasicPersonages::InitPersonage( int Armor, le::GroupColision GroupColisionPersonage, le::GroupColision GroupColisionWeapon )
{
	iArmor = Armor;
	this->GroupColisionPersonage = GroupColisionPersonage;
	this->GroupColisionWeapon = GroupColisionWeapon;
}

//-------------------------------------------------------------------------//
