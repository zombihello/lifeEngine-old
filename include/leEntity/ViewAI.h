#ifndef VIEWAI_H
#define VIEWAI_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

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

		/////////////////////////////
		/// ИНИЦИАЛИЗАЦИЯ ОБЗОРА ИИ
		/////////////////////////////
		void InitViewAI( BasicPersonages* Personage, GroupColision GroupColision );

		/////////////////////////
		/// ЗАДАТЬ РЕЖИМ ДЕБАГА
		////////////////////////
		void SetDebug( bool debug );

		//////////////////////
		/// УВИДЕЛ ЛИ ВРАГА
		/////////////////////
		bool IsLook();

		//////////////////////////////
		/// ПОЛУЧИТЬ УВИДЕНОГО ВРАГА
		//////////////////////////////
		BasicPersonages* GetLookPersonage();
	private:
		//////////////////////////////
		/// НАРИСОВАТЬ "ЗРЕНИЕ" ИИ
		//////////////////////////////
		void DrawView();

		int							iIdPersonage;

		bool						bIsLook;
		bool						bDebug;

		le::Body*					Body;
		BasicPersonages*			PersonageAI;
		BasicPersonages*			LookPersonage;
		RenderWindow*				Window;
		RectangleShape				View;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // VIEWAI_H
