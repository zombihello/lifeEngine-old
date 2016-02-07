/////////////////////////////////////
// This is example work engine
// Это пример работы движка
//////////////////////////////////////
#include "..\lifeEngine\System.h"
#include "..\lifeEngine\TextManager.h"
#include "..\lifeEngine\ButtonManager.h"
#include "..\lifeEngine\Text.h"
#include "..\lifeEngine\LevelManager.h"
#include "..\lifeEngine\LightManager.h"
#include "..\lifeEngine\EntityManager.h"
#include "..\lifeEngine\PhysicManager.h"
#include "..\lifeEngine\MusicManager.h"
#include "..\lifeEngine\SoundManager.h"
#include "..\lifeEngine\WeaponManager.h"
#include "..\lifeEngine\AnimationManager.h"
#include "..\lifeEngine\BasicHUD.h"

class Item : public le::BasicEntity , private le::PhysicManager
{
public:
    Item( le::System& System , le::Object obj ) : le::BasicEntity( System ) , PhysicManager( System.GetConfiguration().fTime )
    {
        Texture.loadFromFile( obj.GetPropertyString( "route" ) );
        Sprite.setTexture( Texture );

        Rect = FloatRect( obj.rect.left , obj.rect.top , Texture.getSize().x , Texture.getSize().y );
        iAddValue = obj.GetPropertyInt( "add" );

        BasicEntity::Option( obj.GetPropertyString( "name" ) , iAddValue , 0 , TYPES_ENTITY::ITEM );
        PhysicManager::Option( TYPE_BODY::DINAMIC );
    }


    void UpdateEntity( vector<le::Object>& obj , vector<BasicEntity*>& vEntity )
    {
        Rect.left += fDx * *BasicEntity::fTime;
        UpdatePhysic( obj , vEntity , Rect , 0 );

        Rect.top += fDy * *BasicEntity::fTime;
        UpdatePhysic( obj , vEntity , Rect , 1 );

        Sprite.setPosition( Rect.left , Rect.top );
        RenderWindow->draw( Sprite );
    }
private:
    int             iAddValue;
};

class Pistolet : public le::BasicWeapon
{
public:
    Pistolet( le::System& System , le::AnimationManager& AnimationManager , const bool IsPlayer ) : BasicWeapon( System , AnimationManager , IsPlayer )
    {
        Option( 255 , 15 , 2 , 2 , "pistolet" );
    }

    void UpdateWeapon()
    {
        AnimationManager->SetAnimation( sNameAnimation );
        AnimationManager->Play();
    }
};

class Bullet : public le::BasicEntity , private le::PhysicManager
{
public:
    Bullet( le::System& System , Vector2f Position , const string BelongsEntity , const int Damage , const bool bLeft ) : le::BasicEntity( System ) , PhysicManager( System.GetConfiguration().fTime )
    {
        AnimationManager = new le::AnimationManager( System );

        Texture.loadFromFile( "Resources/pl.png" );
        AnimationManager->LoadAnimation( "Resources/PlayerAnimation.xml" , Texture );
        AnimationManager->SetAnimation( "bullet" );

        Rect = FloatRect( Position , AnimationManager->GetSize() );
        iDamage = Damage;
        sBelongsEntity = BelongsEntity;

        BasicEntity::Option( "Bullet" , 0 , 0 , le::BasicEntity::BULLET );
        PhysicManager::Option( TYPE_BODY::STATIC );

        if ( bLeft )
            fDx = -0.3f;
        else
            fDx = 0.3f;
    }

    void UpdateEntity( vector<le::Object>& obj , vector<BasicEntity*>& vEntity )
    {
        Rect.left += fDx * *BasicEntity::fTime;
        UpdatePhysic( obj , vEntity , Rect , 0 );

        if ( ObjectCollided.type == "solid" )
            bLife = false;

        if ( EntityCollided != NULL )
            if ( EntityCollided->GetTypeEntity() != TYPES_ENTITY::BULLET )
                bLife = false;

        AnimationManager->TickAnimation();
        AnimationManager->DrawAnimation( Rect );
    }

    ~Bullet()
    {
        delete AnimationManager;
    }
private:
    int                                     iDamage;

    string                                  sBelongsEntity;
    le::AnimationManager*                   AnimationManager;
};

class Player : public le::BasicEntity , private le::PhysicManager
{
public:
    Player( le::System& System , le::Object obj ) : BasicEntity( System ) , PhysicManager( System.GetConfiguration().fTime )
    {
        AnimationManager = new le::AnimationManager( System );
        WeaponManager = new le::WeaponManager( System );
        SoundManager = new le::SoundManager( System );
        MusicManager = new le::MusicManager( System );

        Event = &System.GetEvent();
        this->System = &System;

        Texture.loadFromFile( "Resources/pl.png" );
        AnimationManager->LoadAnimation( "Resources/PlayerAnimation.xml" , Texture );
        AnimationManager->SetAnimation( "stay" );

        Rect = FloatRect( obj.rect.left , obj.rect.top , AnimationManager->GetSize().x , AnimationManager->GetSize().y );

        BasicEntity::Option( "Player" , 20 , 0 );
        PhysicManager::Option();
    }

    void KeyCheck()
    {
        if ( Keyboard::isKeyPressed( Keyboard::A ) )
            fDx = -0.1;

        if ( Keyboard::isKeyPressed( Keyboard::D ) )
            fDx = 0.1;

        if ( Keyboard::isKeyPressed( Keyboard::W ) && bOnGround )
        {
            fDy = -0.4;
            bOnGround = false;
        }
    }

    void CheckAnim()
    {
        if ( fDx != 0 )
        {
            AnimationManager->SetAnimation( "walk" );

            if ( fDx < 0 )
                bLeft = true;
            else
                bLeft = false;
        }

        if ( fDy != 0 )
            AnimationManager->SetAnimation( "jump" );

        if ( fDx == 0 && fDy == 0 )
            AnimationManager->SetAnimation( "stay" );

        AnimationManager->Play();
    }

    void UpdateEntity( vector<le::Object>& obj , vector<BasicEntity*>& vEntity )
    {
        KeyCheck();
        CheckAnim();

        Rect.left += fDx * *BasicEntity::fTime;
        UpdatePhysic( obj , vEntity , Rect , 0 );

        Rect.top += fDy * *BasicEntity::fTime;
        UpdatePhysic( obj , vEntity , Rect , 1 );

        WeaponManager->UpdateWeapons( fDx , fDy );

        if ( WeaponManager->GetTmpWeapon() != NULL && Event->type == Event::KeyReleased )
            if ( Event->key.code == Keyboard::Space )
            {
                if ( bLeft )
                    vEntity.push_back( new Bullet( *System , Vector2f( Rect.left , Rect.top + Rect.height / 2 ) , sNameEntity , WeaponManager->GetTmpWeapon()->GetDamage() , bLeft ) );
                else
                    vEntity.push_back( new Bullet( *System , Vector2f( Rect.left + Rect.width , Rect.top + Rect.height / 2 ) , sNameEntity , WeaponManager->GetTmpWeapon()->GetDamage() , bLeft ) );

                Event->key.code = ( Keyboard::Key ) - 1;
            }

        if ( EntityCollided != NULL )
        {
            string sTemp = EntityCollided->GetNameEntity();

            if ( sTemp.find( "ammo_" ) != string::npos )
            {
                sTemp.erase( sTemp.find( "ammo_" ) , 5 );

                if ( WeaponManager->GetWeapon( sTemp ) != NULL )
                    if ( WeaponManager->GetWeapon( sTemp )->GetCartridgesInStore() < WeaponManager->GetWeapon( sTemp )->GetMaxAmmo() )
                    {
                        WeaponManager->GiveAmmo( EntityCollided->GetHealtch() , sTemp );
                        EntityCollided->GetLife() = false;
                    }
            }
            else
                if ( sTemp.find( "weapon_" ) != string::npos )
                {
                    sTemp.erase( sTemp.find( "weapon_" ) , 7 );

                    if ( WeaponManager->GetWeapon( sTemp ) == NULL )
                    {
                        WeaponManager->GiveWeapon( new Pistolet( *System , *AnimationManager , true ) );
                        EntityCollided->GetLife() = false;
                    }
                }
                else
                    if ( sTemp.find( "item_" ) != string::npos )
                    {
                        sTemp.erase( sTemp.find( "item_" ) , 5 );

                        if ( sTemp == "healtch" && iHealtch < 100 )
                        {
                            iHealtch += EntityCollided->GetHealtch();
                            EntityCollided->GetLife() = false;
                        }
                    }
        }

        if ( ObjectCollided.type == "solid" )
        {
            if ( !SoundManager->GetLoadedSound( ObjectCollided.GetPropertyString( "name" ) ) )
                SoundManager->LoadSound( ObjectCollided.GetPropertyString( "route" ) , ObjectCollided.GetPropertyString( "name" ) );

            if ( fDx != 0 || fDy < 0 )
                SoundManager->PlaySound( ObjectCollided.GetPropertyString( "name" ) );
        }

        if ( ObjectCollided.name == "triger" )
        {
            if ( ObjectCollided.type == "music" )
            {

                if ( !MusicManager->GetLoadedMusic( ObjectCollided.GetPropertyString( "name" ) ) )
                    MusicManager->LoadMusic( ObjectCollided.GetPropertyString( "route" ) , ObjectCollided.GetPropertyString( "name" ) );

                TempMusicPlay = ObjectCollided.GetPropertyString( "name" );

                obj.erase( obj.begin() + iIdObjectCollided );
            }
        }

        if ( TempMusicPlay != "" )
            MusicManager->PlayMusic( TempMusicPlay );

        AnimationManager->Flip( bLeft );
        AnimationManager->TickAnimation();
        AnimationManager->DrawAnimation( Rect );

        fDx = 0;
    }

    ~Player()
    {
        delete AnimationManager;
        delete SoundManager;
        delete WeaponManager;
        delete MusicManager;
    }
private:
    bool                                bLeft;

    le::AnimationManager*               AnimationManager;
    le::SoundManager*                   SoundManager;
    le::MusicManager*                   MusicManager;
    le::System*                         System;
    Event*                              Event;
    string                              TempMusicPlay;
};



class StageGame : public le::BasicStageGame
{
public:
    StageGame( le::System& System )
    {
        this->System = &System;
        MouseCursor = &System.GetMouseCursor();

        TextManager = new le::TextManager( System );
        ButtonManager = new le::ButtonManager( System );
        Button = new le::Button( System );
        Text = new le::Text( System );
        LevelManager = new le::LevelManager;
        EntityManager = new le::EntityManager;

        Game.reset( FloatRect( 0 , 0 , System.GetConfiguration().iWindowWidth , System.GetConfiguration().iWindowHeight ) );

        TextManager->LoadFont( "Resources/1.ttf" ); // Load font for text
        MouseCursor->LoadTexture( "Resources/cur.png" ); // Load texture for cursor
        Text->SetFont( TextManager->GetFont() );
        LevelManager->LoadFromFile( "Resources/a0a0.map" ); // map uploaded | Created in the program 'Tiled'

        LightManager = new le::LightManager( System , LevelManager->GetMapSize().x * LevelManager->GetTileSize().x , LevelManager->GetMapSize().y * LevelManager->GetTileSize().y );

        vector<le::Object> obj = LevelManager->GetAllObjects();
        for ( int i = 0; i < obj.size(); i++ )
        {
            if ( obj[ i ].name == "light" )
                LightManager->CreateLight( obj[ i ] );

            if ( obj[ i ].name == "player" )
                EntityManager->CreateEntity( new Player( System , obj[ i ] ) );

            if ( obj[ i ].name == "item" )
                EntityManager->CreateEntity( new Item( System , obj[ i ] ) );
        }

        // Write text
        TextManager->WriteText( "Text" , 15 , Vector2f( 25.f , 25.f ) , Color::Red );
        TextManager->WriteText( "Text and Value: " , 15 , Vector2f( 25.f , 50.f ) , Color::Red , 25 );


        // Create button
        Text->WriteText( "This is ButtonManager" , 15 , Vector2f( 25.f , 75.f ) , Color::Red );
        ButtonManager->CreateButton( Text->GetText() );

        Text->WriteText( "This is Button" , 15 , Vector2f( 25.f , 100.f ) , Color::Red );
        Button->CreateButton( Text->GetText() );

        Text->WriteText( "This is Text!" , 15 , Vector2f( 25.f , 125.f ) , Color::Red );
    }

    ~StageGame()
    {
        delete TextManager;
        delete ButtonManager;
        delete Button;
        delete Text;
        delete LightManager;
        delete LevelManager;
        delete EntityManager;
    }

    void CheckStages()
    {
        // RENDER
        LevelManager->Draw( System->GetWindow() );
        TextManager->UpdateText();
        Text->UpdateText();
        ButtonManager->UpdateButtons();
        Button->UpdateButton();
        EntityManager->UpdateAllEntity( LevelManager->GetAllObjects() , Game );

        Game.setCenter( EntityManager->GetEntity( "Player" )->GetRect().left , EntityManager->GetEntity( "Player" )->GetRect().top );
        System->GetWindow().setView( Game );

        LightManager->DrawAllLight( System->GetWindow() , Game );
        MouseCursor->DrawCursor( System->GetWindow() );
    }
private:
    le::System*             System;
    le::TextManager*        TextManager;
    le::ButtonManager*      ButtonManager;
    le::MouseCursor*        MouseCursor;
    le::Button*             Button;
    le::Text*               Text;
    le::LevelManager*       LevelManager;
    le::LightManager*       LightManager;
    le::EntityManager*      EntityManager;
    View                    Game; // this game camera
};


int main()
{
    le::System System;
    StageGame StageGame( System );

    System.MainLoop( StageGame );

    return 0;
}