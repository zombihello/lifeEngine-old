#include "EntityManager.h"

//-------------------------------------------------------------------------//

le::EntityManager::EntityManager( le::System& System )
{
	Camera = &System.GetGameCamera();
}

//-------------------------------------------------------------------------//

le::EntityManager::~EntityManager()
{
	DeleteAll();
}

//-------------------------------------------------------------------------//

void le::EntityManager::CreateEntity( le::BasicEntity *BasicEntity )
{
	vEntity.push_back( BasicEntity );
}

//-------------------------------------------------------------------------//

void le::EntityManager::CreateBullet( le::BasicBullet *BasicBullet )
{
	vBullet.push_back( BasicBullet );
}

//-------------------------------------------------------------------------//

void le::EntityManager::CreateItem( le::BasicItems *BasicItems )
{
	vItems.push_back( BasicItems );
}

//-------------------------------------------------------------------------//

void le::EntityManager::CreatePersonage( le::BasicPersonages *BasicPersonages )
{
	vPersonages.push_back( BasicPersonages );
}

//-------------------------------------------------------------------------//

void le::EntityManager::UpdateEntity()
{
	FloatRect TempCamera = FloatRect( Camera->getCenter().x - Camera->getSize().x / 2,
									  Camera->getCenter().y - Camera->getSize().y / 2,
									  Camera->getSize().x,
									  Camera->getSize().y );

	for ( int i = 0; i < vEntity.size(); )
	{
		BasicEntity* Entity = vEntity[ i ];

		if ( TempCamera.intersects( Entity->GetRect() ) )
			Entity->Update( vBullet, vItems, vPersonages );

		if ( !Entity->GetLife() )
		{
			vEntity.erase( vEntity.begin() + i );
			delete Entity;
		}
		else i++;
	}

	for ( int i = 0; i < vItems.size(); )
	{
		BasicItems* Item = vItems[ i ];

		if ( TempCamera.intersects( Item->GetRect() ) )
			Item->Update( vBullet, vItems, vPersonages );

		if ( !Item->GetLife() )
		{
			vItems.erase( vItems.begin() + i );
			delete Item;
		}
		else i++;
	}

	for ( int i = 0; i < vPersonages.size(); )
	{
		BasicPersonages* Personage = vPersonages[ i ];

		if ( TempCamera.intersects( Personage->GetRect() ) )
			Personage->Update( vBullet, vItems, vPersonages );

		if ( !Personage->GetLife() )
		{
			vPersonages.erase( vPersonages.begin() + i );
			delete Personage;
		}
		else i++;
	}

	for ( int i = 0; i < vBullet.size(); )
	{
		BasicBullet* Bullet = vBullet[ i ];

		if ( TempCamera.intersects( Bullet->GetRect() ) )
			Bullet->Update( vBullet, vItems, vPersonages );

		if ( !Bullet->GetLife() )
		{
			vBullet.erase( vBullet.begin() + i );
			delete Bullet;
		}
		else i++;
	}
}

//-------------------------------------------------------------------------//

void le::EntityManager::DeleteAll()
{
	DeleteAllEntity();
	DeleteAllBullets();
	DeleteAllItems();
	DeleteAllPersonages();
}

//-------------------------------------------------------------------------//

void le::EntityManager::DeleteAllEntity()
{
	for ( int i = 0; i < vEntity.size(); i++ )
		delete vEntity[ i ];

	vEntity.clear();
}

//-------------------------------------------------------------------------//

void le::EntityManager::DeleteAllBullets()
{
	for ( int i = 0; i < vBullet.size(); i++ )
		delete vBullet[ i ];

	vBullet.clear();
}

//-------------------------------------------------------------------------//

void le::EntityManager::DeleteAllItems()
{
	for ( int i = 0; i < vItems.size(); i++ )
		delete vItems[ i ];

	vItems.clear();
}

//-------------------------------------------------------------------------//

void le::EntityManager::DeleteAllPersonages()
{
	for ( int i = 0; i < vPersonages.size(); i++ )
		delete vPersonages[ i ];

	vPersonages.clear();
}

//-------------------------------------------------------------------------//

le::BasicEntity *le::EntityManager::GetEntity( const string sName )
{
	for ( int i = 0; i < vEntity.size(); i++ )
	{
		BasicEntity* Entity = vEntity[ i ];

		if ( Entity->GetNameEntity() == sName )
			return Entity;
	}

	return NULL;
}

//-------------------------------------------------------------------------//

le::BasicBullet *le::EntityManager::GetBullet( const string sName )
{
	for ( int i = 0; i < vBullet.size(); i++ )
	{
		BasicBullet* Bullet = vBullet[ i ];

		if ( Bullet->GetNameEntity() == sName )
			return Bullet;
	}

	return NULL;
}

//-------------------------------------------------------------------------//

le::BasicItems *le::EntityManager::GetItem( const string sName )
{
	for ( int i = 0; i < vItems.size(); i++ )
	{
		BasicItems* Item = vItems[ i ];

		if ( Item->GetNameEntity() == sName )
			return Item;
	}

	return NULL;
}

//-------------------------------------------------------------------------//

le::BasicPersonages *le::EntityManager::GetPersonage( const string sName )
{
	for ( int i = 0; i < vPersonages.size(); i++ )
	{
		BasicPersonages* Personage = vPersonages[ i ];

		if ( Personage->GetNameEntity() == sName )
			return Personage;
	}

	return NULL;
}

//-------------------------------------------------------------------------//

vector<le::BasicEntity *>& le::EntityManager::GetAllEntity()
{
	return vEntity;
}

//-------------------------------------------------------------------------//

vector<le::BasicBullet *>& le::EntityManager::GetAllBullets()
{
	return vBullet;
}

//-------------------------------------------------------------------------//

vector<le::BasicItems *>& le::EntityManager::GetAllItems()
{
	return vItems;
}

//-------------------------------------------------------------------------//

vector<le::BasicPersonages *>& le::EntityManager::GetAllPersonages()
{
	return vPersonages;
}

//-------------------------------------------------------------------------//
