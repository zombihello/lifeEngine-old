#include "TextManager.h"

le::TextManager::TextManager( le::System & System )
{
    RenderWindow = &System.GetWindow();
}

le::TextManager::~TextManager()
{
    DeleteAllText();
}

void le::TextManager::LoadFont( const string sRoute )
{
    Font.loadFromFile( sRoute );
}

void le::TextManager::WriteText( const string sText , const int iSize , Vector2f PositionText , Color ColorText )
{
    vText.push_back( new Text );
    Text* Text = vText[ vText.size() - 1 ];

    ostringstream ostringstream;
    ostringstream << sText;

    Text->setFont( Font );
    Text->setString( ostringstream.str() );
    Text->setCharacterSize( iSize );
    Text->setColor( ColorText );
    Text->setPosition( PositionText );
}

void le::TextManager::UpdateText()
{
    for ( int i = 0; i < vText.size(); i++ )
        RenderWindow->draw( *vText[ i ] ); // Draw all text in vector
}

void le::TextManager::DeleteAllText()
{
    for ( int i = 0; i < vText.size(); i++ )
        delete vText[ i ]; // Delete all text in vector

    vText.clear();
}

void le::TextManager::DeleteText( int ID )
{
    if ( ID - 1 > -1 && ID - 1 < vText.size() )
    {
        delete vText[ ID - 1 ];
        vText.erase( vText.begin() + ID - 1 );    
    }
}

Text le::TextManager::GetText( int ID )
{
    return *vText[ ID - 1 ];
}

Font le::TextManager::GetFont() const
{
    return Font;
}
