#ifndef LIGHT_H
#define LIGHT_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include "../System/leSystem.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class DLL_API Light2D
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
		void CreateLight( Vector2f Position, float Radius, Color Color, string NameLight );

		////////////////////////
		/// ЗАДАТЬ РАДИУС СВЕТА
		////////////////////////
		void SetRadius( float Radius );

		/////////////////////////
		/// ЗАДАТЬ ПОЗИЦИЮ СВЕТА
		////////////////////////
		void SetPosition( Vector2f Position );

		///////////////////////
		/// ЗАДАТЬ ЦВЕТ СВЕТА
		//////////////////////
		void SetColor( Color Color );

		///////////////////
		/// ПОЛУЧИТЬ СВЕТ
		//////////////////
		CircleShape GetCircleShape();

		///////////////////////////
		/// ПОЛУЧИТЬ РАДИУС СВЕТА
		//////////////////////////
		float GetRadius();

		///////////////////////////
		/// ПОЛУЧИТЬ ПОЗИЦИЮ СВЕТА
		//////////////////////////
		Vector2f GetPosition();

		///////////////////////////
		/// ПОЛУЧИТЬ ЦВЕТ СВЕТА
		//////////////////////////
		Color GetColor();

		//////////////////////////////////
		/// ПОЛУЧИТЬ ПРЯМОУГОЛЬНИК СВЕТА
		/////////////////////////////////
		FloatRect GetRect();

		////////////////////////
		/// ПОЛУЧИТЬ ИМЯ СВЕТА
		////////////////////////
		string GetNameLight();
	private:
		string				sNameLight;

		Texture				Texture;
		CircleShape			CircleShape;
		FloatRect			Rect;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // LIGHT_H
