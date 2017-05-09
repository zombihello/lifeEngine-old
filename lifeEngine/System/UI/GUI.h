#ifndef GUI_H
#define GUI_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include "ButtonManager.h"
#include "TextManager.h"

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API GUI
	{
	public:
		//////////////////
		/// КОНСТРУКТОР
		//////////////////
		GUI( System& System );

		////////////////////////////
		/// ЗАДАТЬ КАМЕРУ GUI
		////////////////////////////
		void SetView( View View );

		//////////////////////////
		/// ДОБАВИТЬ ТЕКСТ В GUI
		//////////////////////////
		void AddText( le::Text* Text );

		////////////////////////////////////
		/// ДОБАВИТЬ МЕНЕДЖЕР ТЕКСТА В GUI
		///////////////////////////////////
		void AddTextManager( TextManager* TextManager );

		//////////////////////////
		/// ДОБАВТЬ КНОПКУ В GUI
		//////////////////////////
		void AddButton( Button* Button );

		/////////////////////////////////////
		/// ДОБАВИТЬ МЕНЕДЖЕР КНОПОК В GUI
		/////////////////////////////////////
		void AddButtonManager( ButtonManager* ButtonManager );

		/////////////////////
		/// НАРИСОВАТЬ GUI
		/////////////////////
		void UpdateGUI();

		/////////////////
		/// ОЧИСТИТЬ GUI
		/////////////////
		void ClearGUI();

		//////////////////////////////
		/// ПОЛУЧИТЬ КАМЕРУ ДЛЯ GUI
		//////////////////////////////
		View GetView();

	private:

		vector<Text*>					vText;
		vector<Button*>					vButton;
		vector<ButtonManager*>			vButtonManager;
		vector<TextManager*>			vTextManager;

		System*							System;
		View							ViewGUI;
	};

	//-------------------------------------------------------------------------//
}

#endif // GUI_H

