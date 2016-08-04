#include "../Button.h"

//-------------------------------------------------------------------------//

le::Button::Button( le::System& System )
{
	Text = new le::Text( System );
	MouseCursor = &System.GetMouseCursor();

	bClick = false;
	ColorSelect = Color::White;
	iIdButton = 1;
}

//-------------------------------------------------------------------------//

le::Button::~Button()
{
	delete Text;
}

//-------------------------------------------------------------------------//

void le::Button::LoadFont( const string sRoute )
{
	Font.loadFromFile( sRoute );
}

//-------------------------------------------------------------------------//

void le::Button::CreateButton( const String sText, const int iSize, Vector2f PositionText, Color ColorText )
{
	Text->SetFont( Font );

	Text->WriteText( sText, iSize, PositionText, ColorText );
	ColorDefoult = ColorText;

	RectButton = FloatRect( PositionText, Vector2f( iSize * sText.getSize() / 1.5 , iSize ) );
}

//-------------------------------------------------------------------------//

void le::Button::CreateButton( sf::Text Text )
{
	this->Text->SetFont( Font );

	this->Text->WriteText( Text.getString(), Text.getCharacterSize(), Text.getPosition(), Text.getColor() );
	ColorDefoult = Text.getColor();

	RectButton = FloatRect( Text.getPosition(), Vector2f( Text.getCharacterSize() * Text.getString().getSize() / 1.5 , Text.getCharacterSize() ) );
}

//-------------------------------------------------------------------------//

void le::Button::ButtonUpdate()
{
	if ( bClick ) bClick = false;

	if ( RectButton.intersects( MouseCursor->GetRectCursor() ) )
	{
		Text->GetText().setColor( ColorSelect );

		if ( MouseCursor->ButtonPressed( Mouse::Left ) )
		{
			bClick = true;
			Actions();
		}
	}
	else
		if ( Text->GetText().getColor() != ColorDefoult )
			Text->GetText().setColor( ColorDefoult );

	Text->UpdateText();
}

//-------------------------------------------------------------------------//

void le::Button::SetId( int id )
{
	iIdButton = id;
}

//-------------------------------------------------------------------------//

void le::Button::SetFont( const sf::Font Font )
{
	this->Font = Font;
}

//-------------------------------------------------------------------------//

void le::Button::SetColorSelect( Color Color )
{
	ColorSelect = Color;
}

//-------------------------------------------------------------------------//

void le::Button::SetColorDefoult( Color Color )
{
	Text->GetText().setColor( Color );
	ColorDefoult = Color;
}

//-------------------------------------------------------------------------//

void le::Button::SetText( const String sText )
{
	Text->GetText().setString( sText );
}

//-------------------------------------------------------------------------//

void le::Button::SetSize( const int iSize )
{
	Text->GetText().setCharacterSize( iSize );

	RectButton.width = iSize * Text->GetText().getString().getSize() / 1.5 ;
	RectButton.height = iSize;
}

//-------------------------------------------------------------------------//

bool le::Button::IsClick() const
{
	return bClick;
}

//-------------------------------------------------------------------------//

String le::Button::GetTextInButton()
{
	return Text->GetText().getString();
}

//-------------------------------------------------------------------------//
