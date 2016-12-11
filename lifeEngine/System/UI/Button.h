#ifndef BUTTON_H
#define BUTTON_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "../System.h"
#include "Text.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class DLL_API Button
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		////////////////
		Button( System& System );

		/////////////////
		/// ДЕСТРУКТОР
		////////////////
		~Button();

		//////////////////////
		/// ЗАГРУЗИТЬ ШРИФТ
		/////////////////////
		void LoadFont( const string sRoute );

		////////////////////
		/// СОЗДАТЬ КНОПКУ
		///////////////////
		void CreateButton( const String sText , const int iSize , Vector2f PositionText , Color ColorText );
		template<typename T> void CreateButton( const String sText , const int iSize , Vector2f PositionText , Color ColorText, T Value );
		void CreateButton( sf::Text Text );

		////////////////////
		/// ОБНОВИТЬ КНОПКУ
		///////////////////
		void ButtonUpdate();

		//////////////////////////////////////////////////////////////
		/// ДЕЙСТВИЯ КНОПКИ КОГДА ОНА НАЖАТА [ВИРТУАЛЬНЫЙ МЕТОД]
		//////////////////////////////////////////////////////////////
		virtual void Actions() = 0;

		/////////////////////
		/// ЗАДАТЬ ID КНОПКЕ
		////////////////////
		void SetId( int id );

		///////////////////////////
		/// ЗАДАТЬ ЦВЕТ ВЫДЕЛЕНИЯ
		//////////////////////////
		void SetColorSelect( Color Color );

		/////////////////////////////
		/// ЗАДАТЬ СТАНДАРТНЫЙ ЦВЕТ
		////////////////////////////
		void SetColorDefoult( Color Color );

		//////////////////
		/// ЗАДАТЬ ШРИФТ
		//////////////////
		void SetFont( const Font Font );

		///////////////////////////
		/// ЗАДАТЬ ТЕКСТ В КНОПКЕ
		//////////////////////////
		void SetText( const String sText );

		///////////////////////////
		/// ЗАДАТЬ РАЗМЕР КНОПКЕ
		//////////////////////////
		void SetSize( const int iSize );

		///////////////////////////
		/// КЛИКНУТА ЛИ КНОПКА
		//////////////////////////
		bool IsClick() const;

		/////////////////////////////
		/// ПОЛУЧИТЬ ТЕКСТ В КНОПКЕ
		/////////////////////////////
		String GetTextInButton();
	private:
		bool                bClick;

		int					iIdButton;

		le::Text*           Text;
		MouseCursor*        MouseCursor;
		Font                Font;
		Color               ColorSelect;
		Color               ColorDefoult;
	};
	
	//-------------------------------------------------------------------------//

	template<typename T> void Button::CreateButton( const String sText , const int iSize , Vector2f PositionText , Color ColorText, T Value )
	{
		Text->SetFont( Font );

		Text->WriteText( sText, iSize, PositionText, ColorText, Value );
		ColorDefoult = ColorText;
	}

	//-------------------------------------------------------------------------//
}

#endif // BUTTON_H
