#include <System\System.h>
#include <Graphics\Scene.h>
#include <Graphics\Light\PointLight.h>

//-------------------------------------------------------------------------//

le::PointLight::PointLight() :
	Radius( 25 )
{
	Logger::Log( Logger::Info, "Creating Point Light" );
	Logger::Log( Logger::None, "  Radius: " + to_string( Radius ) );
	Logger::Log( Logger::None, "  Intensivity: " + to_string( Intensivity ) );
	Logger::Log( Logger::None, "  Position: " + to_string( Position.x ) + " " + to_string( Position.y ) + " " + to_string( Position.z ) );
	Logger::Log( Logger::None, "  Color: " + to_string( Color.x * 255.f ) + " " + to_string( Color.y * 255.f ) + " " + to_string( Color.z * 255.f ) + " " + to_string( Color.w * 255.f ) );
	Logger::Log( Logger::None, "  Specular: " + to_string( Specular.x * 255.f ) + " " + to_string( Specular.y * 255.f ) + " " + to_string( Specular.z * 255.f ) + " " + to_string( Specular.w * 255.f ) );

	LightSphere.InitSphere( Radius );
	LightSphere.SetPosition( Position );

	glm::vec3 TempPosition = Position;
	LightProjection = glm::perspective( glm::radians( 90.f ), 1.f, 1.f, Radius );

	LightViews =
	{
		glm::lookAt( TempPosition, TempPosition + glm::vec3( 1, 0, 0 ), glm::vec3( 0, 1, 0 ) ),
		glm::lookAt( TempPosition, TempPosition + glm::vec3( -1, 0, 0 ), glm::vec3( 0, 1, 0 ) ),
		glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 1, 0 ), glm::vec3( 0, 0, 1 ) ),
		glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, -1, 0 ), glm::vec3( 0, 0, -1 ) ),
		glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 0, 1 ), glm::vec3( 0, 1, 0 ) ),
		glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 0, -1 ), glm::vec3( 0, 1, 0 ) )
	};

	LightTransforms =
	{
		LightProjection * LightViews[ 0 ],
		LightProjection * LightViews[ 1 ],
		LightProjection * LightViews[ 2 ],
		LightProjection * LightViews[ 3 ],
		LightProjection * LightViews[ 4 ],
		LightProjection * LightViews[ 5 ]
	};

	for ( size_t i = 0; i < 6; i++ )
	{
		Frustums.push_back( Frustum() );
		Frustums[ i ].UpdateFrustum( LightProjection, LightViews[ i ] );
	}

	Logger::Log( Logger::Info, "Created Point Light" );
}

//-------------------------------------------------------------------------//

le::PointLight::PointLight( const PointLight& Copy )
{
	CopyBaseLight( Copy );

	Radius = Copy.Radius;
	LightSphere = Copy.LightSphere;
}

//-------------------------------------------------------------------------//

le::PointLight::PointLight( float Radius, const glm::vec3& Position, const glm::vec4& Color, float Intensivity, const glm::vec4& Specular )
{
	Logger::Log( Logger::Info, "Creating Point Light" );
	Logger::Log( Logger::None, "  Radius: " + to_string( Radius ) );
	Logger::Log( Logger::None, "  Intensivity: " + to_string( Intensivity ) );
	Logger::Log( Logger::None, "  Position: " + to_string( Position.x ) + " " + to_string( Position.y ) + " " + to_string( Position.z ) );
	Logger::Log( Logger::None, "  Color: " + to_string( Color.x ) + " " + to_string( Color.y ) + " " + to_string( Color.z ) + " " + to_string( Color.w ) );
	Logger::Log( Logger::None, "  Specular: " + to_string( Specular.x ) + " " + to_string( Specular.y ) + " " + to_string( Specular.z ) + " " + to_string( Specular.w ) );

	this->Position = glm::vec4( Position, 1.0f );
	this->Color = Color / 255.f;
	this->Specular = Specular / 255.f;
	this->Radius = Radius;
	this->Intensivity = Intensivity;

	LightSphere.InitSphere( Radius );
	LightSphere.SetPosition( Position );

	LightProjection = glm::perspective( glm::radians( 90.f ), 1.f, 1.f, Radius );

	LightViews =
	{
		glm::lookAt( Position, Position + glm::vec3( 1, 0, 0 ), glm::vec3( 0, 1, 0 ) ),
		glm::lookAt( Position, Position + glm::vec3( -1, 0, 0 ), glm::vec3( 0, 1, 0 ) ),
		glm::lookAt( Position, Position + glm::vec3( 0, 1, 0 ), glm::vec3( 0, 0, 1 ) ),
		glm::lookAt( Position, Position + glm::vec3( 0, -1, 0 ), glm::vec3( 0, 0, -1 ) ),
		glm::lookAt( Position, Position + glm::vec3( 0, 0, 1 ), glm::vec3( 0, 1, 0 ) ),
		glm::lookAt( Position, Position + glm::vec3( 0, 0, -1 ), glm::vec3( 0, 1, 0 ) )
	};

	LightTransforms =
	{
		LightProjection * LightViews[ 0 ],
		LightProjection * LightViews[ 1 ],
		LightProjection * LightViews[ 2 ],
		LightProjection * LightViews[ 3 ],
		LightProjection * LightViews[ 4 ],
		LightProjection * LightViews[ 5 ]
	};

	for ( size_t i = 0; i < 6; i++ )
	{
		Frustums.push_back( Frustum() );
		Frustums[ i ].UpdateFrustum( LightProjection, LightViews[ i ] );
	}

	Logger::Log( Logger::Info, "Created Point Light" );
}

//-------------------------------------------------------------------------//

le::PointLight::~PointLight()
{}

//-------------------------------------------------------------------------//

void le::PointLight::SetRadius( float Radius )
{
	this->Radius = Radius;
	LightSphere.SetRadius( Radius );

	glm::vec3 TempPosition = Position;
	LightProjection = glm::perspective( glm::radians( 90.f ), 1.f, 1.f, Radius );

	LightViews =
	{
		glm::lookAt( TempPosition, TempPosition + glm::vec3( 1, 0, 0 ), glm::vec3( 0, 1, 0 ) ),
		glm::lookAt( TempPosition, TempPosition + glm::vec3( -1, 0, 0 ), glm::vec3( 0, 1, 0 ) ),
		glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 1, 0 ), glm::vec3( 0, 0, 1 ) ),
		glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, -1, 0 ), glm::vec3( 0, 0, -1 ) ),
		glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 0, 1 ), glm::vec3( 0, 1, 0 ) ),
		glm::lookAt( TempPosition, TempPosition + glm::vec3( 0, 0, -1 ), glm::vec3( 0, 1, 0 ) )
	};

	LightTransforms =
	{
		LightProjection * LightViews[ 0 ],
		LightProjection * LightViews[ 1 ],
		LightProjection * LightViews[ 2 ],
		LightProjection * LightViews[ 3 ],
		LightProjection * LightViews[ 4 ],
		LightProjection * LightViews[ 5 ]
	};

	for ( size_t i = 0; i < 6; i++ )
	{
		Frustums.push_back( Frustum() );
		Frustums[ i ].UpdateFrustum( LightProjection, LightViews[ i ] );
	}
}

//-------------------------------------------------------------------------//

void le::PointLight::SetPosition( const glm::vec3& Position )
{
	this->Position = glm::vec4( Position, 1.0f );
	LightSphere.SetPosition( Position );

	LightViews =
	{
		glm::lookAt( Position, Position + glm::vec3( 1, 0, 0 ), glm::vec3( 0, 1, 0 ) ),
		glm::lookAt( Position, Position + glm::vec3( -1, 0, 0 ), glm::vec3( 0, 1, 0 ) ),
		glm::lookAt( Position, Position + glm::vec3( 0, 1, 0 ), glm::vec3( 0, 0, 1 ) ),
		glm::lookAt( Position, Position + glm::vec3( 0, -1, 0 ), glm::vec3( 0, 0, -1 ) ),
		glm::lookAt( Position, Position + glm::vec3( 0, 0, 1 ), glm::vec3( 0, 1, 0 ) ),
		glm::lookAt( Position, Position + glm::vec3( 0, 0, -1 ), glm::vec3( 0, 1, 0 ) )
	};

	LightTransforms =
	{
		LightProjection * LightViews[ 0 ],
		LightProjection * LightViews[ 1 ],
		LightProjection * LightViews[ 2 ],
		LightProjection * LightViews[ 3 ],
		LightProjection * LightViews[ 4 ],
		LightProjection * LightViews[ 5 ]
	};

	for ( size_t i = 0; i < 6; i++ )
	{
		Frustums.push_back( Frustum() );
		Frustums[ i ].UpdateFrustum( LightProjection, LightViews[ i ] );
	}
}

//-------------------------------------------------------------------------//

le::PointLight& le::PointLight::operator=( const PointLight& Copy )
{
	CopyBaseLight( Copy );

	Radius = Copy.Radius;
	LightSphere = Copy.LightSphere;

	return *this;
}

//-------------------------------------------------------------------------//
