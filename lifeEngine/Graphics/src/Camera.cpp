#include <Graphics\BoundingBox.h>
#include "..\Camera.h"

//-------------------------------------------------------------------------//

le::Camera::Camera( System& System ) :
	InclinationCamera( 0 )
{
	RenderWindow = &System.GetWindow();
	ProjectionMatrix = &System::Configuration.ProjectionMatrix;
	SensitivityMouse = System.Configuration.SensitivityMouse;

	CenterWindow = Vector2i( NUMBER_TO_INT( System::Configuration.WindowSize.x ) / 2, NUMBER_TO_INT( System::Configuration.WindowSize.y ) / 2 );
}

//-------------------------------------------------------------------------//

le::Camera::~Camera()
{}

//-------------------------------------------------------------------------//

void le::Camera::UpdateCamera()
{
	// 1.55 радиан = 89 градусов

	MousePosition = Mouse::getPosition( *RenderWindow );

	float OffsetX = ( MousePosition.x - CenterWindow.x ) * SensitivityMouse;
	float OffsetY = ( CenterWindow.y - MousePosition.y ) * SensitivityMouse;

	Angle.x += glm::radians( OffsetX );
	Angle.y += glm::radians( OffsetY );

	if ( Angle.y < -1.55f )
		Angle.y = -1.55f;
	else if ( Angle.y > 1.55f )
		Angle.y = 1.55f;

	Direction.x = glm::cos( Angle.x ) * glm::cos( Angle.y );
	Direction.y = glm::sin( Angle.y );
	Direction.z = glm::sin( Angle.x ) * glm::cos( Angle.y );
	Direction = glm::normalize( Direction );

	CameraRight = glm::normalize( glm::cross( Direction, glm::vec3( 0, 1, 0 ) ) );

	ViewMatrix = InclinationCameraMatrix * glm::lookAt( Position, Position + Direction, glm::vec3( 0, 1, 0 ) );
	Frustum.UpdateFrustum( *ProjectionMatrix, ViewMatrix );

	Mouse::setPosition( CenterWindow, *RenderWindow );
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
		Position += Direction * MoveSpeed;
		break;

	case TypeMove::Back:
		Position -= Direction * MoveSpeed;
		break;

	case TypeMove::Left:
		Position -= CameraRight * MoveSpeed;
		break;

	case TypeMove::Right:
		Position += CameraRight * MoveSpeed;
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

glm::vec3& le::Camera::GetPosition()
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
		OffsetMove = Direction * MoveSpeed;
		break;

	case TypeMove::Back:
		OffsetMove = -Direction * MoveSpeed;
		break;

	case TypeMove::Left:
		OffsetMove = -CameraRight * MoveSpeed;
		break;

	case TypeMove::Right:
		OffsetMove = CameraRight * MoveSpeed;
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
