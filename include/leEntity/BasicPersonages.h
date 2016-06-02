#ifndef BASICCHARACTERS_H
#define BASICCHARACTERS_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "../leSystem/GroupColision.h"
#include "../leWeapons/leWeapons.h"
#include "BasicEntity.h"
#include "BasicBullet.h"
#include "BasicItems.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class DLL_API BasicPersonages : public BasicEntity
	{
	public:
		////////////////
		/// КОНСТРУКТОР
		////////////////
		BasicPersonages( le::System& System, le::Physic& Physic );

		////////////////
		/// ДЕСТРУКТОР
		////////////////
		virtual ~BasicPersonages();

		///////////////////////////////
		/// ПОЛУЧИТЬ МЕНЕДЖЕР ОРУЖИЯ
		///////////////////////////////
		le::WeaponManager& GetWeaponManager();

		////////////////////////////////////////
		/// ПОЛУЧИТЬ ГРУППУ КОЛИЗИЙ У ПЕРСОНАЖА
		////////////////////////////////////////
		le::GroupColision& GetGroupColisionPersonage();

		/////////////////////////////////////
		/// ПОЛУЧИТЬ ГРУППУ КОЛИЗИЙ У ОРУЖИЯ
		/////////////////////////////////////
		le::GroupColision& GetGroupColisionWeapon();

		/////////////////////////
		/// ВРАГ ЛИ ЭТОТ ОБЪЕКТ
		/////////////////////////
		bool IsEnemy( string sNameEntity );

		///////////////////////////////
		/// АТАКУЕТ ЛИ ПЕРСОНАЖ РУКОЙ
		//////////////////////////////
		bool IsAtackHand();

		////////////////////
		/// ПОЛУЧИТЬ БРОНЮ
		////////////////////
		int& GetArmor();

		/////////////////////////
		/// ПОЛУЧИТЬ УРОН ОТ РУК
		/////////////////////////
		int GetDamageHand();

		///////////////////////////
		/// ПОЛУЧИТЬ СПИСОК ВРАГОВ
		///////////////////////////
		map<string,string> GetListEnemies();
	protected:
		////////////////////////////
		/// ИНИЦИАЛИЗАЦИЯ ПЕРСОНАЖА
		////////////////////////////
		void InitPersonage( int Armor, int DamageHand, le::GroupColision GroupColisionPersonage );
		void InitPersonage( int Armor, int DamageHand, le::GroupColision GroupColisionPersonage, le::GroupColision GroupColisionWeapon );

		/////////////////////////////
		/// ДОБАВИТЬ ВРАГА В СПИСОК
		/////////////////////////////
		void AddEnemy( string sNameEntity );

		/////////////////////////////
		/// ОЧИСТИТЬ СПИСОК ВРАГОВ
		/////////////////////////////
		void ClearListEnemies();

		/////////////////////////////
		/// УБРАТЬ ИЗ СПИСКА ВРАГА
		/////////////////////////////
		void RemoveEnemy( string sNameEntity );

		int									iArmor;
		int									iDamageHand;

		bool								bAtackHand;

		le::WeaponManager*                  WeaponManager;
		le::GroupColision					GroupColisionPersonage;
		le::GroupColision					GroupColisionWeapon;

		map<string,string>					ListEnemies;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // BASICCHARACTERS_H
