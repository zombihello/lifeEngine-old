#include "../../UI/Text.h"
using namespace le;

//-------------------------------------------------------------------------//

le::Text::Text( le::System &System )
{
	RenderWindow = &System.GetWindow();
}

//-------------------------------------------------------------------------//

void le::Text::LoadFont( const string sRoute )
{
	Font.loadFromFile( sRoute );
}

//-------------------------------------------------------------------------//

void le::Text::WriteText( const String sText, const int iSize, Vector2f PositionText, Color ColorText )
{
	wostringstream wostringstream;
	wostringstream << sText.toWideString();

	text.setFont( Font );
	text.setString( wostringstream.str() );
	text.setCharacterSize( iSize );
	text.setColor( ColorText );
	text.setPosition( PositionText );
}

//-------------------------------------------------------------------------//

void le::Text::UpdateText()
{
	RenderWindow->draw( text );
}

//-------------------------------------------------------------------------//

void le::Text::SetFont( const sf::Font Font )
{
	this->Font = Font;
}

//-------------------------------------------------------------------------//

void le::Text::SetText( String Text )
{
	wostringstream wostringstream;
	wostringstream << Text.toWideString();

	text.setString( wostringstream.str() );
}

//-------------------------------------------------------------------------//

sf::Text& le::Text::GetText()
{
	return text;
}

//-------------------------------------------------------------------------//

Font le::Text::GetFont() const
{
	return Font;
}

//-------------------------------------------------------------------------//
