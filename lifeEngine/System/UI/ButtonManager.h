#ifndef BUTTONMANAGER_H
#define BUTTONMANAGER_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "../System.h"
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

		////////////////////
		/// СОЗДАТЬ КНОПКУ
		///////////////////
		void CreateButton( Button* Button );

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
		Color						ColorSelect;
		vector<Button*>             vButton;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // BUTTONMANAGER_H
