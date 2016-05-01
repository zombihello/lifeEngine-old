#include "../LightManager.h"

//-------------------------------------------------------------------------//

le::LightManager::LightManager( le::System& System )
{
	Camera = &System.GetCamera();
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
	Camera = &System.GetCamera();
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

void le::LightManager::CreateLight( le::Object obj )
{
	Light* Light = new le::Light;
	LoadLightMask( obj.GetPropertyString( "route" ) );
	Light->SetMask( Texture );

	Light->CreateLight( Vector2f( obj.rect.left, obj.rect.top ),
						obj.GetPropertyInt( "radius" ),
						Color( obj.GetPropertyInt( "color_R" ) ,
							   obj.GetPropertyInt( "color_G" ) ,
							   obj.GetPropertyInt( "color_B" ) )
						);
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
			RenderTexture.draw( vLight[ i ]->GetLight(), sf::BlendAdd );
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

void le::LightManager::SetSizeMap( Vector2f SizeMap )
{
	this->SizeMap = SizeMap;
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
