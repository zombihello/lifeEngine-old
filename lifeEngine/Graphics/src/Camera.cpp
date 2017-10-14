#include <Graphics\BoundingBox.h>
#include "..\Camera.h"

#define PI 3.141592653f

//-------------------------------------------------------------------------//

le::Camera::Camera( System& System ) :
	SensitivityMouse( 4 ),
	InclinationCamera( 0 )
{
	RenderWindow = &System.GetWindow();
	ProjectionMatrix = &System.GetConfiguration().ProjectionMatrix;
}

//-------------------------------------------------------------------------//

le::Camera::~Camera()
{}

//-------------------------------------------------------------------------//

void le::Camera::UpdateCamera()
{
	// 1.55 радиан = 89 градусов

	MousePosition = Mouse::getPosition( *RenderWindow );
	SizeWindow = RenderWindow->getSize();
	CenterWindow.x = NUMBER_TO_FLOAT( SizeWindow.x / 2 );
	CenterWindow.y = NUMBER_TO_FLOAT( SizeWindow.y / 2 );

	Angle.x += ( ( CenterWindow.x - MousePosition.x ) / SensitivityMouse ) / 180 * PI;
	Angle.y += ( ( CenterWindow.y - MousePosition.y ) / SensitivityMouse ) / 180 * PI;

	if ( Angle.y < -1.55f )
		Angle.y = -1.55f;
	else if ( Angle.y > 1.55f )
		Angle.y = 1.55f;

	Mouse::setPosition( Vector2i( NUMBER_TO_INT( CenterWindow.x ), NUMBER_TO_INT( CenterWindow.y ) ), *RenderWindow );

	ViewMatrix = InclinationCameraMatrix * glm::lookAt(
		glm::vec3( Position.x, Position.y, Position.z ),
		glm::vec3( Position.x - sin( Angle.x ), Position.y + tan( Angle.y ), Position.z - cos( Angle.x ) ),
		glm::vec3( 0, 1, 0 ) );

	Frustum.UpdateFrustum( *ProjectionMatrix, ViewMatrix );
}

//-------------------------------------------------------------------------//

void le::Camera::Move( const glm::vec3& FactorMove )
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
	InclinationCamera += FactorTilt;
	InclinationCameraMatrix *= glm::rotate( glm::radians( FactorTilt ), glm::vec3( 0, 0, 1 ) );

	if ( InclinationCamera > 360 || InclinationCamera < -360 )
		InclinationCamera = 0;
}

//-------------------------------------------------------------------------//

void le::Camera::SetSensitivityMouse( float sensitivityMouse )
{
	SensitivityMouse = sensitivityMouse;
}

//-------------------------------------------------------------------------//

void le::Camera::SetPosition( const glm::vec3& NewPosition )
{
	Position = NewPosition;
}

//-------------------------------------------------------------------------//

void le::Camera::SetInclinationCamera( float InclinationCamera )
{
	if ( InclinationCamera > 360 || InclinationCamera < -360 )
		InclinationCamera = 0;
	else
		InclinationCamera = InclinationCamera;

	InclinationCameraMatrix = glm::rotate( glm::radians( InclinationCamera ), glm::vec3( 0, 0, 1 ) );
}

//-------------------------------------------------------------------------//

const glm::vec3& le::Camera::GetPosition()
{
	return Position;
}

//-------------------------------------------------------------------------//

float le::Camera::GetInclinationCamera()
{
	return InclinationCamera;
}

//-------------------------------------------------------------------------//

inline float le::Camera::GetDistance( const glm::vec3& PositionObject )
{
	return glm::distance( Position, PositionObject );
}

//-------------------------------------------------------------------------//

glm::vec3 le::Camera::GetVectorMove( TypeMove typeMove, float MoveSpeed )
{
	// 1.5707 радиан = 90 градусов

	glm::vec3 OffsetMove;

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

glm::mat4& le::Camera::GetViewMatrix()
{
	return ViewMatrix;
}

//-------------------------------------------------------------------------//

le::Frustum& le::Camera::GetFrustum()
{
	return Frustum;
}

//-------------------------------------------------------------------------//
