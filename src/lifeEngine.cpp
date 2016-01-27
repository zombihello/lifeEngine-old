/////////////////////////////////////
// This is example work engine
// Это пример работы движка
//////////////////////////////////////
#include "lifeEngine\System.h"
#include "lifeEngine\TextManager.h"
#include "lifeEngine\ButtonManager.h"

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

        TextManager->LoadFont( "1.ttf" ); // Load font for text
        MouseCursor->LoadTexture( "cur.png" ); // Load texture for cursor

        // Write text
        TextManager->WriteText( "Text" , 15 , Vector2f( 25.f , 25.f ) , Color::Red );
        TextManager->WriteText( "Text and Value: " , 15 , Vector2f( 25.f , 50.f ) , Color::Red , 25 );

        // Create button
        TextManager->WriteText( "This is ButtonManager" , 15 , Vector2f( 25.f , 75.f ) , Color::Red );
        ButtonManager->CreateButton( TextManager->GetText( 3 ) );
        TextManager->DeleteText( 3 );

        TextManager->WriteText( "This is Button" , 15 , Vector2f( 25.f , 100.f ) , Color::Red );
        Button->CreateButton( TextManager->GetText( 3 ) );
        TextManager->DeleteText( 3 );

    }

    ~StageGame()
    {
        delete TextManager;
    }

    void CheckStages()
    {
        // RENDER
        TextManager->UpdateText();
        ButtonManager->UpdateButtons();
        Button->UpdateButton();
        MouseCursor->DrawCursor( System->GetWindow() );
    }
private:
    le::System*             System;
    le::TextManager*        TextManager;
    le::ButtonManager*      ButtonManager;
    le::MouseCursor*        MouseCursor;
    le::Button*             Button;
};

int main()
{
    le::System System;
    StageGame StageGame( System );

    System.MainLoop( StageGame );

    return 0;
}