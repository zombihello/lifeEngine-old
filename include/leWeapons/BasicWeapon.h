#ifndef BASICWEAPON_H
#define BASICWEAPON_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "../leEntity/GroupColision.h"
#include "../leSystem/leSystem.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
    class BasicBullet;
	
	//-------------------------------------------------------------------------//

    class DLL_API BasicWeapon
    {
    public:
        ////////////////
        /// КОНСТРУКТОР
        ////////////////
        BasicWeapon();

        ////////////////
        /// ДЕСТРУКТОР
        ////////////////
       virtual ~BasicWeapon();

        ///////////////////////
        /// ОБНОВЛЕНИЕ ОРУЖИЯ
        ///////////////////////
        virtual void Update() = 0;

        //////////////
        /// СТРЕЛЬБА
        //////////////
		virtual void Shot( vector<le::BasicBullet*>& vBullet, Vector2f Position, bool Flip ) = 0;

        //////////////////////////////
        /// ДОБАВИТЬ ПАТРОНОВ В ЗАПАС
        //////////////////////////////
        void AddAmmoInStock( int Ammo );

		///////////////////////////
		/// ЗАДАТЬ ГРУППУ КОЛИЗИЙ
		///////////////////////////
		void SetGroupColision( le::GroupColision GroupColision );

        ///////////////////
        /// ПОЛУЧИТЬ УРОН
        ///////////////////
        int GetDamage();

        ////////////////////////////////////
        /// ПОЛУЧИТЬ МАКС. КОЛ-ВО ПАТРОНОВ
        ////////////////////////////////////
        int GetMaxAmmo();

        ////////////////////////////////////////
        /// ПОЛУЧИТЬ КОЛ-ВО ПАТРОНОВ В МАГАЗИНЕ
        ///////////////////////////////////////
        int GetCartridgesInStore();

        ////////////////////////////////////////
        /// ПОЛУЧИТЬ КОЛ-ВО ПАТРОНОВ В ЗАПАСЕ
        ///////////////////////////////////////
        int GetAmmoInStock();

        ////////////////////////////
        /// АТАКУЕТ ЛИ ЭТО ОРУЖИЕ
        ///////////////////////////
        bool GetIsAtack();

        //////////////////////////////
        /// ПОЛУЧИТЬ НАЗВАНИЕ ОРУЖИЯ
        /////////////////////////////
        string GetNameWeapon();

		///////////////////////////
		/// ПОЛУЧИТЬ ГРУППУ КОЛИЗИЙ
		///////////////////////////
		le::GroupColision& GetGroupColision();
    protected:
        //////////////////////////
        /// ИНИЦИАЛИЗАЦИЯ ОРУЖИЯ
        //////////////////////////
		void InitWeapon( int Damage , int MaxAmmo , int AmmoInStock , le::GroupColision GroupColision, string sNameAnimation );

		int								iDamage;
		int								iMaxAmmo;
		int								iCartridgesInStore;
		int								iAmmoInStock;

		bool							bIsAtack;

		string							sNameAnimation;

		le::GroupColision				GroupColision;
    };
	
	//-------------------------------------------------------------------------//
}

#endif // BASICWEAPON_H
