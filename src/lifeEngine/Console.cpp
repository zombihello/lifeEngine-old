#include "Console.h"



le::Console::Console( const int iWindowWidth , const int iWindowHeight )
{
    ColorBackground = Color::Black;
    fLastY = 0;

    Background.setPosition( 0 , 0 );
    Background.setSize( Vector2f( iWindowWidth , iWindowHeight / 2 ) );
    Background.setFillColor( Color::Color( ColorBackground.r , ColorBackground.g , ColorBackground.b , 200 ) );
}


le::Console::~Console()
{
    ClearConsole();
}

void le::Console::LoadFont( const string sRoute )
{
    Font.loadFromFile( sRoute );
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

void le::Console::UpdateCondole( RenderWindow & RenderWindow )
{
    if ( Keyboard::isKeyPressed( Keyboard::Tilde ) )
    {
        RenderWindow.draw( Background );

        for ( int i = 0; i < vConsole.size(); i++ )
        {
            if ( i < 13 )
                RenderWindow.draw( *vConsole[ i ] );
            else ClearConsole();
        }
    }
}

void le::Console::ClearConsole()
{
    for ( int i = 0; i < vConsole.size(); i++ )
        delete vConsole[ i ];

    vConsole.clear();
}
