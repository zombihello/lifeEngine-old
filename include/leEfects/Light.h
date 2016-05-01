#ifndef LIGHT_H
#define LIGHT_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include "../leSystem/leSystem.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class DLL_API Light
	{
	public:
		///////////////////////////////
		/// ЗАГРУЗИТЬ МАСКУ ДЛЯ СВЕТА
		///////////////////////////////
		void SetMask( Texture& Texture );

		///////////////////
		/// СОЗДАТЬ СВЕТ
		//////////////////
		void CreateLight( Vector2f Position, float Radius, Color Color );

		///////////////////
		/// ПОЛУЧИТЬ СВЕТ
		//////////////////
		CircleShape GetLight();

		///////////////////////////////////
		/// ПОЛУЧИТЬ ПРЯМОУГОЛЬНИК СВЕТА
		///////////////////////////////////
		FloatRect GetRect();
	private:
		Texture				Texture;
		CircleShape			CircleShape;
		FloatRect			Rect;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // LIGHT_H
