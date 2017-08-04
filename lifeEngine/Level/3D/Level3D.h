#ifndef LEVEL3D_H
#define LEVEL3D_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include <System/3D/Scene3D.h>
#include <System/System.h>
#include <Physic/3D/Physic3D.h>
#include "Brush.h"
#include "Entity.h"

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API Level3D
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		////////////////
		Level3D( System& System, Scene3D& Scene, Physic3D& Physic );
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

		////////////////////////////////
		/// ДОБАВИТЬ УРОВЕНЬ НА СЦЕНУ
		////////////////////////////////
		void AddToScene( Scene3D& Scene );

		///////////////////////
		/// ОТРЕНДЕРИТЬ УРОВЕНЬ
		//////////////////////
		void UpdateLevel( Shader* Shader = NULL );

		///////////////////////
		/// ПОЛУЧИТЬ ЭНТИТИ
		//////////////////////
		Entity* GetEntity( string NameEntity );

		//////////////////////////
		/// ПОЛУЧИТЬ ВСЕ ЭНТИТИ
		//////////////////////////
		vector<Entity>& GetAllEntitys();

		//////////////////////////
		/// ПОЛУЧИТЬ ВСЕ БРАШИ
		//////////////////////////
		vector<Brush*>& GetAllBrushes();

	private:

		string				sNameMap;
		string				sDescriptionMap;
		string				sSkyBoxName;

		System*				System;
		Physic3D*			Physic;
		Scene3D*			Scene;

		vector<Brush*>		vBrushes;
		vector<Entity>		vEntitys;
	};

	//-------------------------------------------------------------------------//
}

#endif // LEVEL3D_H
