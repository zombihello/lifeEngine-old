#include "Light.h"



le::Light::Light()
{
}


le::Light::~Light()
{
}

void le::Light::SetMask( sf::Texture & Texture )
{
    this->Texture = Texture;
    CircleShape.setTexture( &this->Texture );
}

void le::Light::CreateLight( Vector2f PositionLight , const float fRadius , Color ColorLight )
{
    CircleShape.setPosition( PositionLight );
    CircleShape.setFillColor( ColorLight );
    CircleShape.setRadius( fRadius );
    CircleShape.setOrigin( fRadius , fRadius );

    Rect = FloatRect( PositionLight.x - fRadius, PositionLight.y - fRadius , fRadius*2 , fRadius*2 );
}

CircleShape le::Light::GetLight() const
{
    return CircleShape;
}

FloatRect le::Light::GetRect() const
{
    return Rect;
}
