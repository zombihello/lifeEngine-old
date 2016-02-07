#ifndef WEAPON_MANAGER_H
#define WEAPON_MANAGER_H

///////////////////////
// LIFEENGINE
//////////////////////
#include "System.h"
#include "BasicWeapon.h"
//#include "BasicEntity.h"

namespace le
{
    class WeaponManager
    {
    public:
        ////////////////////////////////
        // CONSTRUCTOR | КОНСТРУКТОР 
        ////////////////////////////////
        WeaponManager( System& System );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~WeaponManager();

        ////////////////////////////////
        // GIVE WEAPON | ДАТЬ ОРУЖИЕ  
        ////////////////////////////////
        void GiveWeapon( BasicWeapon* BasicWeapon );

        ////////////////////////////////
        // GIVE AMMO | ДАТЬ ПАТРОНЫ  
        ////////////////////////////////
        void GiveAmmo( const int ammo , const string NameWeapon );

        ///////////////////////////////////////////
        // DELETE ALL WEAPON | УДАЛИТЬ ВСЕ ОРУЖИЕ   
        //////////////////////////////////////////
        void DeleteAllWeapon();

        //////////////////////////////////
        // DELETE WEAPON | УДАЛИТЬ ОРУЖИЕ   
        //////////////////////////////////
        void DeleteWeapon( const string sNameWeapon );

        ///////////////////////////////////////////
        // UPDATE ALL WEAPON | ОБНОВИТЬ ВСЕ ОРУЖИЕ   
        //////////////////////////////////////////
        void UpdateWeapons( const float fDx , const float fDy );

        //////////////////////////////////////////////
        // GET WEAPON BY ID | ПОЛУЧИТЬ ОРУЖИЕ ПО ID
        // @return BasicWeapon*
        //////////////////////////////////////////////
        BasicWeapon* GetWeapon( const int id );

        //////////////////////////////////////////////////
        // GET WEAPON BY NAME | ПОЛУЧИТЬ ОРУЖИЕ ПО ИМЕНИ
        // @return BasicWeapon*
        //////////////////////////////////////////////////
        BasicWeapon* GetWeapon( const string NameWeapon );

        //////////////////////////////////////////////////
        // GET TEMP WEAPON | ПОЛУЧИТЬ ТЕКУЩИЕ ОРУЖИЕ  
        // @return BasicWeapon*
        //////////////////////////////////////////////////
        BasicWeapon* GetTmpWeapon();
    private:
        int                             iIdTmpWeapon;
        int                             iMaxWeapon;

        System*                         System;
        Event*                          Event;
        Event::EventType*               TypeEvent;
        vector<BasicWeapon*>            vWeapon;
    };
}

#endif //WEAPON_MANAGER_H