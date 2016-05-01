#include <iostream>

#include "leSystem/leSystem.h"
#include "leLevel/leLevel.h"
#include "leEntity/leEntity.h"
#include "lePhysic/Physic.h"
#include "leEntity/AI.h"
#include "leWeapons/leWeapons.h"
#include "leMedia/leMedia.h"
#include "leEfects/leEfects.h"
using namespace std;

#define HERO 0x0004
#define ENEMY 0x0008

class Bullet_Pistol : public le::BasicBullet
{
public:
	Bullet_Pistol( le::System& System, le::Physic& Physic ) : le::BasicBullet( System, Physic )
	{
		Texture.loadFromFile( "Resources/bullet.png" );
		Sprite.setTexture( Texture );

		InitEntity( 50, 50, 5, "Bullet_Pistol" );
		InitBullet( 2 );

		EntityBody = new le::Body( Vector2f( 50, 50 ), sNameEntity, le::Body::TYPE_BODY::DINAMIC );
		EntityBody->SetPropirtes( 1,0.5f );
		Physic.CreateBody( EntityBody );

		EntityBody->CreatePolygonShape( Vector2f( EntityRect.width, EntityRect.height ) );
		EntityBody->SetIgnoreCategoryCollisions( CATEGORY_VIEW_AI );
	}

	void Update( vector<le::BasicBullet*>& vBullet, vector<le::BasicItems*> vItems, vector<le::BasicPersonages*> vPersonages )
	{
		if ( EntityBody->IsColision() ) bLife = false;

		Render();
	}

	virtual ~Bullet_Pistol()
	{
	}
};

class Pistol : public le::BasicWeapon
{
public:
	Pistol( le::System& Sys, le::Physic& Ph )
	{
		System = &Sys;
		Physic = &Ph;

		InitWeapon( 2, 255, 15, "Pistol" );
	}

	void Update()
	{

	}

	void Shot( vector<le::BasicBullet*>& vBullet )
	{
		vBullet.push_back( new Bullet_Pistol( *System, *Physic ) );
	}

	virtual ~Pistol()
	{

	}
private:
	le::System*         System;
	le::Physic*         Physic;
};

class Player
{
public:
	Player( le::System& System , le::BasicPersonages* Hero )
	{
		this->Hero = Hero;
		Event = &System.GetEvent();
		Camera = &System.GetCamera();
		iTmpWeapon = 0;
	}

	void KeyCheck( vector<le::BasicBullet*>& bullet )
	{
		if ( Hero != NULL )
		{
			le::Body* body = &Hero->GetBody();
			le::WeaponManager* Wp = &Hero->GetWeaponManager();

			if ( Keyboard::isKeyPressed( Keyboard::A ) )
				body->MoveBody( Vector2f( -20, 0 ), le::Body::FORCE );

			if ( Keyboard::isKeyPressed( Keyboard::D ))
				body->MoveBody( Vector2f( 20, 0 ), le::Body::FORCE );

			if ( Keyboard::isKeyPressed( Keyboard::W ) )
				body->MoveBody(Vector2f(0,-100), le::Body::FORCE );

			if ( Event->MouseWheelScrolled )
			{
				if ( Event->mouseWheelScroll.delta == 1 && iTmpWeapon < Wp->GetMaxWeapon() )
					iTmpWeapon++;

				if ( Event->mouseWheelScroll.delta == -1 && iTmpWeapon > 0 )
					iTmpWeapon--;

				Wp->SetTmpWeapon( iTmpWeapon );

				Event->type = (sf::Event::EventType) -1;
			}

			if ( Keyboard::isKeyPressed(Keyboard::Space) )
			{
				le::BasicWeapon* Weapon = Wp->GetTmpWeapon();

				if ( Weapon != NULL )
					Weapon->Shot( bullet );
			}
		}
	}

	void Update( vector<le::Object>& obj,vector<le::BasicItems*> itm, vector<le::BasicPersonages*> pers, vector<le::BasicBullet*>& bullet )
	{
		if( Hero != NULL )
		{
			KeyCheck( bullet );

			le::Body* body = &Hero->GetBody();

			for ( int i = 0; i < itm.size(); i++ )
			{
				le::BasicItems* Itm = itm[ i ];
				le::Body* b = &Itm->GetBody();

				if ( body->IsBodyColision( b ) )
					Itm->GiveItem( *Hero );
			}

			Camera->setCenter( Vector2f( Hero->GetRect().left, Hero->GetRect().top ) );
		}
	}

	~Player()
	{

	}

private:
	int                 iTmpWeapon;

	le::BasicPersonages* Hero;
	Event*              Event;
	View*				Camera;
};

//Entity
class Box : public le::BasicEntity
{
public:
	Box( le::System& System, le::Physic& Physic, int x, int y ) : le::BasicEntity( System, Physic )
	{
		Texture.loadFromFile( "Resources/box.png" );
		Sprite.setTexture( Texture );

		InitEntity( x, y, 5, "Box" );

		EntityBody = new le::Body( Vector2f( x, y ), sNameEntity, le::Body::TYPE_BODY::DINAMIC );
		EntityBody->SetPropirtes( 1,0.5f );
		Physic.CreateBody( EntityBody );

		EntityBody->CreatePolygonShape( Vector2f( EntityRect.width, EntityRect.height ) );
		//EntityBody->SetIgnoreCategoryCollisions( CATEGORY_VIEW_AI );
	}

	void Update( vector<le::BasicBullet*>& vBullet, vector<le::BasicItems*> vItems, vector<le::BasicPersonages*> vPersonages )
	{
		Render();
	}

	virtual ~Box()
	{

	}
};

//Item
class Healtch : public le::BasicItems
{
public:
	Healtch( le::System& System, le::Physic& Physic, int x, int y ) : le::BasicItems( System, Physic )
	{
		Texture.loadFromFile( "Resources/medkit.png" );
		Sprite.setTexture( Texture );

		InitEntity( x, y, 100, "Healtch" );
		InitItem( 25 );

		EntityBody = new le::Body( Vector2f( x, y ), sNameEntity, le::Body::TYPE_BODY::DINAMIC );
		EntityBody->SetPropirtes( 1,0.5f );
		Physic.CreateBody( EntityBody );

		EntityBody->CreatePolygonShape( Vector2f( EntityRect.width, EntityRect.height ) );
		EntityBody->SetIgnoreCategoryCollisions( CATEGORY_VIEW_AI );
	}

	void Update( vector<le::BasicBullet*>& vBullet, vector<le::BasicItems*> vItems, vector<le::BasicPersonages*> vPersonages )
	{
		Render();
	}

	void GiveItem( le::BasicPersonages& Personage )
	{
		Personage.GetWeaponManager().GiveWeapon( new Pistol( *System, *Physic ) );
		bLife = false;
	}

	virtual ~Healtch()
	{

	}
};



//Personage
class Hero : public le::BasicPersonages
{
public:
	Hero( le::System& System, le::Physic& Physic, int x, int y ) : le::BasicPersonages( System, Physic )
	{
		Texture.loadFromFile( "Resources/pl.png" );
		Sprite.setTexture( Texture );

		AnimationManager->LoadAnimation( "Resources/PlayerAnimation.xml" );
		AnimationManager->SetAnimation( "stay", true );

		InitEntity( x, y, 100, "Hero" );
		InitPersonage( 0 );

		WeaponManager = new le::WeaponManager( System );
		EntityBody = new le::Body( Vector2f( x, y ), sNameEntity, le::Body::TYPE_BODY::DINAMIC );

		EntityBody->SetFixedRotation( true );
		Physic.CreateBody( EntityBody );

		EntityBody->CreatePolygonShape( Vector2f( EntityRect.width, EntityRect.height ) );

		EntityBody->SetCategorieCollisions( HERO );
		EntityBody->SetIgnoreCategoryCollisions( HERO & ~CATEGORY_VIEW_AI );

		EntityBody->SetPropirtes( 1,0 );
	}

	void Update( vector<le::BasicBullet*>& vBullet, vector<le::BasicItems*> vItems, vector<le::BasicPersonages*> vPersonages )
	{
		if ( iTempHealtch <= 0 ) bLife = false;

		WeaponManager->UpdateWeapon();
		Render();
	}

	virtual ~Hero()
	{

	}
};

class Zombie : public le::BasicPersonages ,  private le::AI
{
public:
	Zombie( le::System& System, le::Physic& Physic, int x, int y ) : le::BasicPersonages( System, Physic ), le::AI(System)
	{
		Texture.loadFromFile( "Resources/zh.png" );
		Sprite.setTexture( Texture );

		AnimationManager->LoadAnimation( "Resources/ZombieAnimation.xml" );
		AnimationManager->SetAnimation( "walk", true );

		InitEntity( x, y, 100, "Zombie" );
		InitAI( this );
		InitPersonage( 0 );

		WeaponManager = new le::WeaponManager( System );
		EntityBody = new le::Body( Vector2f( x, y ), sNameEntity, le::Body::TYPE_BODY::DINAMIC );

		EntityBody->SetFixedRotation( true );
		Physic.CreateBody( EntityBody );

		EntityBody->CreatePolygonShape( Vector2f( EntityRect.width, EntityRect.height ) );
		EntityBody->SetIgnoreCategoryCollisions( CATEGORY_VIEW_AI );
		EntityBody->SetPropirtes( 1,0 );
	}

	void Actions()
	{
		if ( IsLook() )
			MoveToObject( GetLookPersonage() );
		else
			Walk();
	}

	void MoveToObject( BasicPersonages* Personage )
	{
		if ( EntityBody->GetSpeedBody().x < 1 && EntityBody->GetSpeedBody().x > -1 )
			if ( Sprite.getPosition().x < Personage->GetRect().left )
				EntityBody->MoveBody( Vector2f( 10,0 ), le::Body::FORCE );
			else
				EntityBody->MoveBody( Vector2f( -10,0 ), le::Body::FORCE );
	}

	void Walk()
	{
		if ( EntityBody->GetSpeedBody().x < 1 && EntityBody->GetSpeedBody().x > -1 )
			EntityBody->MoveBody( Vector2f( 10,0 ), le::Body::FORCE );
	}

	void Update( vector<le::BasicBullet*>& vBullet, vector<le::BasicItems*> vItems, vector<le::BasicPersonages*> vPersonages )
	{
		if ( iTempHealtch <= 0 ) bLife = false;

		UpdateAI( vPersonages );
		WeaponManager->UpdateWeapon();
		Render();
	}

	virtual ~Zombie()
	{

	}
};

class Game : public le::BasicStagesGame
{
public:
	Game( le::System& Sys )
	{
		sys = &Sys;
		MouseCursor = &Sys.GetMouseCursor();
		Physic = new le::Physic( le::Gravity( 0.f, 9.8f ) );
		Level = new le::Level(Sys,*Physic);
		EntityManager = new le::EntityManager( Sys );

		MouseCursor->LoadTexture( "Resources/cur.png" );
		Level->LoadLevel( "Resources/a0a0.map" );

		EntityManager->CreateEntity( new Box( Sys, *Physic, 400, 0 ) );
		EntityManager->CreateItem( new Healtch( Sys, *Physic, 50, 100 ) );
		EntityManager->CreatePersonage( new Hero( Sys, *Physic, 200, 150 ) );
		EntityManager->CreatePersonage( new Zombie( Sys, *Physic, 300, 200 ));

		Pl = new Player( Sys , EntityManager->GetPersonage( "Hero" ) );
		MusicManager = new le::MusicManager( Sys );
		SoundManager = new le::SoundManager( Sys );
		LightManager = new le::LightManager( Sys, Vector2f( Level->GetMapSize().x, Level->GetMapSize().y ) );

		MusicManager->LoadMusic( "Resources/XF_song2.ogg", "Music" );
		SoundManager->LoadSound( "Resources/pl_tile1.wav", "Sound" );
		LightManager->LoadLightMask( "Resources/Spotlight.png", true );
		LightManager->CreateLight( Vector2f( 800, 600 ),500, Color::White );
	}

	~Game()
	{
		delete MouseCursor;
		delete MusicManager;
		delete SoundManager;
		delete EntityManager;
		delete Level;
		delete Physic;
		delete Pl;
	}

	void CheckStages()
	{
		Physic->UpdatePhysic( 1/60.f,8,3 );

		Level->DrawLevel();
		EntityManager->UpdateEntity();
		MouseCursor->DrawCursor( sys->GetWindow() );

		if( EntityManager->GetPersonage("Hero") != NULL )
			Pl->Update( Level->GetAllObjects(),EntityManager->GetAllItems(), EntityManager->GetAllPersonages(), EntityManager->GetAllBullets() );

		if ( Keyboard::isKeyPressed(Keyboard::Q))
			Level->ClearLevel();

		LightManager->DrawLight();

		MusicManager->PlayMusic( "Music" );

		if ( Keyboard::isKeyPressed( Keyboard::Z ) )
			SoundManager->PlaySound( "Sound" );
	}

	le::System*         sys;
	le::MouseCursor*    MouseCursor;
	le::Level*          Level;
	le::EntityManager*  EntityManager;
	le::Physic*         Physic;
	le::MusicManager*   MusicManager;
	le::SoundManager*	SoundManager;
	le::LightManager*	LightManager;
	Player*             Pl;
};

int main()
{
	le::System S( "config.cfg" );
	Game gm(S);

	S.MainLoop( gm );

	return 0;
}
