#include "..\SpotLight.h"

//-------------------------------------------------------------------------//

le::SpotLight::SpotLight() :
	SpotExponent( 0 ),
	SpotCutoff( -1 ) // cos 180 градусов = -1
{
	Logger::Log( Logger::Info, "Creating Spotlight" );
	Logger::Log( Logger::None, "  Cone Radius: " + to_string( 25 ) );
	Logger::Log( Logger::None, "  Cone Height: " + to_string( 45 ) );
	Logger::Log( Logger::None, "  Spot Exponent: " + to_string( SpotExponent ) );
	Logger::Log( Logger::None, "  Spot Cutoff: " + to_string( SpotCutoff ) );
	Logger::Log( Logger::None, "  StaticLight: " + to_string( IsStaticLight ) );
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

	SpotExponent = Copy.SpotExponent;
	SpotCutoff = Copy.SpotCutoff;
	SpotDirection = Copy.SpotDirection;
	LightCone = Copy.LightCone;
}

//-------------------------------------------------------------------------//

le::SpotLight::SpotLight( float Radius, float Height, float SpotExponent, const glm::vec3& Rotation, const glm::vec3& Position, const glm::vec4& Color, const glm::vec4& Specular )
{
	this->SpotExponent = SpotExponent;
	SpotCutoff = cos( Radius );
	SpotDirection = glm::vec3( Position.x - sin( Rotation.x ), Position.y + tan( Rotation.y ), Position.z - cos( Rotation.z ) );
	//SpotDirection = glm::normalize(SpotDirection);

	Logger::Log( Logger::Info, "Creating Spotlight" );
	Logger::Log( Logger::None, "  Cone Radius: " + to_string( Radius ) );
	Logger::Log( Logger::None, "  Cone Height: " + to_string( Height ) );
	Logger::Log( Logger::None, "  Spot Exponent: " + to_string( SpotExponent ) );
	Logger::Log( Logger::None, "  Spot Cutoff: " + to_string( SpotCutoff ) );
	Logger::Log( Logger::None, "  StaticLight: " + to_string( IsStaticLight ) );
	Logger::Log( Logger::None, "  Position: " + to_string( Position.x ) + " " + to_string( Position.y ) + " " + to_string( Position.z ) );
	Logger::Log( Logger::None, "  Spot Direction: " + to_string( SpotDirection.x ) + " " + to_string( SpotDirection.y ) + " " + to_string( SpotDirection.z ) );
	Logger::Log( Logger::None, "  Rotation: " + to_string( Rotation.x ) + " " + to_string( Rotation.y ) + " " + to_string( Rotation.z ) );
	Logger::Log( Logger::None, "  Color: " + to_string( Color.x ) + " " + to_string( Color.y ) + " " + to_string( Color.z ) + " " + to_string( Color.w ) );
	Logger::Log( Logger::None, "  Specular: " + to_string( Specular.x ) + " " + to_string( Specular.y ) + " " + to_string( Specular.z ) + " " + to_string( Specular.w ) );

	this->Position = glm::vec4( Position, 1.0f );
	this->Color = Color / 255.f;
	this->Specular = Specular / 255.f;

	LightCone.InitCone( Height, Radius );
	LightCone.SetPosition( this->Position );
	LightCone.SetRotation( Rotation );
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
}

//-------------------------------------------------------------------------//

void le::SpotLight::SetHeight( float Height )
{
	LightCone.SetHeight( Height );
}

//-------------------------------------------------------------------------//

void le::SpotLight::SetRotation( const glm::vec3& Rotation )
{
	SpotDirection = glm::vec3( Rotation.x, Rotation.y, Rotation.z );
	LightCone.SetRotation( Rotation );
}

//-------------------------------------------------------------------------//

void le::SpotLight::SetRotation( const glm::quat& Rotation )
{
	SpotDirection = glm::eulerAngles( Rotation );
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

	SpotExponent = Copy.SpotExponent;
	SpotCutoff = Copy.SpotCutoff;
	SpotDirection = Copy.SpotDirection;
	LightCone = Copy.LightCone;

	return *this;
}

//-------------------------------------------------------------------------//
