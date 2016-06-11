#ifndef BUTTONMANAGER_H
#define BUTTONMANAGER_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "System.h"
#include "Button.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class DLL_API ButtonManager
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		////////////////
		ButtonManager( System& System );

		/////////////////
		/// ДЕСТРУКТОР
		////////////////
		~ButtonManager();

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
		/// ОБНОВИТЬ КНОПКИ
		///////////////////
		void ButtonsUpdate();

		////////////////////////
		/// УДАЛЯЕМ ВСЕ КНОПКИ
		////////////////////////
		void DeleteAllButtons();

		//////////////////////////
		/// УДАЛЯЕМ КНОПКУ ПО ID
		//////////////////////////
		void DeleteButton( const int id );

		//////////////////
		/// ЗАДАТЬ ШРИФТ
		//////////////////
		void SetFont( const Font Font );

		//////////////////////////
		/// ЗАДАТЬ ЦВЕТ ВЫДЕЛЕНИЯ
		//////////////////////////
		void SetColorSelect( const int IdButton, Color Color );
		void SetColorSelect( Color Color );

		/////////////////////////////
		/// ЗАДАТЬ СТАНДАРТНЫЙ ЦВЕТ
		////////////////////////////
		void SetColorDefoult( const int IdButton, Color Color );
		void SetColorDefoult( Color Color );

		///////////////////////////
		/// ЗАДАТЬ ТЕКСТ В КНОПКЕ
		//////////////////////////
		void SetText( const int IdButton, const String sText );

		///////////////////////////
		/// ЗАДАТЬ РАЗМЕР КНОПКЕ
		//////////////////////////
		void SetSize( const int IdButton, const int iSize );
		void SetSize( const int iSize );

		////////////////////
		/// ПОЛУЧИТЬ КНОПКУ
		////////////////////
		Button* GetButton( int id );
		Button* GetButton( String textInButton );

		///////////////////////
		/// ПОЛУЧИТЬ ID КНОПКИ
		///////////////////////
		const int GetIdButton( String textInButton );

		/////////////////////////
		/// ПОЛУЧИТЬ ВСЕ КНОПКИ
		/////////////////////////
		vector<Button*>& GetAllButtons();

		////////////////////////////////
		/// ПОЛУЧАЕМ ТЕКУЩИЙ ID КНОПКИ
		////////////////////////////////
		int GetTmpIdButton() const;
	private:
		int                         iIdButton;

		System*                     System;
		Font                        Font;
		Color						ColorSelect;
		vector<Button*>             vButton;
	};
	
	//-------------------------------------------------------------------------//

	template<typename T> void ButtonManager::CreateButton( const String sText , const int iSize , Vector2f PositionText , Color ColorText, T Value )
	{
		le::Button* Button = new le::Button( *System );
		Button->SetFont( Font );
		Button->CreateButton( sText, iSize, PositionText, ColorText, Value );

		vButton.push_back( Button );
	}

	//-------------------------------------------------------------------------//
}

#endif // BUTTONMANAGER_H
