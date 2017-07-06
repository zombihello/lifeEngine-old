#include "..\..\3D\Camera.h"

#define PI 3.141592653

//-------------------------------------------------------------------------//

le::Camera::Camera( le::System& System )
{
	this->System = &System;
	fSpeed = 1;
	fSensitivityMouse = 4;
	CenterWindow = Vector2f( System.GetWindow( ).getSize( ).x / 2, System.GetWindow( ).getSize( ).y / 2 );
}

//-------------------------------------------------------------------------//

le::Camera::~Camera()
{
}

//-------------------------------------------------------------------------//

void le::Camera::SetSensitivityMouse( float sensitivityMouse )
{
	fSensitivityMouse = sensitivityMouse;
}

//-------------------------------------------------------------------------//

void le::Camera::UpdateCamera()
{
	Move();

	Vector2i MousePosition = Mouse::getPosition( System->GetWindow( ) );

	Angle.x += ( CenterWindow.x - MousePosition.x ) / fSensitivityMouse;
	Angle.y += ( CenterWindow.y - MousePosition.y ) / fSensitivityMouse;

	if ( Angle.y < -89.0 )
		Angle.y = -89.0;
	else if ( Angle.y > 89.0 )
		Angle.y = 89.0;
	
	Mouse::setPosition( Vector2i( CenterWindow.x, CenterWindow.y ), System->GetWindow( ) );

	gluLookAt( Position.x, Position.y, Position.z, Position.x - sin( Angle.x / 180 * PI ), Position.y + tan( Angle.y / 180 * PI ), Position.z - cos( Angle.x / 180 * PI ), 0, 1, 0 );
}

//-------------------------------------------------------------------------//

void le::Camera::Move()
{
	if ( Keyboard::isKeyPressed( Keyboard::W ) )
	{
		Position.x -= ( float ) sin( Angle.x / 180 * PI ) * fSpeed;
		Position.y += ( float ) tan( Angle.y / 180 * PI ) * fSpeed;
		Position.z -= ( float ) cos( Angle.x / 180 * PI ) * fSpeed;
	}

	if ( Keyboard::isKeyPressed( Keyboard::S ) )
	{
		Position.x += ( float ) sin( Angle.x / 180 * PI ) * fSpeed;
		Position.y -= ( float ) tan( Angle.y / 180 * PI ) * fSpeed;
		Position.z += ( float ) cos( Angle.x / 180 * PI ) * fSpeed;
	}

	if ( Keyboard::isKeyPressed( Keyboard::A ) )
	{
		Position.x += ( float ) sin( ( Angle.x - 90 ) / 180 * PI ) * fSpeed;
		Position.z += ( float ) cos( ( Angle.x - 90 ) / 180 * PI ) * fSpeed;
	}

	if ( Keyboard::isKeyPressed( Keyboard::D ) )
	{
		Position.x += ( float ) sin( ( Angle.x + 90 ) / 180 * PI ) * fSpeed;
		Position.z += ( float ) cos( ( Angle.x + 90 ) / 180 * PI ) * fSpeed;
	}
}

//-------------------------------------------------------------------------//

void le::Camera::SetPosition( Vector3f NewPosition )
{
	Position = NewPosition;
}

//-------------------------------------------------------------------------//

Vector3f le::Camera::GetPosition()
{
	return Position;
}

//-------------------------------------------------------------------------//