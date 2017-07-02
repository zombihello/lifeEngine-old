#include "../CircleShape2D.h"

//-------------------------------------------------------------------------//

le::CircleShape2D::CircleShape2D( float Radius )
{
	Circle.m_radius = Radius/30.f;

	fRadius = Radius;
	Center = Vector2f( 0,0 );
	fDef.shape = &Circle;
	Fixture = NULL;
}

//-------------------------------------------------------------------------//

le::CircleShape2D::CircleShape2D( float Radius, Vector2f Center )
{
	Circle.m_radius = Radius/30.f;
	Circle.m_p.Set( Center.x/30.f, Center.y/30.f );

	fRadius = Radius;
	this->Center = Center;
	fDef.shape = &Circle;
	Fixture = NULL;
}

//-------------------------------------------------------------------------//

void le::CircleShape2D::SetRadius( float Radius )
{
	fRadius += Radius - fRadius;

	Circle.m_radius = fRadius/30.f;
}

//-------------------------------------------------------------------------//

void le::CircleShape2D::SetPropirtes( float Density, float Restitution, float Friction )
{
	fDef.density = Density;
	fDef.restitution = Restitution;
	fDef.friction = Friction;
}

//-------------------------------------------------------------------------//

void le::CircleShape2D::SetFixture( b2Fixture *Fixture )
{
	this->Fixture = Fixture;
}

//-------------------------------------------------------------------------//

b2FixtureDef& le::CircleShape2D::GetFixtureDef( )
{
	return fDef;
}

//-------------------------------------------------------------------------//

b2Fixture& le::CircleShape2D::GetFixture( )
{
	return *Fixture;
}

//-------------------------------------------------------------------------//
