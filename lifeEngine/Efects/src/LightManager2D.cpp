#include "../LightManager2D.h"

//-------------------------------------------------------------------------//

le::LightManager2D::LightManager2D( le::System& System )
{
	Camera = &System.GetGameCamera();
	RenderWindow = &System.GetWindow();
	SizeMap = Vector2f( System.GetConfiguration().iWindowWidth, System.GetConfiguration().iWindowHeight );

	color[0] = 100;
	color[1] = 100;
	color[2] = 100;

	RenderTexture.create( SizeMap.x, SizeMap.y + 30 );
}

//-------------------------------------------------------------------------//

le::LightManager2D::LightManager2D( le::System& System, Vector2f SizeMap )
{
	Camera = &System.GetGameCamera();
	RenderWindow = &System.GetWindow();
	this->SizeMap = SizeMap;

	color[0] = 100;
	color[1] = 100;
	color[2] = 100;

	RenderTexture.create( SizeMap.x, SizeMap.y + 30 );
}

//-------------------------------------------------------------------------//

le::LightManager2D::~LightManager2D( )
{
	DeleteAllLight();
}

//-------------------------------------------------------------------------//

void le::LightManager2D::LoadLightMask( string Route, bool Smooth )
{
	if ( Texture.loadFromFile( Route ) )
		Texture.setSmooth( Smooth );
}

//-------------------------------------------------------------------------//

void le::LightManager2D::CreateLight( Vector2f Position, float Radius, Color Color )
{
	Light2D* Light = new le::Light2D;
	Light->SetMask( Texture );
	Light->CreateLight( Position, Radius, Color );

	vLight.push_back( Light );
}

//-------------------------------------------------------------------------//

void le::LightManager2D::CreateLight( Vector2f Position, float Radius, Color Color, string NameLight )
{
	Light2D* Light = new le::Light2D;
	Light->SetMask( Texture );
	Light->CreateLight( Position, Radius, Color, NameLight );

	vLight.push_back( Light );
}

//-------------------------------------------------------------------------//

void le::LightManager2D::CreateLight( le::Object obj )
{
	Light2D* Light = new le::Light2D;
	LoadLightMask( obj.GetPropertyString( "route" ) );
	Light->SetMask( Texture );

	Light->CreateLight( Vector2f( obj.rect.left, obj.rect.top ),
		obj.GetPropertyInt( "radius" ),
		Color( obj.GetPropertyInt( "r" ),
		obj.GetPropertyInt( "g" ),
		obj.GetPropertyInt( "b" ) ),
		obj.GetPropertyString( "name" )
		);

	vLight.push_back( Light );
}

//-------------------------------------------------------------------------//

void le::LightManager2D::DrawLight( )
{
	Sprite sLight;

	FloatRect TempCamera = FloatRect( Camera->getCenter().x - Camera->getSize().x / 2,
		Camera->getCenter().y - Camera->getSize().y / 2,
		Camera->getSize().x,
		Camera->getSize().y );

	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light2D* Light = vLight[i];

		if ( Light->GetRect().intersects( TempCamera ) )
			RenderTexture.draw( vLight[i]->GetCircleShape(), sf::BlendAdd );
	}

	const sf::Texture &tLight = RenderTexture.getTexture();

	sLight.setTexture( tLight );
	RenderWindow->draw( sLight, BlendMultiply );

	RenderTexture.clear( Color( color[0], color[1], color[2] ) );
	RenderTexture.display();
}

//-------------------------------------------------------------------------//

void le::LightManager2D::DeleteAllLight( )
{
	for ( int i = 0; i < vLight.size(); i++ )
		delete vLight[i];

	vLight.size();
}

//-------------------------------------------------------------------------//

void le::LightManager2D::DeleteLight( int id )
{
	if ( id - 1 > -1 && id - 1 < vLight.size() )
	{
		delete vLight[id];
		vLight.erase( vLight.begin() + id - 1 );
	}
}

//-------------------------------------------------------------------------//

void le::LightManager2D::DeleteLight( string NameLight )
{
	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light2D* light = vLight[i];

		if ( light->GetNameLight() == NameLight )
		{
			delete light;
			vLight.erase( vLight.begin() + i );
			return;
		}
	}
}

//-------------------------------------------------------------------------//

void le::LightManager2D::SetSizeMap( Vector2f SizeMap )
{
	this->SizeMap = SizeMap;

	RenderTexture.clear( Color( color[0], color[1], color[2] ) );
	RenderTexture.create( SizeMap.x, SizeMap.y );
}

//-------------------------------------------------------------------------//

void le::LightManager2D::SetOpencity( float Opencity )
{
	color[0] += Opencity;
	color[1] += Opencity;
	color[2] += Opencity;

	RenderTexture.clear( Color( color[0], color[1], color[2] ) );
}

//-------------------------------------------------------------------------//

void le::LightManager2D::SetColorShadow( float r, float g, float b )
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
}

//-------------------------------------------------------------------------//

void le::LightManager2D::SetColorShadow( Vector3f color )
{
	this->color[0] = color.x;
	this->color[1] = color.y;
	this->color[2] = color.z;
}

//-------------------------------------------------------------------------//

void le::LightManager2D::SetRadius( int id, float Radius )
{
	if ( id - 1 > -1 && id - 1 < vLight.size() )
	{
		Light2D* light = vLight[id - 1];
		light->SetRadius( Radius );
	}
}

//-------------------------------------------------------------------------//

void le::LightManager2D::SetRadius( string NameLight, float Radius )
{
	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light2D* light = vLight[i];

		if ( light->GetNameLight() == NameLight )
		{
			light->SetRadius( Radius );
			return;
		}
	}
}

//-------------------------------------------------------------------------//

void le::LightManager2D::SetPosition( int id, Vector2f Position )
{
	if ( id - 1 > -1 && id - 1 < vLight.size() )
	{
		Light2D* light = vLight[id - 1];
		light->SetPosition( Position );
	}
}

//-------------------------------------------------------------------------//

void le::LightManager2D::SetPosition( string NameLight, Vector2f Position )
{
	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light2D* light = vLight[i];

		if ( light->GetNameLight() == NameLight )
		{
			light->SetPosition( Position );
			return;
		}
	}
}

//-------------------------------------------------------------------------//

void le::LightManager2D::SetColor( int id, sf::Color Color )
{
	if ( id - 1 > -1 && id - 1 < vLight.size() )
	{
		Light2D* light = vLight[id - 1];
		light->SetColor( Color );
	}
}

//-------------------------------------------------------------------------//

void le::LightManager2D::SetColor( string NameLight, sf::Color Color )
{
	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light2D* light = vLight[i];

		if ( light->GetNameLight() == NameLight )
		{
			light->SetColor( Color );
			return;
		}
	}
}

//-------------------------------------------------------------------------//

le::Light2D* le::LightManager2D::GetLight( int id )
{
	if ( id - 1 > -1 && id - 1 < vLight.size() )
		return vLight[id - 1];

	return NULL;
}

//-------------------------------------------------------------------------//

le::Light2D* le::LightManager2D::GetLight( string NameLight )
{
	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light2D* light = vLight[i];

		if ( light->GetNameLight() == NameLight )
			return light;
	}

	return NULL;
}

//-------------------------------------------------------------------------//

float le::LightManager2D::GetRadius( int id )
{
	if ( id - 1 > -1 && id - 1 < vLight.size() )
	{
		Light2D* light = vLight[id - 1];
		return light->GetRadius();
	}

	return -1;
}

//-------------------------------------------------------------------------//

float le::LightManager2D::GetRadius( string NameLight )
{
	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light2D* light = vLight[i];

		if ( light->GetNameLight() == NameLight )
			return light->GetRadius();
	}

	return -1;
}

//-------------------------------------------------------------------------//

Vector2f le::LightManager2D::GetPosition( int id )
{
	if ( id - 1 > -1 && id - 1 < vLight.size() )
	{
		Light2D* light = vLight[id - 1];
		return light->GetPosition();
	}

	return Vector2f( -1, -1 );
}

//-------------------------------------------------------------------------//

Vector2f le::LightManager2D::GetPosition( string NameLight )
{
	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light2D* light = vLight[i];

		if ( light->GetNameLight() == NameLight )
			return light->GetPosition();
	}

	return Vector2f( -1, -1 );
}

//-------------------------------------------------------------------------//

Color le::LightManager2D::GetColor( int id )
{
	if ( id - 1 > -1 && id - 1 < vLight.size() )
	{
		Light2D* light = vLight[id - 1];
		return light->GetColor();
	}

	return Color( -1, -1, -1, -1 );
}

//-------------------------------------------------------------------------//

Color le::LightManager2D::GetColor( string NameLight )
{
	for ( int i = 0; i < vLight.size(); i++ )
	{
		Light2D* light = vLight[i];

		if ( light->GetNameLight() == NameLight )
			return light->GetColor();
	}

	return Color( -1, -1, -1, -1 );
}

//-------------------------------------------------------------------------//

Vector3f le::LightManager2D::GetColorShadow( )
{
	return Vector3f( color[0], color[1], color[2] );
}

//-------------------------------------------------------------------------//
