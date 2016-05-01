#ifndef MOUSECURSOR
#define MOUSECURSOR

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// SYSTEM
/////////////////
#include <string>
using namespace std;

///////////////////
// SFML
//////////////////
#include <SFML\Graphics.hpp>
using namespace sf;

namespace le
{
	//-------------------------------------------------------------------------//
	
	class DLL_API MouseCursor
	{
	public:
		//////////////////////////////
		/// КОНСТРУКТОР ПО УМОЛЧАНИЮ
		//////////////////////////////
		MouseCursor();

		/////////////////////////
		/// ЗАГРУЗИТЬ ТЕКСТУРУ
		////////////////////////
		void LoadTexture( const string sRoute );

		//////////////////////////////
		/// ОБНОВЛЕНИЕ ПОЗИЦИИ МЫШИ
		//////////////////////////////
		void UpdatePosition( RenderWindow& RenderWindow );

		///////////////////////
		/// НАРИСОВАТЬ КУРСОР
		//////////////////////
		void DrawCursor( RenderWindow& RenderWindow );

		//////////////////////////////
		/// ПОЛУЧИТЬ ВЕКТОР КУРСОРА
		//////////////////////////////
		Vector2i GetVectorCursor() const;

		///////////////////////////////////
		/// ПОЛУЧИТЬ ПРЯМОУГОЛЬНИК КУРСОРА
		//////////////////////////////////
		FloatRect& GetRectCursor();
	private:
		FloatRect       RectCursor;
		Texture         Texture;
		Sprite          Sprite;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // MOUSECURSOR

