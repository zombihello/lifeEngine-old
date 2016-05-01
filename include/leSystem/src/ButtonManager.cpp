#include "ButtonManager.h"

//-------------------------------------------------------------------------//

le::ButtonManager::ButtonManager( le::System &System )
{
    this->System = &System;

    iIdButton = 0;
}

//-------------------------------------------------------------------------//

le::ButtonManager::~ButtonManager()
{
    DeleteAllButtons();
}

//-------------------------------------------------------------------------//

void le::ButtonManager::LoadFont( const string sRoute )
{
    Font.loadFromFile( sRoute );
}

//-------------------------------------------------------------------------//

void le::ButtonManager::CreateButton( const string sText, const int iSize, Vector2f PositionText, Color ColorText )
{
    vButton.push_back( new le::Button( *System ) );
    le::Button* Button = vButton[ vButton.size()-1 ];

    Button->SetFont( Font );
    Button->CreateButton( sText, iSize, PositionText, ColorText );
}

//-------------------------------------------------------------------------//

void le::ButtonManager::ButtonsUpdate()
{
    iIdButton = -1;

    for ( int i = 0; i < vButton.size(); i++ )
    {
        vButton[ i ]->ButtonUpdate();

        if ( vButton[ i ]->IsClick() )
            iIdButton = i;
    }
}

//-------------------------------------------------------------------------//

void le::ButtonManager::DeleteAllButtons()
{
    for ( int i = 0; i < vButton.size(); i++ )
        delete vButton[ i ];

    iIdButton = 0;
    vButton.clear();
}

//-------------------------------------------------------------------------//

void le::ButtonManager::DeleteButton( const int id )
{
    if ( id - 1 > -1 && id - 1 < vButton.size() )
    {
        delete vButton[ id - 1 ];
        vButton.erase( vButton.begin() + id - 1 );
    }
}

//-------------------------------------------------------------------------//

void le::ButtonManager::SetFont( const sf::Font Font )
{
    this->Font = Font;
}

//-------------------------------------------------------------------------//

void le::ButtonManager::SetColorSelect( const int IdButton, Color Color )
{
    if ( IdButton - 1 > -1 && IdButton - 1 < vButton.size() )
        vButton[ IdButton-1 ]->SetColorSelect( Color );
}

//-------------------------------------------------------------------------//

void le::ButtonManager::SetColorDefoult( const int IdButton, Color Color )
{
    if ( IdButton - 1 > -1 && IdButton - 1 < vButton.size() )
        vButton[ IdButton-1 ]->SetColorDefoult( Color );
}

//-------------------------------------------------------------------------//

void le::ButtonManager::SetText( const int IdButton, const string sText )
{
    if ( IdButton - 1 > -1 && IdButton - 1 < vButton.size() )
        vButton[ IdButton-1 ]->SetText( sText );
}

//-------------------------------------------------------------------------//

void le::ButtonManager::SetSize( const int IdButton, const int iSize )
{
    if ( IdButton - 1 > -1 && IdButton - 1 < vButton.size() )
        vButton[ IdButton-1 ]->SetSize( iSize );
}

//-------------------------------------------------------------------------//

int le::ButtonManager::GetTmpIdButton() const
{
    return iIdButton+1;
}

//-------------------------------------------------------------------------//
