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

void le::Light::CreateLight( Vector2f Position, float Radius, Color Color, string NameLight )
{
	CircleShape.setPosition( Position );
	CircleShape.setFillColor( Color );
	CircleShape.setRadius( Radius );
	CircleShape.setOrigin( Radius, Radius );
	sNameLight = NameLight;

	Rect = FloatRect( Position.x - Radius, Position.y - Radius, Radius*2, Radius*2  );
}

//-------------------------------------------------------------------------//

void le::Light::SetRadius( float Radius )
{
	CircleShape.setRadius( Radius );
	CircleShape.setOrigin( Radius, Radius );

	Rect = FloatRect( Rect.left - Radius, Rect.top - Radius, Radius*2, Radius*2  );
}

//-------------------------------------------------------------------------//

void le::Light::SetPosition( Vector2f Position )
{
	CircleShape.setPosition( Position );

	Rect.left = Position.x - CircleShape.getRadius();
	Rect.top = Position.y - CircleShape.getRadius();
}

//-------------------------------------------------------------------------//

void le::Light::SetColor( sf::Color Color )
{
	CircleShape.setFillColor( Color );
}

//-------------------------------------------------------------------------//

sf::CircleShape le::Light::GetCircleShape()
{
	return CircleShape;
}

//-------------------------------------------------------------------------//

float le::Light::GetRadius()
{
	return CircleShape.getRadius();
}

//-------------------------------------------------------------------------//

Vector2f le::Light::GetPosition()
{
	return CircleShape.getPosition();
}

//-------------------------------------------------------------------------//

Color le::Light::GetColor()
{
	return CircleShape.getFillColor();
}

//-------------------------------------------------------------------------//

FloatRect le::Light::GetRect()
{
	return Rect;
}

//-------------------------------------------------------------------------//

string le::Light::GetNameLight()
{
	return sNameLight;
}

//-------------------------------------------------------------------------//
