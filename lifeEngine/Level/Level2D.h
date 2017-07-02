#ifndef LEVEL2D_H
#define LEVEL2D_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "Object.h"
#include "Layer.h"
#include "../Entity/AI_Route.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class DLL_API Level2D
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		////////////////
		Level2D( le::System& System );

		/////////////////
		/// ДЕСТРУКТОР
		////////////////
		~Level2D( );

		///////////////////////
		/// ЗАГРУЗИТЬ УРОВЕНЬ
		//////////////////////
		bool LoadLevel( const string sRoute , Physic2D& Physic , bool bSmoothTextures = false );

		///////////////////////
		/// ОЧИСТИТЬ УРОВЕНЬ
		//////////////////////
		void ClearLevel();

		///////////////////////
		/// ОТРИСОВАТЬ УРОВЕНЬ
		//////////////////////
		void DrawLevel();

		///////////////////////
		/// ПОЛУЧИТЬ ОБЪЕКТ
		//////////////////////
		Object* GetObject( const string sName );

		///////////////////////
		/// ПОЛУЧИТЬ ОБЪЕКТЫ
		//////////////////////
		vector<Object> GetObjects( const string sName );

		/////////////////////////
		/// ПОЛУЧИТЬ ВСЕ ОБЪЕКТЫ
		/////////////////////////
		vector<Object>& GetAllObjects();

		///////////////////////////
		/// ПОЛУЧИТЬ РАЗМЕР ТАЙЛА
		///////////////////////////
		Vector2i GetTileSize() const;

		///////////////////////////
		/// ПОЛУЧИТЬ РАЗМЕР КАРТЫ
		///////////////////////////
		Vector2i GetMapSize() const;

		///////////////////////////
		/// ПОЛУЧИТЬ ПУТИ ИИ 
		///////////////////////////
		map<string , AI_Route> GetRoutesAI();
	private:

		int							iWidth;
		int							iHeight;
		int							iTileWidth;
		int							iTileHeight;

		Rect<float>					drawingBounds;
		le::System*					System;
		vector<Texture>				vTextures;
		vector<Object>				vObjects;
		vector<Layer>				vLayers;
		map<string , AI_Route>		mRoutesAI;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // LEVEL2D_H
