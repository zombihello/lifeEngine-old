#ifndef WEAPONMANAGER_H
#define WEAPONMANAGER_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "../System/leSystem.h"
#include "BasicWeapon.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
    class DLL_API WeaponManager
    {
    public:
        ////////////////
        /// КОНСТРУКТОР
        ////////////////
        WeaponManager( System& System );

        ////////////////
        /// ДЕСТРУКТОР
        ////////////////
        ~WeaponManager();

        ////////////////
        /// ДАТЬ ОРУЖИЕ
        ////////////////
        void GiveWeapon( BasicWeapon* BasicWeapon );

        /////////////////
        /// ДАТЬ ПАТРОНЫ
        /////////////////
		void GiveAmmoInStock( int ammo , string NameWeapon );
		void GiveAmmoInStore( int ammo , string NameWeapon );

        ////////////////////////
        // УДАЛИТЬ ВСЕ ОРУЖИЕ
        ///////////////////////
        void DeleteAllWeapon();

        ///////////////////
        // УДАЛИТЬ ОРУЖИЕ
        ///////////////////
        void DeleteWeapon( string sNameWeapon );

        ////////////////////
        /// ОБНОВИТЬ ОРУЖИЕ
        ////////////////////
        void UpdateWeapon();

        /////////////////////////////
        // ЗАДАТЬ ТЕК. ОРУЖИЕ ПО ID
        /////////////////////////////
        void SetTmpWeapon( int IdWeapon );

        //////////////////////////
        // ПОЛУЧИТЬ ОРУЖИЕ ПО ID
        //////////////////////////
        BasicWeapon* GetWeapon( int id );

        ////////////////////////////////
        // ПОЛУЧИТЬ МАКС. КОЛ-ВО ОРУЖИЯ
        ///////////////////////////////
        int GetMaxWeapon();

        /////////////////////////////
        // ПОЛУЧИТЬ ОРУЖИЕ ПО ИМЕНИ
        /////////////////////////////
        BasicWeapon* GetWeapon( string NameWeapon );

        /////////////////////////////
        // ПОЛУЧИТЬ ТЕКУЩИЕ ОРУЖИЕ
        /////////////////////////////
        BasicWeapon* GetTmpWeapon();

        ////////////////////////
        // ПОЛУЧИТЬ ВСЕ ОРУЖИЕ
        ////////////////////////
        vector<BasicWeapon*> GetAllWeapon();
    private:
        int                                 iIdTmpWeapon;
        int                                 iMaxWeapon;

        Event*                              Event;
        vector<BasicWeapon*>                vWeapon;
    };
	
	//-------------------------------------------------------------------------//
}

#endif // WEAPONMANAGER_H
