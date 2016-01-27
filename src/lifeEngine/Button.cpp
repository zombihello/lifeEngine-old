#include "Button.h"


le::Button::Button( le::System& System )
{
    RenderWindow = &System.GetWindow();
    MouseCursor = &System.GetMouseCursor();
}

le::Button::~Button()
{
}

void le::Button::UpdateButton()
{
    if ( RectButton.intersects( MouseCursor->GetRectCursor() ) )
        Text.setColor( Color::White );
    else
        Text.setColor( Color );


    RenderWindow->draw( Text );
}

void le::Button::CreateButton( const sf::Text & Text )
{
    this->Text = Text;
    Color = Text.getColor();
    iId = -1;

    RectButton = FloatRect( Text.getPosition() , Vector2f( Text.getCharacterSize() * Text.getString().getSize() / 1.5 , Text.getCharacterSize() ) );
}

void le::Button::SetId( const int id )
{
    iId = id;
}

Text le::Button::GetText() const
{
    return Text;
}

Color le::Button::GetColor() const
{
    return Color;
}

FloatRect le::Button::GetRectButton() const
{
    return RectButton;
}

int le::Button::GetIdButton() const
{
    return iId;
}
