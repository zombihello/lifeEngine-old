#ifndef BASIC_WEAPON_H
#define BASIC_WEAPON_H

/////////////////////
// LIFEENGINE
////////////////////
#include "System.h"

namespace le
{
    class BasicWeapon
    {
    public:
        ///////////////////////////////
        // CONSTRUCTOR | КОНСТРУКТОР
        //////////////////////////////
        BasicWeapon( System& System , float& fDx , const IntRect& RectEntitySprite , FloatRect& RectEntity, const bool IsPlayer = false );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~BasicWeapon();

        ////////////////////////////////////
        // UPDATE WEAPON | ОБНОВИТЬ ОРУЖИЕ  
        ////////////////////////////////////
        virtual void UpdateWeapon() = 0;

        //////////////////////////////////////////////////////////////////////
        // GET CARTRIDGES IN STORE | ПОЛУЧИТЬ КОЛ-ВО БОЕПРИПАСОВ В МАГАЗИНЕ   
        // @return iCartridgesInStore ( int )
        ////////////////////////////////////////////////////////////////////
        int GetCartridgesInStore() const;

        ////////////////////////////////////////////////////////////////
        // GET AMMO IN STOCK | ПОЛУЧИТЬ КОЛ-ВО БОЕПРИПАСОВ В НАЛИЧИИ   
        // @return iAmmoInStock ( int )
        ///////////////////////////////////////////////////////////////
        int GetAmmoInStock() const;

        ////////////////////////////////////////////////////////////////
        // GET MAX AMMO | ПОЛУЧИТЬ МАКСИМАЛЬНОЕ КОЛ-ВО БОЕПРЕПАСОВ
        // @return iMaxAmmo ( int )
        ///////////////////////////////////////////////////////////////
        int GetMaxAmmo() const;

        ///////////////////////////////
        // GET DAMAGE | ПОЛУЧИТЬ УРОН 
        // @return iDamage ( int )
        //////////////////////////////
        int GetDamage() const;

        ///////////////////////////////////////////
        // GET NAME WEAPON | ПОЛУЧИТЬ ИМЯ ОРУЖИЯ
        // @return sNameWeapon ( string )
        //////////////////////////////////////////
        string GetNameWeapon() const;

        ////////////////////////////////////////////////////
        // GET RECT WEAPON | ПОЛУЧИТЬ ПРЯМОУГОЛЬНИК ОРУЖИЯ
        // @return RectWeapon ( FloatRect )
        ////////////////////////////////////////////////////
        FloatRect& GetRectWeapon();

        ////////////////////////////////////////////////////
        // GET SPRITE WEAPON | ПОЛУЧИТЬ СПРАЙТ ОРУЖИЯ
        // @return Sprite ( Sprite )
        ////////////////////////////////////////////////////
        Sprite& GetSpriteWeapon();
    protected:
        ///////////////////////////////////////
        // SHOOTING WEAPONS | СТРЕЛЬБА ОРУЖИЯ   
        //////////////////////////////////////
        virtual void Fire() = 0;

        ///////////////////////////////////////////////////////////////////////////
        // THE ANIMATION OF THE WEAPON WHEN MOVING | АНИМАЦИЯ ОРУЖИЯ ПРИ ДВИЖЕНИИ
        ///////////////////////////////////////////////////////////////////////////
        void AnimationMove();

        /////////////////////////////////
        // INIT VALUES | ИНИЦ. ЗНАЧЕНИЙ  
        /////////////////////////////////
        void Option( const int MaxAmmo , const int Damage , const float Angle , const string sNameWeapon );

        int                     iCartridgesInStore;
        int                     iAmmoInStock;
        int                     iMaxAmmo;
        int                     iDamage;

        float                   fTimer;
        float*                  fTime;
        float                   fAngle;
        float                   fRotation;
        float*                  fDx;

        bool                    bIsAtack;
        bool                    bIsPlayer;

        const IntRect*          RectEntitySprite;
        FloatRect*              RectEntity;
        FloatRect               RectWeapon;
        RenderWindow*           RenderWindow;
        Texture                 Texture;
        Sprite                  Sprite;
        string                  sNameWeapon;
    };
}

#endif // BASIC_WEAPON_H