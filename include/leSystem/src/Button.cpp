#include "Button.h"

//-------------------------------------------------------------------------//

le::Button::Button( le::System& System )
{
    Text = new le::Text( System );
    MouseCursor = &System.GetMouseCursor();

    bClick = false;
    ColorSelect = Color::White;
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

void le::Button::CreateButton( const string sText, const int iSize, Vector2f PositionText, Color ColorText )
{
    Text->SetFont( Font );

    Text->WriteText( sText, iSize, PositionText, ColorText );
    ColorDefoult = ColorText;

    RectButton = FloatRect( PositionText, Vector2f( iSize * sText.size() / 1.5 , iSize ) );
}

//-------------------------------------------------------------------------//

void le::Button::ButtonUpdate()
{
    if ( bClick ) bClick = false;

    if ( RectButton.intersects( MouseCursor->GetRectCursor() ) )
    {
        Text->GetText().setColor( ColorSelect );

        if ( Mouse::isButtonPressed( Mouse::Left ) )
            bClick = true;
    }
    else
        if ( Text->GetText().getColor() != ColorDefoult )
            Text->GetText().setColor( ColorDefoult );

    Text->UpdateText();
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

void le::Button::SetText( const string sText )
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
