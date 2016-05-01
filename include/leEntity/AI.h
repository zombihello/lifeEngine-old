#ifndef AI_H
#define AI_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "ViewAI.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class DLL_API AI : private ViewAI
	{
	public:
		AI( System& System );

		//////////////////
		/// ОБНОВЛЕНИЕ ИИ
		//////////////////
		void UpdateAI( vector<BasicPersonages*> vPersonage );

		//////////////////////
		/// ИИ УВИДЕЛ ВРАГА
		/////////////////////
		bool IsLook();

		//////////////////////////////
		/// ПОЛУЧИТЬ УВИДЕНОГО ВРАГА
		//////////////////////////////
		BasicPersonages* GetLookPersonage();
	protected:
		//////////////////////////
		/// ИНИЦИАЛИЗАЦИЯ ИИ
		//////////////////////////
		void InitAI( BasicPersonages* Personage );

		/////////////////
		/// ДЕЙСТВИЯ ИИ
		/////////////////
		virtual void Actions() = 0;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // AI_H
