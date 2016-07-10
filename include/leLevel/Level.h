#ifndef LEVEL_H
#define LEVEL_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "Object.h"
#include "Layer.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class DLL_API Level
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		////////////////
		Level( le::System& System );

		/////////////////
		/// ДЕСТРУКТОР
		////////////////
		~Level();

		///////////////////////
		/// ЗАГРУЗИТЬ УРОВЕНЬ
		//////////////////////
		bool LoadLevel( const string sRoute, Physic& Physic );

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
		Object& GetObject( const string sName );

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
	private:
		int                 iFirstTileID;
		int                 iWidth;
		int                 iHeight;
		int                 iTileWidth;
		int                 iTileHeight;

		Rect<float>         drawingBounds;
		Texture             tilesetImage;
		le::System*			System;
		vector<Object>      vObjects;
		vector<Layer>       vLayers;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // LEVEL_H
