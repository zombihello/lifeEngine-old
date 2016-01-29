#include "Console.h"

#include <iostream>

le::Console::Console( const int iWindowWidth , const int iWindowHeight )
{
    ColorBackground = Color::Black;
    fLastY = 0;
    bConsoleOpen = false;

    Background.setPosition( 0 , 0 );
    Background.setSize( Vector2f( iWindowWidth , iWindowHeight / 2 ) );
    Background.setFillColor( Color::Color( ColorBackground.r , ColorBackground.g , ColorBackground.b , 200 ) );

    InputText.setCharacterSize( 15 );
    InputText.setColor( Color::White );
    InputText.setPosition( 5 , Background.getSize().y - 20 );
}


le::Console::~Console()
{
    ClearConsole();
}

void le::Console::LoadFont( const string sRoute )
{
    Font.loadFromFile( sRoute );
    InputText.setFont( Font );
}

void le::Console::WriteToConsole( const string sText , Color ColorText )
{
    ostringstream ostringstream;
    ostringstream << sText;

    vConsole.push_back( new Text );
    sf::Text* Text = vConsole[ vConsole.size() - 1 ];

    Text->setFont( Font );
    Text->setString( ostringstream.str() );
    Text->setCharacterSize( 15 );
    Text->setColor( ColorText );
    Text->setPosition( 5 , fLastY + 20 );

    fLastY = Text->getPosition().y;
}

void le::Console::UpdateConsole( RenderWindow & RenderWindow , Event& Event )
{
    if ( Event.key.code == Keyboard::Tilde )
    {
        bConsoleOpen = !bConsoleOpen;
        Event.key.code = ( Keyboard::Key ) - 1;
        if ( !bConsoleOpen ) InputText.setString( "" );
    }

    if ( bConsoleOpen )
    {

        RenderWindow.draw( Background );

        for ( int i = 0; i < vConsole.size(); i++ )
        {
            if ( i < 13 )
                RenderWindow.draw( *vConsole[ i ] );
            else
            {
                ClearConsole();
                fLastY = 0;
            }
        }

        InputInConsole( RenderWindow , Event );
    }
}

void le::Console::ClearConsole()
{
    for ( int i = 0; i < vConsole.size(); i++ )
        delete vConsole[ i ];

    vConsole.clear();
}

void le::Console::InputInConsole( RenderWindow & RenderWindow , Event& Event )
{
    ostringstream ostringstream;

    if ( Event.type == Event::TextEntered )
        if ( Event.text.unicode < 128 )
        {
            if ( static_cast< char >( Event.text.unicode ) == ( char ) 13 )
            {
                vConsole.push_back( new Text( InputText ) );
                sf::Text* Text = vConsole[ vConsole.size() - 1 ];

                Text->setPosition( 5 , fLastY + 20 );
                InputText.setString( "" );

                fLastY = Text->getPosition().y;

                ParsingComand( Text->getString() );
            }
            else
                ostringstream << static_cast< char >( Event.text.unicode );
            InputText.setString( InputText.getString() + ostringstream.str() );

            Event.type = ( Event::EventType ) - 1;
        }


    RenderWindow.draw( InputText );
}

void le::Console::ParsingComand( const string sComand )
{
    // this is prototype
    if ( sComand == "half-life 3" )
        WriteToConsole( "[info] LoL. No!" , Color::Green );
    else
        WriteToConsole( "Error: Comand [" + sComand + "] Not Found" , Color::Red );
}
