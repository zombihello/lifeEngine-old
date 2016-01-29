/////////////////////////////////////
// This is example work engine
// Это пример работы движка
//////////////////////////////////////
#include "lifeEngine\System.h"
#include "lifeEngine\TextManager.h"
#include "lifeEngine\ButtonManager.h"
#include "lifeEngine\Text.h"
#include "lifeEngine\LevelManager.h"
#include "lifeEngine\LightManager.h"
#include "lifeEngine\EntityManager.h"
#include "lifeEngine\PhysicManager.h"
#include "lifeEngine\MusicManager.h"
#include "lifeEngine\SoundManager.h"

class Player : public le::BasicEntity , private le::PhysicManager
{
public:
    Player( le::System& System ) : le::PhysicManager( System.GetConfiguration().fTime )
    {
        RenderWindow = &System.GetWindow();
        fTime = &System.GetConfiguration().fTime;

        Texture.loadFromFile( "pl.png" ); // load texture for player
        Sprite.setTexture( Texture );

        Rect = FloatRect( 20 , 20 , Texture.getSize().x , Texture.getSize().y );

        le::BasicEntity::Option( "Player" , 100 , 0 );  // init values
        le::PhysicManager::Option();
    }

    void UpdateEntity( vector<le::Object> obj , vector<BasicEntity*> vEntity )
    {
        if ( Keyboard::isKeyPressed( Keyboard::A ) ) fDx = -0.1;  // Move left
        if ( Keyboard::isKeyPressed( Keyboard::D ) ) fDx = 0.1; // Move right
        if ( Keyboard::isKeyPressed( Keyboard::W ) && bOnGround ) { fDy = -0.4; bOnGround = false; } // Move jump

        Rect.left += fDx * *fTime;
        UpdatePhysic( obj , vEntity , Rect , 0 ); // update physic

        Rect.top += fDy * *fTime;
        UpdatePhysic( obj , vEntity , Rect , 1 );

        if ( GetNameEntityCollided() == "Zombie" )
            Sprite.setColor( Color::Red );

        Sprite.setPosition( Rect.left , Rect.top ); // update position
        RenderWindow->draw( Sprite );

        fDx = 0;
    }
private:
    RenderWindow*       RenderWindow;
    float*              fTime;
};



class Zombie : public le::BasicEntity , private le::PhysicManager
{
public:
    Zombie( le::System& System ) : le::PhysicManager( System.GetConfiguration().fTime )
    {
        RenderWindow = &System.GetWindow();
        fTime = &System.GetConfiguration().fTime;

        Texture.loadFromFile( "zh.png" ); // load texture for player
        Sprite.setTexture( Texture );

        Rect = FloatRect( 50 , 50 , Texture.getSize().x , Texture.getSize().y );

        le::BasicEntity::Option( "Zombie" , 100 , 0 );  // init values
        le::PhysicManager::Option();
    }

    void UpdateEntity( vector<le::Object> obj , vector<BasicEntity*> vEntity )
    {

        if ( Keyboard::isKeyPressed( Keyboard::Left ) ) fDx = -0.1;  // Move left
        if ( Keyboard::isKeyPressed( Keyboard::Right ) ) fDx = 0.1; // Move right
        if ( Keyboard::isKeyPressed( Keyboard::Up ) && bOnGround ) { fDy = -0.4; bOnGround = false; } // Move jump

        Rect.left += fDx * *fTime;
        UpdatePhysic( obj , vEntity , Rect , 0 ); // update physic

        Rect.top += fDy * *fTime;
        UpdatePhysic( obj , vEntity , Rect , 1 );

        if ( GetNameEntityCollided() == "Player" )
            Sprite.setColor( Color::Red );

        Sprite.setPosition( Rect.left , Rect.top ); // update position
        RenderWindow->draw( Sprite );

        fDx = 0;
    }
private:
    RenderWindow*       RenderWindow;
    float*              fTime;
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
        LevelManager = new le::LevelManager( System );
        EntityManager = new le::EntityManager( System );
        MusicManager = new le::MusicManager( System );
        SoundManager = new le::SoundManager( System );

        EntityManager->CreateEntity( new Player( System ) );
        EntityManager->CreateEntity( new Zombie( System ) );

        TextManager->LoadFont( "1.ttf" ); // Load font for text
        MouseCursor->LoadTexture( "cur.png" ); // Load texture for cursor
        Text->SetFont( TextManager->GetFont() );
        LevelManager->LoadFromFile( "a0a0.map" ); // map uploaded | Created in the program 'Tiled'
        MusicManager->LoadMusic( "XF_song2.ogg" , "music" , true );
        SoundManager->LoadSound( "wpn_select.wav" , "sound" );

        LightManager = new le::LightManager( System , LevelManager->GetMapSize().x * LevelManager->GetTileSize().x , LevelManager->GetMapSize().y * LevelManager->GetTileSize().y );

        // Write text
        TextManager->WriteText( "For open console press '~'" , 15 , Vector2f( 25.f , 25.f ) , Color::Red );
        TextManager->WriteText( "Text and Value: " , 15 , Vector2f( 25.f , 50.f ) , Color::Red , 25 );


        // Create button
        Text->WriteText( "This is ButtonManager" , 15 , Vector2f( 25.f , 75.f ) , Color::Red );
        ButtonManager->CreateButton( Text->GetText() );

        Text->WriteText( "This is Button" , 15 , Vector2f( 25.f , 100.f ) , Color::Red );
        Button->CreateButton( Text->GetText() );

        Text->WriteText( "This is Text!" , 15 , Vector2f( 25.f , 125.f ) , Color::Red );

        //Create light
        LightManager->LoadMask( "Spotlight.png" ); // uploaded mask for light ID: 1
        LightManager->CreateLight( Vector2f( 50.f , 50.f ) , 100 , Color::White );
        LightManager->LoadMask( "light.png" ); // uploaded mask for light ID: 2
        LightManager->CreateLight( Vector2f( 450.f , 150.f ) , 50 , Color::Green );
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
        delete MusicManager;
        delete SoundManager;
    }

    void CheckStages()
    {
        // RENDER
        MusicManager->PlayMusic( "music" );

        if ( System->GetTypeEvent() == Event::MouseButtonPressed )
                SoundManager->PlaySound( "sound" );

        LevelManager->Draw( System->GetWindow() );
        TextManager->UpdateText();
        Text->UpdateText();
        ButtonManager->UpdateButtons();
        Button->UpdateButton();
        EntityManager->UpdateAllEntity( LevelManager->GetAllObjects() , EntityManager->GetAllEntity() );
        LightManager->DrawAllLight( System->GetWindow() );
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
    le::MusicManager*       MusicManager;
    le::SoundManager*       SoundManager;
};





int main()
{
    le::System System( "1.ttf" ); // "1.ttf" - this is route to font for console | ~ - open console
    StageGame StageGame( System );

    System.MainLoop( StageGame );

    return 0;
}