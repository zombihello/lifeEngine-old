#include "BasicHUD.h"



le::BasicHUD::BasicHUD( System& System )
{
    RenderWindow = &System.GetWindow();

    TextManager = new le::TextManager( System );

    Hud.reset( FloatRect( 0 , 0 , System.GetConfiguration().iWindowWidth , System.GetConfiguration().iWindowHeight ) );
}


le::BasicHUD::~BasicHUD()
{
}

void le::BasicHUD::InitHUD( le::BasicEntity& BasicEntity , le::WeaponManager& WeaponManager , const string sRouteToFont )
{
    TextManager->LoadFont( sRouteToFont );

    this->BasicEntity = &BasicEntity;
    this->WeaponManager = &WeaponManager;

    InitEntityHUD();
    InitWeaponHUD();
}

void le::BasicHUD::UpdateHUD( View GameCamera )
{
    RenderWindow->setView( Hud );

    UpdateEntityHUD();
    UpdateWeaponHUD();

    RenderWindow->setView( GameCamera );
}
