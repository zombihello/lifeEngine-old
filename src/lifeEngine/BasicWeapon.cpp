#include "BasicWeapon.h"


le::BasicWeapon::BasicWeapon( System & System , float & fDx , const IntRect & RectEntitySprite , FloatRect & RectEntity , const bool IsPlayer )
{
    RenderWindow = &System.GetWindow();
    fTime = &System.GetConfiguration().fTime;
    this->fDx = &fDx;
    this->RectEntitySprite = &RectEntitySprite;
    this->RectEntity = &RectEntity;
    bIsPlayer = IsPlayer;
}

le::BasicWeapon::~BasicWeapon()
{
}

int le::BasicWeapon::GetCartridgesInStore() const
{
    return iCartridgesInStore;
}

int le::BasicWeapon::GetAmmoInStock() const
{
    return iAmmoInStock;
}

int le::BasicWeapon::GetMaxAmmo() const
{
    return iMaxAmmo;
}

int le::BasicWeapon::GetDamage() const
{
    return iDamage;
}

string le::BasicWeapon::GetNameWeapon() const
{
    return sNameWeapon;
}

FloatRect & le::BasicWeapon::GetRectWeapon()
{
    return RectWeapon;
}

sf::Sprite & le::BasicWeapon::GetSpriteWeapon()
{
    return Sprite;
}

void le::BasicWeapon::AnimationMove()
{
    if ( *fDx != 0 )
    {
        if ( fTimer < 500 )
            Sprite.rotate( -1 );

        fTimer += *fTime;

        if ( fTimer > 500 )
            Sprite.rotate( 1 );

        if ( fTimer > 1000 )
        {
            fTimer = 0;
            Sprite.setRotation( fAngle );
        }
    }
    else
    {
        Sprite.setRotation( fAngle );
        fTimer = 0;
    }
}

void le::BasicWeapon::Option( const int MaxAmmo , const int Damage , const float Angle , const string sNameWeapon )
{
    iMaxAmmo = MaxAmmo;
    iDamage = Damage;
    fAngle = Angle;
    this->sNameWeapon = sNameWeapon;
    Sprite.setRotation( Angle );

    iCartridgesInStore = iAmmoInStock = fTimer = fRotation = 0;
    bIsAtack = false;
}
