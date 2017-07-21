#include "..\..\3D\Camera.h"

#define PI 3.141592653

//-------------------------------------------------------------------------//

le::Camera::Camera( le::System& System )
{
	this->System = &System;
	fSensitivityMouse = 4;
	fInclinationCamera = 0;
	CenterWindow = Vector2f( System.GetWindow().getSize().x / 2, System.GetWindow().getSize().y / 2 );
}

//-------------------------------------------------------------------------//

le::Camera::~Camera()
{}

//-------------------------------------------------------------------------//

void le::Camera::SetSensitivityMouse( float sensitivityMouse )
{
	fSensitivityMouse = sensitivityMouse;
}

//-------------------------------------------------------------------------//

void le::Camera::UpdateCamera()
{
	// 1.55 радиан = 89 градусов

	Vector2i MousePosition = Mouse::getPosition( System->GetWindow() );

	Angle.x += ( ( CenterWindow.x - MousePosition.x ) / fSensitivityMouse ) / 180 * PI;
	Angle.y += ( ( CenterWindow.y - MousePosition.y ) / fSensitivityMouse ) / 180 * PI;

	if ( Angle.y < -1.55 )
		Angle.y = -1.55;
	else if ( Angle.y > 1.55 )
		Angle.y = 1.55;

	Mouse::setPosition( Vector2i( CenterWindow.x, CenterWindow.y ), System->GetWindow() );

	ViewMatrix = InclinationCameraMatrix * glm::lookAt(
		glm::vec3( Position.x, Position.y, Position.z ),
		glm::vec3( Position.x - sin( Angle.x ), Position.y + tan( Angle.y ), Position.z - cos( Angle.x ) ),
		glm::vec3( 0, 1, 0 ) );
}

//-------------------------------------------------------------------------//

void le::Camera::ApplyCamera()
{
	ViewMatrix = InclinationCameraMatrix * glm::lookAt(
		glm::vec3( Position.x, Position.y, Position.z ),
		glm::vec3( Position.x - sin( Angle.x ), Position.y + tan( Angle.y ), Position.z - cos( Angle.x ) ),
		glm::vec3( 0, 1, 0 ) );

}

//-------------------------------------------------------------------------//

void le::Camera::Move( Vector3f FactorMove )
{
	Position.x += FactorMove.x;
	Position.y += FactorMove.y;
	Position.z += FactorMove.z;
}

//-------------------------------------------------------------------------//

void le::Camera::Move( TypeMove typeMove, float MoveSpeed )
{
	// 1.5707 радиан = 90 градусов

	switch ( typeMove )
	{
	case TypeMove::Forward:
		Position.x -= ( float ) sin( Angle.x ) * MoveSpeed;
		Position.y += ( float ) tan( Angle.y ) * MoveSpeed;
		Position.z -= ( float ) cos( Angle.x ) * MoveSpeed;
		break;

	case TypeMove::Back:
		Position.x += ( float ) sin( Angle.x ) * MoveSpeed;
		Position.y -= ( float ) tan( Angle.y ) * MoveSpeed;
		Position.z += ( float ) cos( Angle.x ) * MoveSpeed;
		break;

	case TypeMove::Left:
		Position.x += ( float ) sin( ( Angle.x - 1.5707 ) ) * MoveSpeed;
		Position.z += ( float ) cos( ( Angle.x - 1.5707 ) ) * MoveSpeed;
		break;

	case TypeMove::Right:
		Position.x += ( float ) sin( ( Angle.x + 1.5707 ) ) * MoveSpeed;
		Position.z += ( float ) cos( ( Angle.x + 1.5707 ) ) * MoveSpeed;
		break;
	}
}

//-------------------------------------------------------------------------//

void le::Camera::TiltCamera( float FactorTilt )
{
	fInclinationCamera += FactorTilt;
	InclinationCameraMatrix *= glm::rotate( glm::radians( FactorTilt ), glm::vec3( 0, 0, 1 ) );

	if ( fInclinationCamera > 360 || fInclinationCamera < -360 )
		fInclinationCamera = 0;
}

//-------------------------------------------------------------------------//

void le::Camera::SetPosition( Vector3f NewPosition )
{
	Position = NewPosition;
}

//-------------------------------------------------------------------------//

void le::Camera::SetInclinationCamera( float InclinationCamera )
{
	if ( InclinationCamera > 360 || InclinationCamera < -360 )
		fInclinationCamera = 0;
	else
		fInclinationCamera = InclinationCamera;

	InclinationCameraMatrix = glm::rotate( glm::radians( fInclinationCamera ), glm::vec3( 0, 0, 1 ) );
}

//-------------------------------------------------------------------------//

Vector3f le::Camera::GetPosition()
{
	return Position;
}

//-------------------------------------------------------------------------//

float le::Camera::GetInclinationCamera()
{
	return fInclinationCamera;
}

//-------------------------------------------------------------------------//

Vector3f le::Camera::GetVectorMove( TypeMove typeMove, float MoveSpeed )
{
	// 1.5707 радиан = 90 градусов

	Vector3f OffsetMove;

	switch ( typeMove )
	{
	case TypeMove::Forward:
		OffsetMove.x = -( float ) sin( Angle.x ) * MoveSpeed;
		OffsetMove.y = ( float ) tan( Angle.y ) * MoveSpeed;
		OffsetMove.z = -( float ) cos( Angle.x ) * MoveSpeed;
		break;

	case TypeMove::Back:
		OffsetMove.x = ( float ) sin( Angle.x ) * MoveSpeed;
		OffsetMove.y = -( float ) tan( Angle.y ) * MoveSpeed;
		OffsetMove.z = ( float ) cos( Angle.x ) * MoveSpeed;
		break;

	case TypeMove::Left:
		OffsetMove.x = ( float ) sin( ( Angle.x - 1.5707 ) ) * MoveSpeed;
		OffsetMove.z = ( float ) cos( ( Angle.x - 1.5707 ) ) * MoveSpeed;
		break;

	case TypeMove::Right:
		OffsetMove.x = ( float ) sin( ( Angle.x + 1.5707 ) ) * MoveSpeed;
		OffsetMove.z = ( float ) cos( ( Angle.x + 1.5707 ) ) * MoveSpeed;
		break;
	}

	return OffsetMove;
}

//-------------------------------------------------------------------------//

glm::mat4 le::Camera::GetViewMatrix()
{
	return ViewMatrix;
}

//-------------------------------------------------------------------------//