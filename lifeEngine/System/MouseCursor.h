#ifndef MOUSECURSOR
#define MOUSECURSOR

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// SYSTEM
/////////////////
#include <string>
#include <vector>
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
		void LoadTexture( const string sRoute, bool bSmooth = false );

		//////////////////////////////
		/// ОБНОВЛЕНИЕ ПОЗИЦИИ МЫШИ
		//////////////////////////////
		void UpdatePosition( RenderWindow& RenderWindow );

		///////////////////////
		/// НАРИСОВАТЬ КУРСОР
		//////////////////////
		void DrawCursor( RenderWindow& RenderWindow );

		////////////////////////////////////
		/// ПРОВЕРКА НА НАЖАТИЕ КНОПКИ МЫШИ
		////////////////////////////////////
		bool ButtonPressed( Mouse::Button Button );

		//////////////////////////////
		/// ПОЛУЧИТЬ ВЕКТОР КУРСОРА
		//////////////////////////////
		Vector2i GetVectorCursor() const;

		///////////////////////////////////
		/// ПОЛУЧИТЬ ПРЯМОУГОЛЬНИК КУРСОРА
		//////////////////////////////////
		FloatRect& GetRectCursor();
	private:
		FloatRect					RectCursor;
		Texture						Texture;
		Sprite						Sprite;
		vector<Mouse::Button>		vButtonsPressed;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // MOUSECURSOR

