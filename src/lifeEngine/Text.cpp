#include "Text.h"

le::Text::Text( System & System )
{
    RenderWindow = &System.GetWindow();
    Console = &System.GetConsole();
}

le::Text::~Text()
{
}

void le::Text::LoadFont( const string sRoute )
{
    if ( !Font.loadFromFile( sRoute ) )
        Console->WriteToConsole( "Error: File [" + sRoute + "] Not Found" , Color::Red );
}

void le::Text::WriteText( const string sText , const int iSize , Vector2f PositionText , Color ColorText )
{
    ostringstream ostringstream;
    ostringstream << sText;

    text.setFont( Font );
    text.setString( ostringstream.str() );
    text.setCharacterSize( iSize );
    text.setColor( ColorText );
    text.setPosition( PositionText );
}

void le::Text::UpdateText()
{
    RenderWindow->draw( text );
}

void le::Text::SetFont( const sf::Font Font )
{
    this->Font = Font;
}

sf::Text le::Text::GetText() const
{
    return text;
}

Font le::Text::GetFont() const
{
    return Font;
}
