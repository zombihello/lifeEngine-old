#include <System\System.h>
#include <System\VAO.h>
#include "..\DirectionalLight.h"

//-------------------------------------------------------------------------//

le::DirectionalLight::DirectionalLight()
{
	Logger::Log( Logger::Info, "Creating Directional Light" );
	Logger::Log( Logger::None, "  Intensivity: " + to_string( Intensivity ) );
	Logger::Log( Logger::None, "  Position: " + to_string( Position.x ) + " " + to_string( Position.y ) + " " + to_string( Position.z ) );
	Logger::Log( Logger::None, "  Color: " + to_string( Color.x ) + " " + to_string( Color.y ) + " " + to_string( Color.z ) + " " + to_string( Color.w ) );
	Logger::Log( Logger::None, "  Specular: " + to_string( Specular.x ) + " " + to_string( Specular.y ) + " " + to_string( Specular.z ) + " " + to_string( Specular.w ) );

	Position.w = 0;
	Quad.InitQuad( 1.f );

	float HalfSizeShadowMap = SHADOWMAP_SIZE / 2.f;
	float HalfRenderDistance = System::Configuration.RenderDistance / 2.f;

	glm::vec3 TempPosition( Position );
	LightProjection = glm::ortho( -HalfSizeShadowMap, HalfSizeShadowMap, -HalfSizeShadowMap, HalfSizeShadowMap, -HalfRenderDistance, HalfRenderDistance );
	
	LightViews.push_back( glm::lookAt( TempPosition, -TempPosition, glm::vec3( 0, 1, 0 ) ) );
	LightTransforms.push_back( LightProjection * LightViews[ 0 ] );

	Frustums.push_back( Frustum() );
	Frustums[0].UpdateFrustum( LightProjection, LightViews[0] );

	Logger::Log( Logger::Info, "Created Directional Light" );
}

//-------------------------------------------------------------------------//

le::DirectionalLight::DirectionalLight( const glm::vec3& Position, const glm::vec4& Color, float Intensivity, const glm::vec4& Specular )
{
	Logger::Log( Logger::Info, "Creating Directional Light" );
	Logger::Log( Logger::None, "  Intensivity: " + to_string( Intensivity ) );
	Logger::Log( Logger::None, "  Position: " + to_string( Position.x ) + " " + to_string( Position.y ) + " " + to_string( Position.z ) );
	Logger::Log( Logger::None, "  Color: " + to_string( Color.x ) + " " + to_string( Color.y ) + " " + to_string( Color.z ) + " " + to_string( Color.w ) );
	Logger::Log( Logger::None, "  Specular: " + to_string( Specular.x ) + " " + to_string( Specular.y ) + " " + to_string( Specular.z ) + " " + to_string( Specular.w ) );

	this->Position = glm::vec4( Position, 0.0f );
	this->Color = Color / 255.f;
	this->Specular = Specular / 255.f;
	this->Intensivity = Intensivity;

	Quad.InitQuad( 1 );

	float HalfSizeShadowMap = SHADOWMAP_SIZE / 2.f;
	float HalfRenderDistance = System::Configuration.RenderDistance / 2.f;

	LightProjection = glm::ortho( -HalfSizeShadowMap, HalfSizeShadowMap, -HalfSizeShadowMap, HalfSizeShadowMap, -HalfRenderDistance, HalfRenderDistance );
	
	LightViews.push_back( glm::lookAt( Position, -Position, glm::vec3( 0, 1, 0 ) ) );
	LightTransforms.push_back( LightProjection * LightViews[ 0 ] );

	Frustums.push_back( Frustum() );
	Frustums[ 0 ].UpdateFrustum( LightProjection, LightViews[ 0 ] );

	Logger::Log( Logger::Info, "Created Directional Light" );
}

//-------------------------------------------------------------------------//

le::DirectionalLight::DirectionalLight( const DirectionalLight& Copy )
{
	CopyBaseLight( Copy );

	Quad = Copy.Quad;
	Center = Copy.Center;
}

//-------------------------------------------------------------------------//

le::DirectionalLight::~DirectionalLight()
{}

//-------------------------------------------------------------------------//

void le::DirectionalLight::SetPosition( const glm::vec3& Position )
{
	this->Position = glm::vec4( Position, 0.0f );

	LightViews[0] = glm::lookAt( Position, -Position, glm::vec3( 0, 1, 0 ) ) * glm::translate( Position - Center );
	LightTransforms[0] = LightProjection * LightViews[ 0 ];
	Frustums[ 0 ].UpdateFrustum( LightProjection, LightViews[ 0 ] );
}

//-------------------------------------------------------------------------//

void le::DirectionalLight::SetCenter( const glm::vec3& Center )
{
	//TODO: [zombiHello] Оптимизировать перемещение направленого света

	this->Center = Center;
	glm::vec3 TempPosition( Position );

	LightViews[ 0 ] = glm::lookAt( TempPosition, -TempPosition, glm::vec3( 0, 1, 0 ) ) * glm::translate( TempPosition - Center );
	LightTransforms[ 0 ] = LightProjection * LightViews[ 0 ];
	Frustums[ 0 ].UpdateFrustum( LightProjection, LightViews[ 0 ] );
}

//-------------------------------------------------------------------------//

le::DirectionalLight& le::DirectionalLight::operator=( const DirectionalLight& Copy )
{
	CopyBaseLight( Copy );

	Quad = Copy.Quad;
	Center = Copy.Center;

	return *this;
}

//-------------------------------------------------------------------------//
