#include "../LightManager.h"

//-------------------------------------------------------------------------//

le::LightManager::LightManager( le::System& System )
{
	Camera = &System.GetGameCamera();
	RenderWindow = &System.GetWindow();
	SizeMap = Vector2f( System.GetConfiguration().iWindowWidth, System.GetConfiguration().iWindowHeight );

	color[ 0 ] = 100;
	color[ 1 ] = 100;
	color[ 2 ] = 100;

	RenderTexture.create( SizeMap.x, SizeMap.y + 30 );
}

//-------------------------------------------------------------------------//

le::LightManager::LightManager( le::System& System, Vector2f SizeMap )
{
	Camera = &System.GetGameCamera();
	RenderWindow = &System.GetWindow();
	this->SizeMap = SizeMap;

	color[ 0 ] = 100;
	color[ 1 ] = 100;
	color[ 2 ] = 100;

	RenderTexture.create( SizeMap.x, SizeMap.y + 30 );
}

//-------------------------------------------------------------------------//

le::LightManager::~LightManager()
{
	DeleteAllLight();
}

//-------------------------------------------------------------------------//

void le::LightManager::LoadLightMask( string Route, bool Smooth )
{
	if ( Texture.loadFromFile( Route ) )
		Texture.setSmooth( Smooth );
}

//-------------------------------------------------------------------------//

void le::LightManager::CreateLight( Vector2f Position, float Radius, Color Color )
{
	Light* Light = new le::Light;
	Light->SetMask( Texture );
	Light->CreateLight( Position, Radius, Color );

	vLight.push_back( Light );
}

//-------------------------------------------------------------------------//

void le::LightManager::CreateLight( Vector2f Position, float Radius, Color Color, string NameLight )
{
	Light* Light = new le::Light;
	Light->SetMask( Texture );
	Light->CreateLight( Position, Radius, Color, NameLight );

	vLight.push_back( Light );
}

//-------------------------------------------------------------------------//

void le::LightManager::CreateLight( le::Object obj )
{
	Light* Light = new le::Light;
	LoadLightMask( obj.GetPropertyString( "route" ) );
	Light->SetMask( Texture );

	Light->CreateLight( Vector2f( obj.rect.left, obj.rect.top ),
						obj.GetPropertyInt( "radius" ),
						Color( obj.GetPropertyInt( "r" ) ,
							   obj.GetPropertyInt( "g" ) ,
							   obj.GetPropertyInt( "b" ) ),
						obj.GetPropertyString( "name" )
						);

	vLight.push_back( Light );
}

//-------------------------------------------------------------------------//

void le::LightManager::DrawLight()
{
	Sprite sLight;

	FloatRect TempCamera = FloatRect( Camera->getCenter().x - Camera->getSize().x / 2,
									  Camera->getCenter().y - Camera->getSize().y / 2,
									  Camera->getSize().x,
									  Camera->getSize().y );

	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light* Light = vLight[ i ];

		if ( Light->GetRect().intersects( TempCamera ) )
			RenderTexture.draw( vLight[ i ]->GetCircleShape(), sf::BlendAdd );
	}

	const sf::Texture &tLight = RenderTexture.getTexture();

	sLight.setTexture( tLight );
	RenderWindow->draw( sLight, BlendMultiply );

	RenderTexture.clear( Color( color[ 0 ], color[ 1 ], color[ 2 ] ) );
	RenderTexture.display();
}

//-------------------------------------------------------------------------//

void le::LightManager::DeleteAllLight()
{
	for ( int i = 0; i < vLight.size(); i++ )
		delete vLight[ i ];

	vLight.size();
}

//-------------------------------------------------------------------------//

void le::LightManager::DeleteLight( int id )
{
	if ( id - 1 > -1 && id - 1 < vLight.size() )
	{
		delete vLight[ id ];
		vLight.erase( vLight.begin() + id - 1 );
	}
}

//-------------------------------------------------------------------------//

void le::LightManager::DeleteLight( string NameLight )
{
	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light* light = vLight[i];

		if ( light->GetNameLight() == NameLight )
		{
			delete light;
			vLight.erase( vLight.begin() + i );
			return;
		}
	}
}

//-------------------------------------------------------------------------//

void le::LightManager::SetSizeMap( Vector2f SizeMap )
{
	this->SizeMap = SizeMap;

	RenderTexture.clear( Color( color[ 0 ], color[ 1 ], color[ 2 ] ) );
	RenderTexture.create( SizeMap.x, SizeMap.y );
}

//-------------------------------------------------------------------------//

void le::LightManager::SetOpencity( float Opencity )
{
	color[ 0 ] += Opencity;
	color[ 1 ] += Opencity;
	color[ 2 ] += Opencity;

	RenderTexture.clear( Color( color[ 0 ] , color[ 1 ] , color[ 2 ] ) );
}

//-------------------------------------------------------------------------//

void le::LightManager::SetColorShadow( float r, float g, float b )
{
	color[ 0 ] = r;
	color[ 1 ] = g;
	color[ 2 ] = b;
}

//-------------------------------------------------------------------------//

void le::LightManager::SetColorShadow( Vector3f color )
{
	this->color[ 0 ] = color.x;
	this->color[ 1 ] = color.y;
	this->color[ 2 ] = color.z;
}

//-------------------------------------------------------------------------//

void le::LightManager::SetRadius( int id, float Radius )
{
	if ( id - 1 > -1 && id - 1 < vLight.size() )
	{
		Light* light = vLight[ id-1 ];
		light->SetRadius( Radius );
	}
}

//-------------------------------------------------------------------------//

void le::LightManager::SetRadius( string NameLight, float Radius )
{
	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light* light = vLight[i];

		if ( light->GetNameLight() == NameLight )
		{
			light->SetRadius( Radius );
			return;
		}
	}
}

//-------------------------------------------------------------------------//

void le::LightManager::SetPosition( int id, Vector2f Position )
{
	if ( id - 1 > -1 && id - 1 < vLight.size() )
	{
		Light* light = vLight[ id-1 ];
		light->SetPosition( Position );
	}
}

//-------------------------------------------------------------------------//

void le::LightManager::SetPosition( string NameLight, Vector2f Position )
{
	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light* light = vLight[i];

		if ( light->GetNameLight() == NameLight )
		{
			light->SetPosition( Position );
			return;
		}
	}
}

//-------------------------------------------------------------------------//

void le::LightManager::SetColor( int id, sf::Color Color )
{
	if ( id - 1 > -1 && id - 1 < vLight.size() )
	{
		Light* light = vLight[ id-1 ];
		light->SetColor( Color );
	}
}

//-------------------------------------------------------------------------//

void le::LightManager::SetColor( string NameLight, sf::Color Color )
{
	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light* light = vLight[i];

		if ( light->GetNameLight() == NameLight )
		{
			light->SetColor( Color );
			return;
		}
	}
}

//-------------------------------------------------------------------------//

le::Light* le::LightManager::GetLight( int id )
{
	if ( id - 1 > -1 && id - 1 < vLight.size() )
		return vLight[ id-1 ];
}

//-------------------------------------------------------------------------//

le::Light* le::LightManager::GetLight( string NameLight )
{
	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light* light = vLight[i];

		if ( light->GetNameLight() == NameLight )
			return light;
	}
}

//-------------------------------------------------------------------------//

float le::LightManager::GetRadius( int id )
{
	if ( id - 1 > -1 && id - 1 < vLight.size() )
	{
		Light* light = vLight[ id-1 ];
		return light->GetRadius();
	}
}

//-------------------------------------------------------------------------//

float le::LightManager::GetRadius( string NameLight )
{
	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light* light = vLight[i];

		if ( light->GetNameLight() == NameLight )
			return light->GetRadius();
	}
}

//-------------------------------------------------------------------------//

Vector2f le::LightManager::GetPosition( int id )
{
	if ( id - 1 > -1 && id - 1 < vLight.size() )
	{
		Light* light = vLight[ id-1 ];
		return light->GetPosition();
	}
}

//-------------------------------------------------------------------------//

Vector2f le::LightManager::GetPosition( string NameLight )
{
	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light* light = vLight[i];

		if ( light->GetNameLight() == NameLight )
			return light->GetPosition();
	}
}

//-------------------------------------------------------------------------//

Color le::LightManager::GetColor( int id )
{
	if ( id - 1 > -1 && id - 1 < vLight.size() )
	{
		Light* light = vLight[ id-1 ];
		return light->GetColor();
	}
}

//-------------------------------------------------------------------------//

Color le::LightManager::GetColor( string NameLight )
{
	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light* light = vLight[i];

		if ( light->GetNameLight() == NameLight )
			return light->GetColor();
	}
}

//-------------------------------------------------------------------------//

Vector3f le::LightManager::GetColorShadow()
{
	return Vector3f( color[ 0 ], color[ 1 ], color[ 2 ] );
}

//-------------------------------------------------------------------------//
