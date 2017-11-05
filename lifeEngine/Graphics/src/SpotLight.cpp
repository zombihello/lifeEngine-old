#include "..\SpotLight.h"

//-------------------------------------------------------------------------//

le::SpotLight::SpotLight() :
	SpotCutoff( -1 ) // cos 180 градусов = -1
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
}

//-------------------------------------------------------------------------//

le::SpotLight::SpotLight( float Radius, float Height, const glm::vec3& Rotation, const glm::vec3& Position, const glm::vec4& Color, float Intensivity, const glm::vec4& Specular )
{
	float C = sqrt( pow( Height, 2 ) + pow( Radius, 2 ) );
	SpotCutoff = Height / C;
	SpotDirection = Rotation;
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
	this->Color = Color / 255.f;
	this->Specular = Specular / 255.f;
	this->Intensivity = Intensivity;

	LightCone.InitCone( Height, Radius );
	LightCone.SetPosition( Position );
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

	return *this;
}

//-------------------------------------------------------------------------//
