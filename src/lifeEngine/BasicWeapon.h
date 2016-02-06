#ifndef BASIC_WEAPON_H
#define BASIC_WEAPON_H

/////////////////////
// LIFEENGINE
////////////////////
#include "System.h"
#include "AnimationManager.h"

namespace le
{
    class BasicWeapon
    {
    public:
        ///////////////////////////////
        // CONSTRUCTOR | КОНСТРУКТОР
        //////////////////////////////
        BasicWeapon( System& System , AnimationManager& AnimationManager, const bool IsPlayer );

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
    protected:
        /////////////////////////////////
        // INIT VALUES | ИНИЦ. ЗНАЧЕНИЙ  
        /////////////////////////////////
        void Option( const int MaxAmmo , const int Damage , const int ID , const string sNameAnimation );

        int                     iDamage;
        int                     iId;
        int                     iMaxAmmo;
        int                     iCartridgesInStore;
        int                     iAmmoInStock;

        bool                    bIsAtack;
        bool                    bIsPlayer;

        AnimationManager*       AnimationManager;
        RenderWindow*           RenderWindow;
        string                  sNameAnimation;
    };
}

#endif // BASIC_WEAPON_H