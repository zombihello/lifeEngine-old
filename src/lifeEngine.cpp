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


        TextManager->LoadFont( "1.ttf" ); // Load font for text
        MouseCursor->LoadTexture( "cur.png" ); // Load texture for cursor
        Text->SetFont( TextManager->GetFont() );
        LevelManager.LoadFromFile( "a0a0.map" ); // map uploaded | Created in the program 'Tiled'

        LightManager = new le::LightManager( LevelManager.GetMapSize().x * LevelManager.GetTileSize().x , LevelManager.GetMapSize().y * LevelManager.GetTileSize().y );

        // Write text
        TextManager->WriteText( "Text" , 15 , Vector2f( 25.f , 25.f ) , Color::Red );
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
        //LightManager->LoadMask( "light.png" ); // uploaded mask for light ID: 2
        LightManager->CreateLight( Vector2f( 150.f , 150.f ) , 150 , Color::Green );

    }

    ~StageGame()
    {
        delete TextManager;
        delete ButtonManager;
        delete Button;
        delete Text;
        delete LightManager;
    }

    void CheckStages()
    {
        // RENDER
        LevelManager.Draw( System->GetWindow() );
        TextManager->UpdateText();
        Text->UpdateText();
        ButtonManager->UpdateButtons();
        Button->UpdateButton();
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
    le::LevelManager        LevelManager;
    le::LightManager*       LightManager;
};

int main()
{
    le::System System;
    StageGame StageGame( System );

    System.MainLoop( StageGame );

    return 0;
}