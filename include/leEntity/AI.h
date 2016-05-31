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
	
	class DLL_API AI //: private ViewAI
	{
	public:
		////////////////
		/// КОНСТРУКТОР
		////////////////
		AI( System& System );

		////////////////
		/// ДЕСТРУКТОР
		////////////////
		~AI();

		//////////////////
		/// ОБНОВЛЕНИЕ ИИ
		//////////////////
		void UpdateAI( vector<BasicPersonages*> vPersonage );

		/////////////////////////
		/// ЗАДАТЬ РЕЖИМ ДЕБАГА
		////////////////////////
		void SetDebug( bool debug );

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
		void InitAI( BasicPersonages* Personage, GroupColision GroupColision );

		/////////////////
		/// ДЕЙСТВИЯ ИИ
		/////////////////
		virtual void Actions() = 0;
	private:
		le::ViewAI*				ViewAI;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // AI_H
