#ifndef VIEWAI_H
#define VIEWAI_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

#define CATEGORY_VIEW_AI 0x0002

//////////////////
// LIFEENGINE
/////////////////
#include "BasicPersonages.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class DLL_API ViewAI
	{
	public:
		////////////////
		/// КОНСТРУКТОР
		////////////////
		ViewAI( System& System );

		////////////////
		/// ДЕСТРУКТОР
		////////////////
		~ViewAI();

		////////////////////////////
		/// ОБНОВЛЕНИЕ ЗРЕНИЯ У ИИ
		///////////////////////////
		void UpdateViewAI( vector<BasicPersonages*> vPersonage );
	protected:
		/////////////////////////////
		/// ИНИЦИАЛИЗАЦИЯ ОБЗОРА ИИ
		/////////////////////////////
		void InitViewAI( BasicPersonages* Personage );

		bool						bIsLook;

		BasicPersonages*			PersonageAI;
		BasicPersonages*			LookPersonage;
	private:
		int							iIdPersonage;

		Body*						Body;
		RenderWindow*				Window;
		RectangleShape				View;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // VIEWAI_H
