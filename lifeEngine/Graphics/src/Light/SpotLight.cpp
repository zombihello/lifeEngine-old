#include <System\System.h>
#include <Graphics\Light\SpotLight.h>

//-------------------------------------------------------------------------//

le::SpotLight::SpotLight() :
	SpotCutoff( -1 ), // cos 180 градусов = -1
	SpotDirection( 0.f, -1.f, 0.f ),
	Radius( 25 ),
	Height( 45 )
{
	Logger::Log( Logger::Info, "Creating Spotlight" );
	Logger::Log( Logger::None, "  Cone Radius: " + to_string( Radius ) );
	Logger::Log( Logger::None, "  Cone Height: " + to_string( Height ) );
	Logger::Log( Logger::None, "  Spot Cutoff: " + to_string( SpotCutoff ) );
	Logger::Log( Logger::None, "  Intensivity: " + to_string( Intensivity ) );
	Logger::Log( Logger::None, "  Position: " + to_string( Position.x ) + " " + to_string( Position.y ) + " " + to_string( Position.z ) );
	Logger::Log( Logger::None, "  Spot Direction: " + to_string( SpotDirection.x ) + " " + to_string( SpotDirection.y ) + " " + to_string( SpotDirection.z ) );
	Logger::Log( Logger::None, "  Rotation: " + to_string( 0 ) + " " + to_string( 0 ) + " " + to_string( 0 ) );
	Logger::Log( Logger::None, "  Color: " + to_string( Color.x * 255.f ) + " " + to_string( Color.y * 255.f ) + " " + to_string( Color.z * 255.f ) + " " + to_string( Color.w * 255.f ) );
	Logger::Log( Logger::None, "  Specular: " + to_string( Specular.x  * 255.f ) + " " + to_string( Specular.y * 255.f ) + " " + to_string( Specular.z * 255.f ) + " " + to_string( Specular.w * 255.f ) );

	LightCone.InitCone( Height, Radius );
	LightCone.SetPosition( Position );

	glm::vec3 TempPosition( Position );
	Center = TempPosition + SpotDirection;
	NormalizeCenter = glm::normalize( Center );

	Right = glm::normalize( glm::cross( glm::vec3( 0, 1, 0 ), NormalizeCenter ) );
	Up = glm::normalize( glm::cross( NormalizeCenter, Right ) );

	LightProjection = glm::perspective( glm::acos( SpotCutoff ) * 2, 1.f, 1.f, Height );
	LightViews.push_back( glm::lookAt( TempPosition, Center, Up ) );
	LightTransforms.push_back( LightProjection * LightViews[ 0 ] );

	Frustums.push_back( Frustum() );
	Frustums[ 0 ].UpdateFrustum( LightProjection, LightViews[ 0 ] );

	Logger::Log( Logger::Info, "Created Spotlight" );
}

//-------------------------------------------------------------------------//

le::SpotLight::SpotLight( const SpotLight& Copy )
{
	CopyBaseLight( Copy );

	SpotCutoff = Copy.SpotCutoff;
	SpotDirection = Copy.SpotDirection;
	LightCone = Copy.LightCone;
	Radius = Copy.Radius;
	Height = Copy.Height;
	Rotation = Copy.Rotation;
	Center = Copy.Center;
	NormalizeCenter = Copy.NormalizeCenter;
	Right = Copy.Right;
	Up = Copy.Up;
}

//-------------------------------------------------------------------------//

le::SpotLight::SpotLight( float Radius, float Height, const glm::vec3& Rotation, const glm::vec3& Position, const glm::vec4& Color, float Intensivity, const glm::vec4& Specular ) :
	SpotCutoff( -1 ), // cos 180 градусов = -1
	SpotDirection( 0.f, -1.f, 0.f ),
	Radius( Radius ),
	Height( Height )
{
	float C = sqrt( pow( Height, 2 ) + pow( Radius, 2 ) );
	SpotCutoff = Height / C;

	glm::vec3 Axis( sin( Rotation.x / 2 ), sin( Rotation.y / 2 ), sin( Rotation.z / 2 ) );
	glm::vec3 Rotations( cos( Rotation.x / 2 ), cos( Rotation.y / 2 ), cos( Rotation.z / 2 ) );

	glm::quat RotateX( Rotations.x, Axis.x, 0, 0 );
	glm::quat RotateY( Rotations.y, 0, Axis.y, 0 );
	glm::quat RotateZ( Rotations.z, 0, 0, Axis.z );

	glm::quat QuatRotation = RotateX * RotateY * RotateZ;

	SpotDirection = QuatRotation * SpotDirection;
	SpotDirection = glm::normalize( SpotDirection );

	Logger::Log( Logger::Info, "Creating Spotlight" );
	Logger::Log( Logger::None, "  Cone Radius: " + to_string( Radius ) );
	Logger::Log( Logger::None, "  Cone Height: " + to_string( Height ) );
	Logger::Log( Logger::None, "  Spot Cutoff: " + to_string( SpotCutoff ) );
	Logger::Log( Logger::None, "  Intensivity: " + to_string( Intensivity ) );
	Logger::Log( Logger::None, "  Position: " + to_string( Position.x ) + " " + to_string( Position.y ) + " " + to_string( Position.z ) );
	Logger::Log( Logger::None, "  Spot Direction: " + to_string( SpotDirection.x ) + " " + to_string( SpotDirection.y ) + " " + to_string( SpotDirection.z ) );
	Logger::Log( Logger::None, "  Rotation: " + to_string( Rotation.x ) + " " + to_string( Rotation.y ) + " " + to_string( Rotation.z ) );
	Logger::Log( Logger::None, "  Color: " + to_string( Color.x ) + " " + to_string( Color.y ) + " " + to_string( Color.z ) + " " + to_string( Color.w ) );
	Logger::Log( Logger::None, "  Specular: " + to_string( Specular.x ) + " " + to_string( Specular.y ) + " " + to_string( Specular.z ) + " " + to_string( Specular.w ) );

	this->Position = glm::vec4( Position, 1.0f );
	this->Rotation = Rotation;
	this->Color = Color / 255.f;
	this->Specular = Specular / 255.f;
	this->Intensivity = Intensivity;

	LightCone.InitCone( Height, Radius );
	LightCone.SetPosition( Position );
	LightCone.SetRotation( QuatRotation );

	Center = Position + SpotDirection;
	NormalizeCenter = glm::normalize( Center );

	Right = glm::normalize( glm::cross( glm::vec3( 0, 1, 0 ), NormalizeCenter ) );
	Up = glm::normalize( glm::cross( NormalizeCenter, Right ) );

	LightProjection = glm::perspective( glm::acos( SpotCutoff ) * 2, 1.f, 1.f, Height );
	LightViews.push_back( glm::lookAt( Position, Center, Up ) );
	LightTransforms.push_back( LightProjection * LightViews[ 0 ] );

	Frustums.push_back( Frustum() );
	Frustums[ 0 ].UpdateFrustum( LightProjection, LightViews[ 0 ] );

	Logger::Log( Logger::Info, "Created Spotlight" );
}

//-------------------------------------------------------------------------//

le::SpotLight::~SpotLight()
{}

//-------------------------------------------------------------------------//

void le::SpotLight::SetRadius( float Radius )
{
	LightCone.SetRadius( Radius );
	this->Radius = Radius;

	float C = sqrt( pow( Height, 2 ) + pow( Radius, 2 ) );
	SpotCutoff = Height / C;

	glm::vec3 TempPosition( Position );
	LightProjection = glm::perspective( glm::acos( SpotCutoff ) * 2, 1.f, 1.f, Height );

	LightViews[ 0 ] = glm::lookAt( TempPosition, Center, Up );
	LightTransforms[ 0 ] = LightProjection * LightViews[ 0 ];
	Frustums[ 0 ].UpdateFrustum( LightProjection, LightViews[ 0 ] );
}

//-------------------------------------------------------------------------//

void le::SpotLight::SetHeight( float Height )
{
	LightCone.SetHeight( Height );
	this->Height = Height;

	float C = sqrt( pow( Height, 2 ) + pow( Radius, 2 ) );
	SpotCutoff = Height / C;

	glm::vec3 TempPosition( Position );
	LightProjection = glm::perspective( glm::acos( SpotCutoff ) * 2, 1.f, 1.f, Height );

	LightViews[ 0 ] = glm::lookAt( TempPosition, Center, Up );
	LightTransforms[ 0 ] = LightProjection * LightViews[ 0 ];
	Frustums[ 0 ].UpdateFrustum( LightProjection, LightViews[ 0 ] );
}

//-------------------------------------------------------------------------//

void le::SpotLight::SetRotation( const glm::vec3& Rotation )
{
	SpotDirection = Rotation;
	SpotDirection = glm::normalize( SpotDirection );

	LightCone.SetRotation( SpotDirection );

	glm::vec3 TempPosition( Position );
	Center = TempPosition + SpotDirection;
	NormalizeCenter = glm::normalize( Center );

	Right = glm::normalize( glm::cross( glm::vec3( 0, 1, 0 ), NormalizeCenter ) );
	Up = glm::normalize( glm::cross( NormalizeCenter, Right ) );

	LightViews[ 0 ] = glm::lookAt( TempPosition, Center, Up );
	LightTransforms[ 0 ] = LightProjection * LightViews[ 0 ];
	Frustums[ 0 ].UpdateFrustum( LightProjection, LightViews[ 0 ] );
}

//-------------------------------------------------------------------------//

void le::SpotLight::SetRotation( const glm::quat& Rotation )
{
	SpotDirection = glm::eulerAngles( Rotation );
	SpotDirection = glm::normalize( SpotDirection );

	LightCone.SetRotation( Rotation );

	glm::vec3 TempPosition( Position );
	Center = TempPosition + SpotDirection;
	NormalizeCenter = glm::normalize( Center );

	Right = glm::normalize( glm::cross( glm::vec3( 0, 1, 0 ), NormalizeCenter ) );
	Up = glm::normalize( glm::cross( NormalizeCenter, Right ) );

	LightViews[ 0 ] = glm::lookAt( TempPosition, Center, Up );
	LightTransforms[ 0 ] = LightProjection * LightViews[ 0 ];
	Frustums[ 0 ].UpdateFrustum( LightProjection, LightViews[ 0 ] );
}

//-------------------------------------------------------------------------//

void le::SpotLight::SetPosition( const glm::vec3& Position )
{
	this->Position = glm::vec4( Position, 1.0f );
	LightCone.SetPosition( Position );

	Center = Position + SpotDirection;
	NormalizeCenter = glm::normalize( Center );

	Right = glm::normalize( glm::cross( glm::vec3( 0, 1, 0 ), NormalizeCenter ) );
	Up = glm::normalize( glm::cross( NormalizeCenter, Right ) );

	LightViews[ 0 ] = glm::lookAt( Position, Center, Up );
	LightTransforms[ 0 ] = LightProjection * LightViews[ 0 ];
	Frustums[ 0 ].UpdateFrustum( LightProjection, LightViews[ 0 ] );
}

//-------------------------------------------------------------------------//

le::SpotLight& le::SpotLight::operator=( const SpotLight& Copy )
{
	CopyBaseLight( Copy );

	SpotCutoff = Copy.SpotCutoff;
	SpotDirection = Copy.SpotDirection;
	LightCone = Copy.LightCone;
	Radius = Copy.Radius;
	Height = Copy.Height;
	Rotation = Copy.Rotation;
	Center = Copy.Center;
	NormalizeCenter = Copy.NormalizeCenter;
	Right = Copy.Right;
	Up = Copy.Up;

	return *this;
}

//-------------------------------------------------------------------------//
