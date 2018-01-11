#include "..\SpotLight.h"

//-------------------------------------------------------------------------//

le::SpotLight::SpotLight() :
	SpotCutoff( -1 ), // cos 180 градусов = -1
	SpotDirection( 0.f, -1.f, 0.f ),
	Radius( 25 ),
	Height( 45 )
{
	Logger::Log( Logger::Info, "Creating Spotlight" );
	Logger::Log( Logger::None, "  Cone Radius: " + to_string( 25 ) );
	Logger::Log( Logger::None, "  Cone Height: " + to_string( 45 ) );
	Logger::Log( Logger::None, "  Spot Cutoff: " + to_string( SpotCutoff ) );
	Logger::Log( Logger::None, "  StaticLight: " + to_string( IsStaticLight ) );
	Logger::Log( Logger::None, "  Intensivity: " + to_string( Intensivity ) );
	Logger::Log( Logger::None, "  Position: " + to_string( Position.x ) + " " + to_string( Position.y ) + " " + to_string( Position.z ) );
	Logger::Log( Logger::None, "  Spot Direction: " + to_string( SpotDirection.x ) + " " + to_string( SpotDirection.y ) + " " + to_string( SpotDirection.z ) );
	Logger::Log( Logger::None, "  Rotation: " + to_string( 0 ) + " " + to_string( 0 ) + " " + to_string( 0 ) );
	Logger::Log( Logger::None, "  Color: " + to_string( Color.x ) + " " + to_string( Color.y ) + " " + to_string( Color.z ) + " " + to_string( Color.w ) );
	Logger::Log( Logger::None, "  Specular: " + to_string( Specular.x ) + " " + to_string( Specular.y ) + " " + to_string( Specular.z ) + " " + to_string( Specular.w ) );

	LightCone.InitCone( 45, 25 );
	LightCone.SetPosition( Position );
	InitShadowMap();

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
	Logger::Log( Logger::None, "  StaticLight: " + to_string( IsStaticLight ) );
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
	InitShadowMap();

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
}

//-------------------------------------------------------------------------//

void le::SpotLight::SetHeight( float Height )
{
	LightCone.SetHeight( Height );
	this->Height = Height;
}

//-------------------------------------------------------------------------//

void le::SpotLight::SetRotation( const glm::vec3& Rotation )
{
	SpotDirection = Rotation;
	SpotDirection = glm::normalize( SpotDirection );

	LightCone.SetRotation( Rotation );
}

//-------------------------------------------------------------------------//

void le::SpotLight::SetRotation( const glm::quat& Rotation )
{
	SpotDirection = glm::eulerAngles( Rotation );
	SpotDirection = glm::normalize( SpotDirection );

	LightCone.SetRotation( Rotation );
}

//-------------------------------------------------------------------------//

void le::SpotLight::SetPosition( const glm::vec3& Position )
{
	this->Position = glm::vec4( Position, 1.0f );
	LightCone.SetPosition( Position );
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

	return *this;
}

//-------------------------------------------------------------------------//
