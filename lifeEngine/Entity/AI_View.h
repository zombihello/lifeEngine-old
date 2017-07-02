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
	
	class DLL_API AI_View
	{
	public:
		////////////////
		/// КОНСТРУКТОР
		////////////////
		AI_View( System& System );

		////////////////
		/// ДЕСТРУКТОР
		////////////////
		~AI_View();

		////////////////////////////
		/// ОБНОВЛЕНИЕ ЗРЕНИЯ У ИИ
		///////////////////////////
		void UpdateViewAI( vector<BasicPersonages*> vPersonage );

		/////////////////////////////
		/// ИНИЦИАЛИЗАЦИЯ ОБЗОРА ИИ
		/////////////////////////////
		void InitViewAI( BasicPersonages* Personage, GroupColision GroupColision );

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

		le::Body2D*					Body;
		BasicPersonages*			PersonageAI;
		BasicPersonages*			LookPersonage;
		le::System*					System;
		RectangleShape				View;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // VIEWAI_H
