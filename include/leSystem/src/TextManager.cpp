#include "../TextManager.h"

//-------------------------------------------------------------------------//

le::TextManager::TextManager( le::System & System )
{
	this->System = &System;
}

//-------------------------------------------------------------------------//

le::TextManager::~TextManager()
{
    DeleteAllText();
}

//-------------------------------------------------------------------------//

void le::TextManager::LoadFont( const string sRoute )
{
    Font.loadFromFile( sRoute );
}

//-------------------------------------------------------------------------//

void le::TextManager::WriteText( const string sText , const int iSize , Vector2f PositionText , Color ColorText )
{
	le::Text* Text = new le::Text( *System );

	Text->SetFont( Font );
	Text->WriteText( sText, iSize, PositionText, ColorText );

	vText.push_back( Text );
}

//-------------------------------------------------------------------------//

void le::TextManager::UpdateText()
{
	for ( int i = 0; i < vText.size() ; i++ )
		vText[ i ]->UpdateText();
}

//-------------------------------------------------------------------------//

void le::TextManager::DeleteAllText()
{
    for ( int i = 0; i < vText.size(); i++ )
        delete vText[ i ];

    vText.clear();
}

//-------------------------------------------------------------------------//

void le::TextManager::DeleteText( int ID )
{
    if ( ID - 1 > -1 && ID - 1 < vText.size() )
    {
        delete vText[ ID - 1 ];
        vText.erase( vText.begin() + ID - 1 );
    }
}

//-------------------------------------------------------------------------//

le::Text le::TextManager::GetText( int ID )
{
    return *vText[ ID - 1 ];
}

//-------------------------------------------------------------------------//

Font le::TextManager::GetFont() const
{
    return Font;
}

//-------------------------------------------------------------------------//