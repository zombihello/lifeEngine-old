#include "../CircleShape.h"

//-------------------------------------------------------------------------//

le::CircleShape::CircleShape( float Radius )
{
	Circle.m_radius = Radius/30.f;

	fRadius = Radius;
	Center = Vector2f( 0,0 );
	fDef.shape = &Circle;
	Fixture = NULL;
}

//-------------------------------------------------------------------------//

le::CircleShape::CircleShape( float Radius, Vector2f Center )
{
	Circle.m_radius = Radius/30.f;
	Circle.m_p.Set( Center.x/30.f, Center.y/30.f );

	fRadius = Radius;
	this->Center = Center;
	fDef.shape = &Circle;
	Fixture = NULL;
}

//-------------------------------------------------------------------------//

void le::CircleShape::SetRadius( float Radius )
{
	fRadius += Radius - fRadius;

	Circle.m_radius = fRadius/30.f;
}

//-------------------------------------------------------------------------//

void le::CircleShape::SetPropirtes( float Density, float Restitution, float Friction )
{
	fDef.density = Density;
	fDef.restitution = Restitution;
	fDef.friction = Friction;
}

//-------------------------------------------------------------------------//

void le::CircleShape::SetFixture( b2Fixture *Fixture )
{
	this->Fixture = Fixture;
}

//-------------------------------------------------------------------------//

b2FixtureDef &le::CircleShape::GetFixtureDef()
{
	return fDef;
}

//-------------------------------------------------------------------------//

b2Fixture &le::CircleShape::GetFixture()
{
	return *Fixture;
}

//-------------------------------------------------------------------------//
