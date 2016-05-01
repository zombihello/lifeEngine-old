#include "../Light.h"

//-------------------------------------------------------------------------//

void le::Light::SetMask( sf::Texture& Texture )
{
	this->Texture = Texture;
	CircleShape.setTexture( &this->Texture );
}

//-------------------------------------------------------------------------//

void le::Light::CreateLight( Vector2f Position, float Radius, Color Color )
{
	CircleShape.setPosition( Position );
	CircleShape.setFillColor( Color );
	CircleShape.setRadius( Radius );
	CircleShape.setOrigin( Radius, Radius );

	Rect = FloatRect( Position.x - Radius, Position.y - Radius, Radius*2, Radius*2  );
}

//-------------------------------------------------------------------------//

sf::CircleShape le::Light::GetLight()
{
	return CircleShape;
}

//-------------------------------------------------------------------------//

FloatRect le::Light::GetRect()
{
	return Rect;
}

//-------------------------------------------------------------------------//
