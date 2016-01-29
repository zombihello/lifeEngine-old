#include "LightManager.h"



le::LightManager::LightManager( System& System )
{
    Console = &System.GetConsole();

    iMapWidth = 800;
    iMapHeight = 600;

    color[ 0 ] = 100;
    color[ 1 ] = 100;
    color[ 2 ] = 100;

    RenderTexture.create( iMapWidth , iMapHeight + 30 );
}

le::LightManager::LightManager( System& System , const int iMapWidth , const int iMapHeight )
{
    Console = &System.GetConsole();

    this->iMapWidth = iMapWidth;
    this->iMapHeight = iMapHeight;

    color[ 0 ] = 100;
    color[ 1 ] = 100;
    color[ 2 ] = 100;

    RenderTexture.create( iMapWidth , iMapHeight + 30 );
}


le::LightManager::~LightManager()
{
    DeleteAllLight();
}

void le::LightManager::LoadMask( const string sRoute , bool Smooth )
{

    if ( !Texture.loadFromFile( sRoute ) )
        Console->WriteToConsole( "Error: File [" + sRoute + "] Not Found" , Color::Red );
    else
        Texture.setSmooth( Smooth );
}

void le::LightManager::CreateLight( Vector2f PositionLight , const float fRadius , Color ColorLight )
{
    vLight.push_back( new Light() );
    vLight[ vLight.size() - 1 ]->SetMask( Texture );
    vLight[ vLight.size() - 1 ]->CreateLight( PositionLight , fRadius , ColorLight );
}

void le::LightManager::DrawAllLight( RenderWindow & RenderWindow )
{
    Sprite sLight_for_draw;

    for ( int i = 0; i < vLight.size(); i++ )
        RenderTexture.draw( vLight[ i ]->GetLight() , sf::BlendAdd );

    const sf::Texture &tLight_for_draw = RenderTexture.getTexture();

    sLight_for_draw.setTexture( tLight_for_draw );
    RenderWindow.draw( sLight_for_draw , sf::BlendMultiply );

    RenderTexture.clear( Color::Color( color[ 0 ] , color[ 1 ] , color[ 2 ] ) );
    RenderTexture.display();
}

void le::LightManager::DeleteAllLight()
{
    for ( int i = 0; i < vLight.size(); i++ )
        delete vLight[ i ];

    vLight.clear();
}

void le::LightManager::DeleteLight( const int id )
{
    if ( id - 1 > -1 && id - 1 < vLight.size() )
    {
        delete vLight[ id - 1 ];
        vLight.erase( vLight.begin() + id - 1 );
    }
}

void le::LightManager::SetMapSize( const int iMapWith , const int iMapHeight )
{
    this->iMapHeight = iMapHeight;
    this->iMapWidth = iMapWidth;

    RenderTexture.create( iMapWidth , iMapHeight + 30 );
}

void le::LightManager::SetOpenciy( const float alpha )
{
    color[ 0 ] += alpha;
    color[ 1 ] += alpha;
    color[ 2 ] += alpha;

    RenderTexture.clear( Color::Color( color[ 0 ] , color[ 1 ] , color[ 2 ] ) );
}

void le::LightManager::SetColorShadow( const float r , const float g , const float b )
{
    color[ 0 ] = r;
    color[ 1 ] = g;
    color[ 2 ] = b;
}
