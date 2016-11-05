#include "../MouseCursor.h"
using namespace le;
#include <iostream>
//-------------------------------------------------------------------------//

MouseCursor::MouseCursor()
{

}

//-------------------------------------------------------------------------//

void MouseCursor::LoadTexture( const string sRoute , bool bSmooth )
{
	Texture.loadFromFile( sRoute );
	Texture.setSmooth( bSmooth );

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
	for ( int i = 0; i < vButtonsPressed.size(); i++ )
		if ( !Mouse::isButtonPressed( vButtonsPressed[ i ] ) )
			vButtonsPressed.erase( vButtonsPressed.begin() + i );

	Sprite.setPosition( RectCursor.left, RectCursor.top );
	RenderWindow.draw( Sprite );

	if ( Mouse::isButtonPressed( Mouse::Left ) )
		vButtonsPressed.push_back( Mouse::Left );

	if ( Mouse::isButtonPressed( Mouse::Right ) )
		vButtonsPressed.push_back( Mouse::Right );

	if ( Mouse::isButtonPressed( Mouse::Middle ) )
		vButtonsPressed.push_back( Mouse::Middle );
}

//-------------------------------------------------------------------------//

bool MouseCursor::ButtonPressed( Mouse::Button Button )
{
	if ( Mouse::isButtonPressed( Button ) )
	{
		for ( int i = 0; i < vButtonsPressed.size(); i++ )
			if ( vButtonsPressed[ i ] == Button )
				return false;
		return true;
	}

	return false;
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
