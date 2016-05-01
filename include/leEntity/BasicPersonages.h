#ifndef BASICCHARACTERS_H
#define BASICCHARACTERS_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "BasicEntity.h"
#include "BasicBullet.h"
#include "BasicItems.h"
#include "../leWeapons/leWeapons.h"

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
		WeaponManager& GetWeaponManager();
	protected:
		////////////////////////////
		/// ИНИЦИАЛИЗАЦИЯ ПЕРСОНАЖА
		////////////////////////////
		void InitPersonage( int Armor );

		int                             iArmor;

		WeaponManager*                  WeaponManager;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // BASICCHARACTERS_H
