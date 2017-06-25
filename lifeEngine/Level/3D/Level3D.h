#ifndef LEVEL3D_H
#define LEVEL3D_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "Brush.h"
#include "Entity.h"
#include "../../System/System.h"

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API Level3D
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		////////////////
		Level3D( System& System );

		/////////////////
		/// ДЕСТРУКТОР
		////////////////
		~Level3D();

		///////////////////////
		/// ЗАГРУЗИТЬ УРОВЕНЬ
		//////////////////////
		bool LoadLevel( string sRoute );

		///////////////////////
		/// ОЧИСТИТЬ УРОВЕНЬ
		//////////////////////
		void ClearLevel();

		///////////////////////
		/// ОТРЕНДЕРИТЬ УРОВЕНЬ
		//////////////////////
		void RenderLevel();

		///////////////////////
		/// ПОЛУЧИТЬ ЭНТИТИ
		//////////////////////
		Entity& GetEntity( string NameEntity );

		//////////////////////////
		/// ПОЛУЧИТЬ ВСЕ ЭНТИТИ
		//////////////////////////
		vector<Entity>& GetAllEntitys();

	private:

		string				sNameMap;
		string				sDescriptionMap;
		string				sSkyBoxName;

		System*				System;

		vector<Brush*>		vBrushes;
		vector<Entity>		vEntitys;
	};

	//-------------------------------------------------------------------------//
}

#endif // LEVEL3D_H
