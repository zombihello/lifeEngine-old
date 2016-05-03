#ifndef BUTTON_H
#define BUTTON_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "System.h"
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
		void CreateButton( const string sText , const int iSize , Vector2f PositionText , Color ColorText );
		template<typename T> void CreateButton( const string sText , const int iSize , Vector2f PositionText , Color ColorText, T Value );
		void CreateButton( sf::Text Text );

		////////////////////
		/// ОБНОВИТЬ КНОПКУ
		///////////////////
		void ButtonUpdate();

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
		void SetText( const string sText );

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
		string GetTextInButton();
	private:
		bool                bClick;

		int					iIdButton;

		le::Text*           Text;
		MouseCursor*        MouseCursor;
		FloatRect           RectButton;
		Font                Font;
		Color               ColorSelect;
		Color               ColorDefoult;
	};
	
	//-------------------------------------------------------------------------//

	template<typename T> void Button::CreateButton( const string sText , const int iSize , Vector2f PositionText , Color ColorText, T Value )
	{
		Text->SetFont( Font );

		Text->WriteText( sText, iSize, PositionText, ColorText, Value );
		ColorDefoult = ColorText;

		RectButton = FloatRect( PositionText, Vector2f( iSize * sText.size() / 1.5 , iSize ) );
	}

	//-------------------------------------------------------------------------//
}

#endif // BUTTON_H
