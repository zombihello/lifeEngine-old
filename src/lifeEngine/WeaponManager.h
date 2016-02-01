#ifndef WEAPON_MANAGER_H
#define WEAPON_MANAGER_H

///////////////////////
// LIFEENGINE
//////////////////////
#include "System.h"
#include "BasicWeapon.h"

namespace le
{
    class WeaponManager
    {
    public:
        ////////////////////////////////
        // CONSTRUCTOR | КОНСТРУКТОР 
        ////////////////////////////////
        WeaponManager( System& System );

        ////////////////////////////////
        // CONSTRUCTOR | КОНСТРУКТОР 
        ////////////////////////////////
        WeaponManager( System& System , BasicWeapon* Hand );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~WeaponManager();

        ////////////////////////////////
        // GIVE WEAPON | ДАТЬ ОРУЖИЕ  
        ////////////////////////////////
        void GiveWeapon( BasicWeapon* BasicWeapon );

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
        void UpdateWeapons();

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
    private:
        int                             iIdTmpWeapon;
        int                             iMaxWeapon;
        System*                         System;
        vector<BasicWeapon*>            vWeapon;
    };
}

#endif //WEAPON_MANAGER_H