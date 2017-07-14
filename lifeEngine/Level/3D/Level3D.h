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
#include <Physic/3D/Physic3D.h>

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API Level3D
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		////////////////
		Level3D( System& System, Physic3D& Physic );

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
		Entity* GetEntity( string NameEntity );

		//////////////////////////
		/// ПОЛУЧИТЬ ВСЕ ЭНТИТИ
		//////////////////////////
		vector<Entity>& GetAllEntitys();

	private:

		string				sNameMap;
		string				sDescriptionMap;
		string				sSkyBoxName;

		System*				System;
		Physic3D*			Physic;

		vector<Brush*>		vBrushes;
		vector<Entity>		vEntitys;
	};

	//-------------------------------------------------------------------------//
}

#endif // LEVEL3D_H
