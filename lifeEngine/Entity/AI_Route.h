#ifndef AI_ROUTE_H
#define AI_ROUTE_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "../System/leSystem.h"

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API AI_Route
	{
	public:

		////////////////
		/// КОНСТРУКТОР
		////////////////
		AI_Route();
		AI_Route( FloatRect Rect , bool NotUseY = false );
		AI_Route( FloatRect Rect , string NextRoute , bool NotUseY = false );

		//////////////////////////////
		/// ПОЛУЧИТЬ СЛЕДУЮЩИЮ ТОЧКУ
		//////////////////////////////
		string GetNextRoute();

		///////////////////////////////////////////////////////////////////////////
		/// ИСПОЛЬЗОВАТЬ ЛИ КООРДИНАТУ 'Y' В ПЕРЕДВИЖЕНИИ ( Т.Е ИДТИ ВВЕРХ/ВНИЗ )
		///////////////////////////////////////////////////////////////////////////
		bool IsUsedY();

		///////////////////////////////////
		/// ПОЛУЧИТЬ ПРЯМОУГОЛЬНИК ТОЧКИ
		//////////////////////////////////
		FloatRect GetRect();
	private:
		bool				bNotUseY;

		FloatRect			RectRoute;
		string				sNextRoute;
	};

	//-------------------------------------------------------------------------//
}

#endif // AI_ROUTE_H

