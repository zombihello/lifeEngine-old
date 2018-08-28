#include <Graphics\Bounding Volume\BoundingBox.h>
#include <Graphics\Camera.h>

//-------------------------------------------------------------------------//

float le::Camera::SensitivityMouse = 1.f;

//-------------------------------------------------------------------------//

le::Camera::Camera( System& System ) :
	IsUpdate( false ),
	InclinationCamera( 0 ),
	AxisVertical( 0, 1.f, 0 )
{
	RenderWindow = &System.GetWindow();
	ProjectionMatrix = &System::Configuration.ProjectionMatrix;
	CenterWindow = Vector2i( NUMBER_TO_INT( System::Configuration.WindowSize.x ) / 2, NUMBER_TO_INT( System::Configuration.WindowSize.y ) / 2 );
}

//-------------------------------------------------------------------------//

le::Camera::~Camera()
{}

//-------------------------------------------------------------------------//

void le::Camera::UpdateTargetPoint()
{
	// 1.55 радиан = 89 градусов

	MousePosition = Mouse::getPosition( *RenderWindow );

	float OffsetX = ( MousePosition.x - CenterWindow.x ) * SensitivityMouse;
	float OffsetY = ( CenterWindow.y - MousePosition.y ) * SensitivityMouse;

	Angle.x -= glm::radians( OffsetX );
	Angle.y += glm::radians( OffsetY );

	if ( Angle.y < -1.55f )
		Angle.y = -1.55f;
	else if ( Angle.y > 1.55f )
		Angle.y = 1.55f;

	if ( AxisVertical.x == 1.f )
	{
		Direction.x = glm::sin( Angle.y );
		Direction.y = glm::cos( Angle.y ) * glm::cos( Angle.x );
		Direction.z = glm::cos( Angle.y ) * glm::sin( Angle.x );
	}
	else if ( AxisVertical.y == 1.f )
	{
		Direction.x = glm::cos( Angle.y ) * glm::sin( Angle.x );
		Direction.y = glm::sin( Angle.y );
		Direction.z = glm::cos( Angle.y ) * glm::cos( Angle.x );
	}
	else
	{
		Direction.x = glm::cos( Angle.y ) * glm::cos( Angle.x );
		Direction.y = glm::cos( Angle.y ) * glm::sin( Angle.x );
		Direction.z = glm::sin( Angle.y );
	}

	Direction = glm::normalize( Direction );
	CameraRight = glm::normalize( glm::cross( Direction, glm::vec3( 0, 1, 0 ) ) );
	TargetPoint = Position + Direction;
	Mouse::setPosition( CenterWindow, *RenderWindow );

	IsUpdate = false;
}

//-------------------------------------------------------------------------//

void le::Camera::UpdateViewMatrix()
{
	if ( IsUpdate ) return;

	ViewMatrix = InclinationCameraMatrix * glm::lookAt( Position, TargetPoint, AxisVertical );
	Frustum.UpdateFrustum( *ProjectionMatrix, ViewMatrix );
	IsUpdate = true;
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

	IsUpdate = false;
}

//-------------------------------------------------------------------------//

void le::Camera::SetSensitivityMouse( float sensitivityMouse )
{
	SensitivityMouse = sensitivityMouse;
}

//-------------------------------------------------------------------------//

void le::Camera::SetAxisVertical( const glm::vec3& AxisVertical )
{
	this->AxisVertical = AxisVertical;
	IsUpdate = false;
}

//-------------------------------------------------------------------------//

void le::Camera::SetPosition( const glm::vec3& NewPosition )
{
	Position = NewPosition;
	IsUpdate = false;
}

//-------------------------------------------------------------------------//

void le::Camera::SetTargetPoint( const glm::vec3& TargetPoint )
{
	this->TargetPoint = Direction = TargetPoint;
	IsUpdate = false;
}

//-------------------------------------------------------------------------//

void le::Camera::SetInclinationCamera( float InclinationCamera )
{
	if ( InclinationCamera > 360 || InclinationCamera < -360 )
		InclinationCamera = 0;
	else
		InclinationCamera = InclinationCamera;

	InclinationCameraMatrix = glm::rotate( glm::radians( InclinationCamera ), glm::vec3( 0, 0, 1 ) );
	IsUpdate = false;
}

//-------------------------------------------------------------------------//

glm::vec3& le::Camera::GetAxisVertical()
{
	return AxisVertical;
}

//-------------------------------------------------------------------------//

glm::vec3& le::Camera::GetPosition()
{
	return Position;
}

//-------------------------------------------------------------------------//

glm::vec3& le::Camera::GetTargetPoint()
{
	return TargetPoint;
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
	if ( !IsUpdate )
	{
		ViewMatrix = InclinationCameraMatrix * glm::lookAt( Position, TargetPoint, AxisVertical );
		Frustum.UpdateFrustum( *ProjectionMatrix, ViewMatrix );
		IsUpdate = true;
	}

	return ViewMatrix;
}

//-------------------------------------------------------------------------//

le::Frustum& le::Camera::GetFrustum()
{
	return Frustum;
}

//-------------------------------------------------------------------------//
