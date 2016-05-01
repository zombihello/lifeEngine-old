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
		void CreateButton( const string sText , const int iSize , Vector2f PositionText , Color ColorText );

		////////////////////
		/// ОБНОВИТЬ КНОПКУ
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

		/////////////////////////////
		/// ЗАДАТЬ СТАНДАРТНЫЙ ЦВЕТ
		////////////////////////////
		void SetColorDefoult( const int IdButton, Color Color );

		///////////////////////////
		/// ЗАДАТЬ ТЕКСТ В КНОПКЕ
		//////////////////////////
		void SetText( const int IdButton, const string sText );

		///////////////////////////
		/// ЗАДАТЬ РАЗМЕР КНОПКЕ
		//////////////////////////
		void SetSize( const int IdButton, const int iSize );

		////////////////////////////////
		/// ПОЛУЧАЕМ ТЕКУЩИЙ ID КНОПКИ
		////////////////////////////////
		int GetTmpIdButton() const;
	private:
		int                         iIdButton;

		System*                     System;
		Font                        Font;
		vector<Button*>             vButton;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // BUTTONMANAGER_H
