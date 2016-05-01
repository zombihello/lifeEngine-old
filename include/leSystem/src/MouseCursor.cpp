#include "../MouseCursor.h"
using namespace le;

//-------------------------------------------------------------------------//

MouseCursor::MouseCursor()
{

}

//-------------------------------------------------------------------------//

void MouseCursor::LoadTexture( const string sRoute )
{
    Texture.loadFromFile( sRoute );

    Sprite.setTexture( Texture );
    RectCursor = FloatRect( 0 , 0 , 1 , 1 );
}

//-------------------------------------------------------------------------//

void MouseCursor::UpdatePosition( RenderWindow &RenderWindow )
{
    RectCursor.left = Mouse::getPosition( RenderWindow ).x;
    RectCursor.top = Mouse::getPosition( RenderWindow ).y;
}

//-------------------------------------------------------------------------//

void MouseCursor::DrawCursor( RenderWindow &RenderWindow )
{
    Sprite.setPosition( RectCursor.left, RectCursor.top );
    RenderWindow.draw( Sprite );
}

//-------------------------------------------------------------------------//

Vector2i MouseCursor::GetVectorCursor() const
{
    return Vector2i( RectCursor.left, RectCursor.top );
}

//-------------------------------------------------------------------------//

FloatRect& MouseCursor::GetRectCursor()
{
    return RectCursor;
}

//-------------------------------------------------------------------------//
