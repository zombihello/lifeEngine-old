#include "MouseCursor.h"



le::MouseCursor::MouseCursor()
{
}


le::MouseCursor::~MouseCursor()
{
}

void le::MouseCursor::LoadTexture( const string sRoute )
{
    Texture.loadFromFile( sRoute );

    Sprite.setTexture( Texture );
    RectCursor = FloatRect( 0 , 0 , 1 , 1 );
}

void le::MouseCursor::MousePosition( RenderWindow & RenderWindow )
{
    RectCursor.left = Mouse::getPosition( RenderWindow ).x;
    RectCursor.top = Mouse::getPosition( RenderWindow ).y;
}

void le::MouseCursor::DrawCursor( RenderWindow & RenderWindow )
{
    Sprite.setPosition( RectCursor.left , RectCursor.top );
    RenderWindow.draw( Sprite );
}

FloatRect& le::MouseCursor::GetRectCursor()
{
    return RectCursor;
}

Vector2i& le::MouseCursor::GetVectorCursor()
{
    return Vector2i( RectCursor.left , RectCursor.top );
}
